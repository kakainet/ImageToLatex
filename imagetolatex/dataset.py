from __future__ import division, print_function

import multiprocessing.pool
import json
import os
import os.path
import re
import itertools
import logging

from keras.utils import Sequence, Progbar
from keras.preprocessing.image import load_img, img_to_array
import numpy as np

from utils import Logger, memory_usage


__all__ = ['FSSequence', 'NPSequence', 'load_fs', 'load_np']


logging.setLoggerClass(Logger)
logger = logging.getLogger(__name__) 


_METADATA_FILE_NAME = '__info__.json'


def _parse_indexes(path):
    _, name = os.path.split(path)
    return (int(x.group()) for x in re.finditer(r'[0-9]+', name))

def _list_files(path):
    return [
        os.path.join(path, x)
        for x in filter(lambda x : not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]

def parallelize(function, iterable, thread_count=None):
    if thread_count is not None:
        pool = multiprocessing.pool.ThreadPool(thread_count)
        return pool.imap_unordered(
            lambda x : (x[0], function(x[1])), enumerate(iterable)
        )
    else:
        return enumerate(map(function, iterable))


class FSSequence(Sequence):

    def __init__(self, label_paths, feature_paths, batch_size, batch_count, variant_count,
                feature_shape=(256, 256, 3), feature_dtype='float32', label_dtype='U128', thread_count=None, **kwargs):

        self._label_paths = label_paths
        self._feature_paths = feature_paths

        self._batch_size = batch_size
        self._batch_count = batch_count
        self._variant_count = variant_count

        self._feature_shape = feature_shape
        self._feature_dtype = feature_dtype
        self._label_dtype = label_dtype
        
        self._thread_count = thread_count
        self._kwargs = kwargs

    @classmethod
    def of_path(cls, input_path, batch_size, batch_count, variant_count,
                feature_shape=(256, 256, 3), feature_dtype='float32', label_dtype='U128', thread_count=None, **kwargs):

        logger.debug('sequence [path]: %s', input_path)
        logger.debug('batch [size, count]: %d, %d', batch_size, batch_count)
        logger.debug('variant [count]: %d', variant_count)
        logger.debug('feature [shape, dtype]: %s, %s', feature_shape, feature_dtype)
        logger.debug('label [dtype]: %s', label_dtype)

        label_paths = [None] * batch_count

        for label_path in _list_files(os.path.join(input_path, 'labels')):
            label_index = next(_parse_indexes(label_path))
            label_paths[label_index] = label_path
        
        feature_paths = [
            [[] for _ in range(batch_size)]
            for _ in range(batch_count)
        ]

        for feature_path in _list_files(os.path.join(input_path, 'features')):
            feature_index = next(_parse_indexes(feature_path))
            batch_index = feature_index // batch_size
            logger.trace('batch: %d, feature: %d, %s', batch_index, feature_index, feature_path)
            feature_paths[batch_index][feature_index % batch_size].append(feature_path)

        return cls(
            label_paths, feature_paths,
            batch_size, batch_count, variant_count,
            feature_shape, 
            feature_dtype, label_dtype,
            thread_count,
            **kwargs
        )

    @property
    def batch_size(self):
        return self._batch_size

    @property
    def batch_count(self):
        return self._batch_count

    @property
    def variant_count(self):
        return self._variant_count

    def subset(self, iterable):
        iterable = sorted(iterable)

        label_paths = [self._label_paths[i] for i in iterable]
        feature_paths = [self._feature_paths[i] for i in iterable]

        return FSSequence(
            label_paths, feature_paths,
            self._batch_size, self._batch_count, self._variant_count,
            self._feature_shape,
            self._feature_dtype, self._label_dtype,
            self._thread_count,
            **self._kwargs
        )

    def load_batch(self, idx):
        logger.info('loading batch: %d', idx)

        unedited_labels = _load_lines(self._label_paths[idx])
        
        feature_paths = self._feature_paths[idx]
        feature_count = self._batch_size * self._variant_count
        
        logger.info('loading features')

        features = self.load_features(
            itertools.chain.from_iterable(feature_paths),
            feature_shape=self._feature_shape,
            feature_dtype=self._feature_dtype,
            feature_count=feature_count,
            thread_count=self._thread_count,
            **self._kwargs
        )

        logger.info('loading labels')

        labels = np.fromiter(
            itertools.chain.from_iterable(
                itertools.repeat(unedited_labels[i], self._variant_count)
                for i in range(self._batch_size)
            ),
            dtype=self._label_dtype, count=feature_count
        )

        return features, labels

    def __getitem__(self, idx):
        return self.load_batch(idx)

    def __len__(self):
        return self._batch_count

    @staticmethod
    def load_feature(feature_path, feature_shape=None, **kwargs):
        return img_to_array(load_img(feature_path, target_size=feature_shape, **kwargs))

    @staticmethod
    def load_features(feature_paths, feature_shape, feature_dtype='float32', feature_count=None, thread_count=None, **kwargs):
        if feature_count is None:
            feature_count = len(feature_paths)
      
        features = np.empty((feature_count, *feature_shape), dtype=feature_dtype)

        progress_bar = Progbar(feature_count)

        logger.debug('before memory usage: %d, estimated growth: %d', memory_usage(), features.nbytes / (1024 * 1024))

        for feature_index, feature in parallelize(lambda x : FSSequence.load_feature(x, feature_shape, **kwargs), feature_paths, thread_count):
            features[feature_index] = feature
            progress_bar.add(1)
    
        logger.debug('after memory usage: %d', memory_usage())

        features /= 255
        return features
    
    @staticmethod
    def save_array(array_path, *args, compressed=True, **kwargs):
        save = np.savez_compressed if compressed else np.savez
        save(array_path, *args, **kwargs)

    def as_np(self, output_path, sub_batch_count, batch_name='batch', compressed=True):
        if self._batch_size % sub_batch_count != 0:
            raise ValueError('Batch size must be divisible by sub_batch count')

        batch_output_path = os.path.join(output_path, 'batches')

        if not os.path.exists(batch_output_path):
            os.makedirs(batch_output_path)

        if sub_batch_count < 1:
            sub_batch_count = 1

        batch_size = self._batch_size * self._variant_count
        sub_batch_size = batch_size // sub_batch_count

        for batch_index, (batch_features, batch_labels) in enumerate(self):
            logger.info('saving batch: %d', batch_index)

            iterable = (
                (
                    os.path.join(batch_output_path, '{}{}_{}'.format(batch_name, batch_index, sub_batch_index)),
                    batch_features[sub_batch_index * sub_batch_size : (sub_batch_index + 1) * sub_batch_size],
                    batch_labels[sub_batch_index * sub_batch_size : (sub_batch_index + 1) * sub_batch_size]
                )
                for sub_batch_index in range(sub_batch_count)
            )

            function = lambda x : FSSequence.save_array(x[0], compressed=compressed, features=x[1], labels=x[2])

            for _ in parallelize(function, iterable, self._thread_count):
                pass
   
        with open(os.path.join(output_path, _METADATA_FILE_NAME), 'w') as ofs:
            json.dump({
                'batch_size' : batch_size,
                'batch_count' : self._batch_count,
                'sub_batch_size' : sub_batch_size,
                'sub_batch_count' : sub_batch_count,
                'feature_shape' : self._feature_shape,
                'feature_dtype' : self._feature_dtype,
                'label_dtype' : self._label_dtype,
            }, ofs, indent=4)

        return NPSequence.of_path(
            output_path,
            batch_size, self._batch_count,
            sub_batch_size, sub_batch_count,
            self._feature_shape,
            self._feature_dtype, self._label_dtype,
            self._thread_count
        )


class NPSequence(Sequence):

    def __init__(self, batch_paths, batch_size, batch_count, sub_batch_size, sub_batch_count,
                feature_shape=(256, 256, 3), feature_dtype='float32', label_dtype='U128', thread_count=None):
        
        self._batch_paths = batch_paths
        
        self._batch_size = batch_size
        self._batch_count = batch_count

        self._sub_batch_size = sub_batch_size
        self._sub_batch_count = sub_batch_count

        self._feature_shape = feature_shape
        self._feature_dtype = feature_dtype
        self._label_dtype = label_dtype

        self._thread_count = thread_count

    @classmethod
    def of_path(cls, input_path, batch_size, batch_count, sub_batch_size, sub_batch_count, 
                feature_shape=(256, 256, 3), feature_dtype='float32', label_dtype='U128', thread_count=None):

        logger.debug('sequence [path]: %s', input_path)
        logger.debug('batch [size, count]: %d, %d', batch_size, batch_count)
        logger.debug('sub_batch [size, count]: %d, %d', sub_batch_size, sub_batch_count)
        logger.debug('feature [shape, dtype]: %s, %s', feature_shape, feature_dtype)
        logger.debug('label [dtype]: %s', label_dtype)

        unsorted_batch_paths = _list_files(os.path.join(input_path, 'batches'))

        batch_paths = [
            [None] * sub_batch_count
            for _ in range(batch_count)
        ]

        for batch_path in unsorted_batch_paths:
            batch_index, sub_batch_index = itertools.islice(_parse_indexes(batch_path), 2)
            batch_paths[batch_index][sub_batch_index] = batch_path
            logger.trace('batch: %d, sub_batch: %d, %s', batch_index, sub_batch_index, batch_path)

        return cls(
            batch_paths,
            batch_size, batch_count,
            sub_batch_size, sub_batch_count,
            feature_shape,
            feature_dtype, label_dtype,
            thread_count
        )

    def subset(self, iterable):
        return NPSequence(
            [self._batch_paths[i] for i in sorted(iterable)],
            self._batch_size, self._batch_count,
            self._sub_batch_size, self._sub_batch_count,
            self._feature_shape,
            self._feature_dtype, self._label_dtype,
            self._thread_count
        )

    @staticmethod
    def load_array(array_path):
        with np.load(array_path, allow_pickle=False) as ifs:
            return ifs['features'], ifs['labels']

    def load_batch(self, idx):
        batch_paths = self._batch_paths[idx]
        batch_shape = self._batch_size, *self._feature_shape  

        features, labels = np.empty(batch_shape, dtype=self._feature_dtype), np.empty(self._batch_size, dtype=self._label_dtype)
        
        progress_bar = Progbar(self._batch_size)
        logger.info('loading sub_batches')

        for sub_index, (sub_features, sub_labels) in parallelize(NPSequence.load_array, batch_paths, self._thread_count):
            features[sub_index * self._sub_batch_size : (sub_index + 1) * self._sub_batch_size] = sub_features
            labels[sub_index * self._sub_batch_size : (sub_index + 1) * self._sub_batch_size] = sub_labels
            progress_bar.add(self._sub_batch_size)

        return features, labels

    def __getitem__(self, idx):
        return self.load_batch(idx)

    def __len__(self):
        return self._batch_count

    def as_fs(self):
        raise NotImplementedError


def load_metadata(input_path):
    logger.info('loading metadata: %s', input_path)
    with open(os.path.join(input_path, _METADATA_FILE_NAME), 'r') as ifs:
        return json.load(ifs)


def load_fs(input_path, thread_count=None, **kwargs):
    meta_data = load_metadata(input_path)
    return FSSequence.of_path(
        input_path,
        meta_data['batch_size'], meta_data['batch_count'], meta_data['variant_count'],
        meta_data['feature_shape'],
        meta_data['feature_dtype'], meta_data['label_dtype'],
        thread_count,
        **kwargs
    )

def load_np(input_path, thread_count=None):
    meta_data = load_metadata(input_path)
    return NPSequence.of_path(
        input_path,
        meta_data['batch_size'], meta_data['batch_count'],
        meta_data['sub_batch_size'], meta_data['sub_batch_count'],
        meta_data['feature_shape'],
        meta_data['feature_dtype'], meta_data['label_dtype'], 
        thread_count
    )


if __name__ == '__main__':
    import argparse

    logging.basicConfig(level=logging.INFO)
    logger.setLevel(Logger.TRACE)
    parser = argparse.ArgumentParser()

