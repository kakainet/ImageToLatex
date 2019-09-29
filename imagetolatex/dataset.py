from __future__ import division, print_function

import multiprocessing.pool
import resource
import sys
import os
import os.path
import re
import itertools
import logging

from keras.utils import Sequence
from keras.preprocessing.image import load_img, img_to_array
import numpy as np


__all__ = ['FSSequence', 'NPSequence']

logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)
logger.setLevel(logging.DEBUG) 


def _memory_usage():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / (1024 * 1024)

def _parse_index(path):
    _, name = os.path.split(path)
    return int(re.search(r'[0-9]+', name).group())

def _list_files(path):
    return [
        os.path.join(path, x)
        for x in filter(lambda x : not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]


class FSSequence(Sequence):

    def __init__(self, label_paths, feature_paths, batch_size, variants_count, target_size,
                label_dtype='U128', feature_dtype='float32', thread_count=None, **kwargs):

        self._label_paths = label_paths
        self._feature_paths = feature_paths

        self._batch_size = batch_size
        self._variants_count = variants_count
        self._target_size = target_size

        self._label_dtype = label_dtype
        self._feature_dtype = feature_dtype

        self._thread_count = thread_count
        self._kwargs = kwargs

    @classmethod
    def of_path(cls, path, batch_size, variants_count, target_size,
                label_dtype='U128', feature_dtype='float32', thread_count=None, **kwargs):

        label_paths = sorted(
            _list_files(os.path.join(path, 'labels')), key=_parse_index
        )
        
        feature_paths = sorted(
            _list_files(os.path.join(path, 'features')), key=_parse_index
        )

        features_per_batch = batch_size * variants_count

        feature_paths = [
            feature_paths[i : i + features_per_batch]
            for i in range(0, len(feature_paths), features_per_batch)
        ]

        return cls(
            label_paths, feature_paths,
            batch_size, variants_count, target_size,
            label_dtype, feature_dtype, thread_count,
            **kwargs
        )

    @property
    def batch_size(self):
        return self._batch_size

    @property
    def variants_count(self):
        return self._variants_count

    @property
    def target_size(self):
        return self._target_size

    def subset(self, iterable):
        iterable = sorted(iterable)

        label_paths = [self._label_paths[i] for i in iterable]
        feature_paths = [self._feature_paths[i] for i in iterable]

        return FSSequence(
            label_paths, feature_paths,
            self._batch_size, self._variants_count, self._target_size,
            self._label_dtype, self._feature_dtype, self._thread_count,
            **self._kwargs
        )

    def load_batch(self, idx, size_limit=None):
        unedited_labels = _load_lines(self._label_paths[idx])
        feature_paths = self._feature_paths[idx]

        if size_limit is not None:
            feature_paths = feature_paths[:size_limit * self._variants_count]

        features = self.load_features(
            feature_paths, self._target_size, self._feature_dtype, self._thread_count,
            **self._kwargs
        )

        labels = np.fromiter(
            itertools.chain.from_iterable(itertools.repeat(x, self._variants_count) for x in unedited_labels),
            dtype=self._label_dtype, count=len(unedited_labels) * self._variants_count
        )

        return features, labels

    def __getitem__(self, idx):
        return self.load_batch(idx)

    def __len__(self):
        return len(self._label_paths)

    @staticmethod
    def load_feature(path, target_size=None, **kwargs):
        return img_to_array(load_img(path, target_size=target_size, **kwargs))

    @staticmethod
    def load_features(paths, target_size, feature_dtype='float32', thread_count=None, **kwargs):
        features = np.empty((len(paths), *target_size), dtype=feature_dtype)

        logger.debug('MU %d (+-) %d', _memory_usage(), features.nbytes / (1024 * 1024))

        with multiprocessing.pool.ThreadPool(thread_count) as pool:
            load_function = lambda pair : FSSequence.load_feature(pair[1], target_size, **kwargs)

            for index, feature in enumerate(pool.imap_unordered(load_function, enumerate(paths))):
                sys.stdout.write('\r{0:.2%}'.format((index + 1) / len(paths)))
                features[index, :, :, :] = feature
            sys.stdout.write('\n')

        logger.debug('MU %d', _memory_usage())

        features /= 255
        return features
    
    def as_np(self, path, batch_name='batch'):
        if not os.path.exists(path):
            os.mkdir(path)

        batch_path = os.path.join(path, batch_name)

        for index, (features, labels) in enumerate(self):
            np.savez_compressed(batch_path + str(index), features=features, labels=labels)
            sys.stdout.write('{0:.2%}'.format((index + 1) / len(self)))
        sys.stdout.write('\n')

        return NPSequence.of_path(path)


class NPSequence(Sequence):

    def __init__(self, batch_paths):
        self._batch_paths = batch_paths

    @classmethod
    def of_path(cls, path):
        batch_paths = sorted(
            _list_files(path), key=_parse_index
        )

        return cls(batch_paths)

    def subset(self, iterable):
        iterable = sorted(iterable)

        return NPSequence(
            [self._batch_paths[i] for i in iterable]
        )

    def load_batch(self, idx):
        with np.load(self._batch_paths[idx], allow_pickle=True) as ifs:
            return ifs['features'], ifs['labels']

    def __getitem__(self, idx):
        return self.load_batch(idx)

    def __len__(self):
        return len(self._batch_paths)

    def as_fs(self):
        raise NotImplementedError


if __name__ == '__main__':
    pass

