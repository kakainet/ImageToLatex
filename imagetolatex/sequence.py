from abc import abstractmethod, ABC

import multiprocessing.pool
import os
import os.path
import re
import functools
import itertools
import operator
import collections
import logging

from keras.utils import to_categorical, Sequence
from keras.preprocessing.image import load_img, img_to_array
import numpy as np


def _parse_indexes(path):
    _, name = os.path.split(path)
    return (int(x.group()) for x in re.finditer(r'[0-9]+', name))


def _list_files(path):
    return [
        os.path.join(path, x)
        for x in filter(lambda x: not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]


def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]


class AbstractSequence(ABC, Sequence):

    def __init__(self, labels, feature_paths, feature_shape, batch_size, thread_count, **feature_kwargs):
        self._labels = labels
        self._feature_paths = feature_paths

        self._batch_size = batch_size

        self._feature_shape = feature_shape
        self._feature_kwargs = feature_kwargs

        if thread_count is None:
            thread_count = os.cpu_count()
        self._thread_count = thread_count

    @property
    def batch_size(self):
        return self._batch_size

    @batch_size.setter
    def batch_size(self, new_batch_size):
        self._batch_size = int(new_batch_size)

    def get_slice(self, index):
        return slice(index * self._batch_size, (index + 1) * self._batch_size)

    @abstractmethod
    def get_labels(self, index_slice):
        pass

    def load_feature(self, feature_path):
        return img_to_array(load_img(feature_path, target_size=self._feature_shape, **self._feature_kwargs))

    def get_features(self, index_slice):
        features = np.empty((self._batch_size, *self._feature_shape), dtype='float32')
        with multiprocessing.pool.ThreadPool(self._thread_count) as pool:
            for feature_index, feature in pool.imap_unordered(lambda x: (x[0], self.load_feature(x[1])),
                                                              enumerate(self._feature_paths[index_slice])):
                features[feature_index] = feature

        return features

    def __getitem__(self, index):
        index_slice = self.get_slice(index)
        return self.get_features(index_slice), self.get_labels(index_slice)

    def __len__(self):
        return int(np.ceil(len(self._feature_paths) / self._batch_size))

    def get_subset(self, iterable):  # TODO !!!
        slice_iterable = [self.get_slice(i) for i in iterable]
        feature_paths = [self._feature_paths[s] for s in slice_iterable]
        labels = [self.get_labels(s) for s in slice_iterable]
        return feature_paths, np.stack(labels)  # TODO check

    @abstractmethod
    def subset(self, iterable):
        pass


class FlatSequence(AbstractSequence):

    def get_labels(self, index_slice):
        return self._labels[index_slice]

    def subset(self, iterable):
        feature_paths, labels = self.get_subset(iterable)
        return FlatSequence(
            labels, feature_paths,
            self._feature_shape, self._batch_size,
            self._thread_count,
            **self._feature_kwargs
        )


class StackedSequence(AbstractSequence):

    def get_labels(self, index_slice):
        return self._labels[:][index_slice]

    def subset(self, iterable):
        feature_paths, labels = self.get_subset(iterable)
        return StackedSequence(
            labels, feature_paths,
            self._feature_shape, self._batch_size,
            self._thread_count,
            **self._feature_kwargs
        )


# len categ encoder to ile znakow, supported characters to glebokosc
# sciezka do calego datasetu
def load(input_path, category_encoder, supported_characters,
         feature_shape, batch_size, thread_count=None):
    ungrouped_feature_paths = _list_files(os.path.join(input_path, 'features'))
    unsorted_feature_paths = collections.defaultdict(list)

    for feature_path in ungrouped_feature_paths:  # dla danego img wszystkie jego wersje wrzuca do 1 listy, ale zapisuje patha do wersji do tego danego featchera i te numerki jego
        feature_index, *sub_feature_indexes = _parse_indexes(feature_path)
        unsorted_feature_paths[feature_index].append(((feature_index, *sub_feature_indexes), feature_path))

    feature_paths = [[] for _ in range(len(unsorted_feature_paths))]
    # chcemy by byly tutaj sflatowane nizej, wystarczy by było a1a2a4a3b2b1b3b4... itd
    for (feature_index, *_), feature_path in itertools.chain.from_iterable(unsorted_feature_paths.values()):
        feature_paths[feature_index].append(feature_path)

    import functools
    import operator

    feature_paths = functools.reduce(operator.concat, feature_paths)

    label_paths = _list_files(os.path.join(input_path, 'labels'))
    ungrouped_labels = [[] for _ in range(len(label_paths))]

    if thread_count is None:
        thread_count = os.cpu_count()

    with multiprocessing.pool.ThreadPool(thread_count) as pool:
        for path, lines in pool.imap_unordered(lambda x: (x, _load_lines(x)), label_paths):
            index = next(_parse_indexes(path))
            #
            #index = index - 1
            #
            for line in lines:
                ungrouped_labels[index].append(
                    to_categorical([
                        category_encoder.encode(x)
                        for x in line.split('\t')
                    ], num_classes=len(category_encoder))
                )
    #TODO: ungrouped_labels są ok
    #TODO: iter daje snd element (z idx == 1)
    #grouped_labels = itertools.chain.from_iterable(
    #    itertools.repeat(x, len(unsorted_feature_paths[i]))
    #    for i, x in enumerate(itertools.chain.from_iterable(ungrouped_labels))
    #)
    grouped_labels = []
    for iff, f in enumerate(ungrouped_labels):
        for ix, x in enumerate(f):
            i = iff * len(ungrouped_labels) + ix
            for _ in range(len(unsorted_feature_paths[i])):
                grouped_labels.append(x)

    stacked_labels = np.zeros(
        (len(ungrouped_feature_paths), supported_characters, len(category_encoder)),
        dtype='float32'
    )
    #^ Czy po tym jest ok? to robi same zera wiec tak.

    for label_index, label_parts in enumerate(grouped_labels):
        stacked_labels[label_index][:len(label_parts)][:] = label_parts
#juz przed reshape zdycha
    stacked_labels = np.reshape(stacked_labels,
                                (supported_characters, len(ungrouped_feature_paths), len(category_encoder))
                                )

    return stacked_labels, feature_paths


def load_flat(input_path, category_encoder, supported_characters,
              feature_shape, batch_size, thread_count=None, **feature_kwargs):

    stacked_labels, feature_paths = load(input_path, category_encoder, supported_characters,
                                         feature_shape, batch_size, thread_count)
    x=42
    return [
        FlatSequence(
            labels, feature_paths, feature_shape, batch_size,
            thread_count=thread_count,
            **feature_kwargs
        )
        for labels in stacked_labels
    ]


def load_stacked(input_path, category_encoder, supported_characters,
                 feature_shape, batch_size, thread_count=None, **feature_kwargs):
    stacked_labels, feature_paths = load(input_path, category_encoder, supported_characters,
                                         feature_shape, batch_size, thread_count)

    return StackedSequence(
        stacked_labels, feature_paths, feature_shape, batch_size,
        thread_count=thread_count,
        **feature_kwargs
    )


# TODO change names
# TODO refactor subset


if __name__ == '__main__':
    pass
