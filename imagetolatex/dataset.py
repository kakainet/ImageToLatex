from __future__ import division, print_function

import multiprocessing.pool
import multiprocessing
import sys
import os
import os.path
import re
import operator
import functools
import itertools
import collections
import logging

from keras.utils import Sequence
from keras.preprocessing.image import load_img, img_to_array
import numpy as np


logger = logging.getLogger(__name__)
logging.basicConfig(level=logging.INFO)
logger.setLevel(logging.DEBUG) 

def _parse_index(path):
    _, name = os.path.split(path)
    return int(re.search(r'[0-9]+', name).group())

def _list_files(path):
    return [os.path.join(path, x)
        for x in filter(lambda x : not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]


class FSSequence(Sequence):

    def __init__(self, label_paths, feature_paths, batch_size, target_size, batch_indexes=None, thread_count=None, **kwargs):
        self._label_paths = label_paths
        self._feature_paths = feature_paths

        self._batch_size = batch_size
        self._target_size = target_size

        self._thread_count = thread_count
        self._kwargs = kwargs

        if batch_indexes is None:
            batch_indexes = list(label_paths.keys())
        self._batch_indexes = batch_indexes

    @classmethod
    def of_path(cls, path, batch_size, target_size, thread_count=None, **kwargs):
        label_paths = {
            _parse_index(x) : x for x in _list_files(os.path.join(path, 'labels'))
        }
        
        feature_paths = {
            i : [[] for x in range(batch_size)]
            for i in range(len(label_paths))
        }

        for feature_path in _list_files(os.path.join(path, 'features')):
            feature_index = _parse_index(feature_path)
            feature_paths[feature_index // batch_size][feature_index % batch_size].append(feature_path)

        return cls(label_paths, feature_paths, batch_size, target_size, thread_count, **kwargs)

    @property
    def batch_size(self):
        return self._batch_size

    @property
    def target_size(self):
        return self._target_size

    @property
    def batch_indexes(self):
        return self._batch_indexes.copy()

    def subset(self, iterable):
        label_paths = {
            i : self._label_paths[i] for i in iterable
        }

        feature_paths = {
            i : self._feature_paths[i] for i in iterable
        }

        return FSSequence(
            label_paths, feature_paths,
            self._batch_size, self._target_size, sorted(iterable),
            self._thread_count, **self._kwargs
        )
    
    def load_batch(self, idx):
        unedited_labels = _load_lines(self._label_paths[idx])
        feature_paths = self._feature_paths[idx]

        features = self.load_features(
            functools.reduce(operator.concat, feature_paths), self._target_size,
            thread_count=self._thread_count, **self._kwargs
        )

        labels = np.array(list(
            itertools.chain.from_iterable(
                (itertools.repeat(unedited_labels[i], len(x)) for i, x in enumerate(feature_paths))
            )
        ))

        return features, labels

    def __getitem__(self, idx):
        return self.load_batch(self._batch_indexes[idx])

    def __len__(self):
        return len(self._label_paths)

    @staticmethod
    def load_feature(path, **kwargs):
        return img_to_array(
            load_img(path, **kwargs)
        )

    @staticmethod
    def load_features(paths, target_size, thread_count=None, **kwargs):
        features = np.empty((len(paths), *target_size), dtype='float32')
        lock = multiprocessing.Lock()

        def load(index, path):
            feature = FSSequence.load_feature(path, target_size=target_size, **kwargs)
            with lock:
                features[index, :, :, :] = feature

        with multiprocessing.pool.ThreadPool(thread_count) as pool:
            for i, _ in enumerate(pool.imap_unordered(lambda pair : load(pair[0], pair[1]), enumerate(paths)), 1):
                sys.stdout.write('\r{0:.2%}'.format(i / len(paths)))
            sys.stdout.write('\n')

        features /= 255
        return features
    
    def as_np(self, path):
        raise NotImplementedError 


if __name__ == '__main__':
    pass

