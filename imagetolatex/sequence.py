import multiprocessing.pool
import os

from keras.preprocessing.image import load_img, img_to_array
import numpy as np


class LayeredSequence:
    
    def __init__(self, feature_paths, label_layers,
                 feature_shape, label_shape,
                 batch_size, thread_count,
                 **feature_kwargs):

        self._feature_paths = feature_paths
        self._label_layers = label_layers
        
        self._feature_shape = feature_shape
        self._label_shape = label_shape
        
        self._batch_size = batch_size
        
        if thread_count is None:
            thread_count = os.cpu_count()
        self._thread_count = thread_count
        
        self._feature_kwargs = feature_kwargs
    
    @property
    def feature_shape(self):
        return self._feature_shape
    
    @feature_shape.setter
    def feature_shape(self, new_feature_shape):
        self._feature_shape = new_feature_shape
    
    @property
    def label_shape(self):
        return self._label_shape
    
    @property
    def batch_size(self):
        return self._batch_size
    
    @batch_size.setter
    def batch_size(self, new_batch_size):
        self._batch_size = new_batch_size
    
    @property
    def data_size(self):
        return len(self._feature_paths)

    @property
    def layer_count(self):
        return len(self._label_layers)

    @property
    def thread_count(self):
        return self._thread_count
    
    @thread_count.setter
    def thread_count(self, new_thread_count):
        self._thread_count = new_thread_count
    
    def __getitem__(self, index):
        return self.load_batch(index)
    
    def __len__(self):
        return int(np.ceil(self.data_size / self.batch_size))

    def load_feature(self, feature_path):
        return img_to_array(load_img(feature_path, target_size=self.feature_shape, **self._feature_kwargs))

    def _index_slice(self, index):
        return slice(index * self.batch_size, (index + 1) * self.batch_size)

    def _feature_slice(self, index_slice):
        features = np.empty((self.batch_size, *self.feature_shape), dtype='float32')

        with multiprocessing.pool.ThreadPool(self.thread_count) as pool:
            for i, feature in pool.imap_unordered(lambda x: (x[0], self.load_feature(x[1])),
                                                  enumerate(self._feature_paths[index_slice])):
                features[i] = feature

        return features

    def _label_slice(self, index_slice):
        return [label_layer[index_slice] for label_layer in self._label_layers]

    def load_batch(self, index):
        index_slice = self._index_slice(index)

        features, label_layers = self._feature_slice(index_slice), self._label_slice(index_slice)

        return features, label_layers

    def _feature_subset(self, index_slices):
        feature_paths = []

        for index_slice in index_slices:
            feature_paths.extend(self._feature_paths[index_slice])

        return feature_paths

    def _label_subset(self, index_slices):
        label_layers = [
            np.empty((self.batch_size * len(index_slices), *self.label_shape), dtype='float32')
            for _ in range(self.layer_count)
        ]

        for i, label_layer in enumerate(self._label_layers):
            for j, index_slice in enumerate(index_slices):
                label_layers[i][self._index_slice(j)] = label_layer[index_slice]

        return label_layers

    def subset(self, batch_indexes):
        index_slices = [self._index_slice(i) for i in batch_indexes]
        feature_paths, label_layers = self._feature_subset(index_slices), self._label_subset(index_slices)

        return LayeredSequence(
            feature_paths, label_layers,
            self.feature_shape, self.label_shape,
            self.batch_size,
            self.thread_count,
            **self._feature_kwargs
        )
    
"""
class AbstractSequence(ABC):

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
    def feature_count(self):
        return len(self._feature_paths)

    @property
    def feature_shape(self):
        return self._feature_shape

    @feature_shape.setter
    def feature_shape(self, new_feature_shape):
        if not isinstance(new_feature_shape, tuple):
            raise TypeError('feature_shape must a tuple')

        if len(new_feature_shape) != 3:
            raise ValueError('Length of feature_shape must be 3')

        self._feature_shape = new_feature_shape

    @property
    def thread_count(self):
        return self._thread_count

    @thread_count.setter
    def thread_count(self, new_thread_count):
        self._thread_count = int(new_thread_count)

    @property
    def batch_size(self):
        return self._batch_size

    @batch_size.setter
    def batch_size(self, new_batch_size):
        self._batch_size = int(new_batch_size)

        if new_batch_size > self.feature_count:
            raise ValueError('batch_size cannot be greater than feature_count')

        self._batch_size = new_batch_size

    def _load_feature(self, feature_path):
        return img_to_array(load_img(feature_path, target_size=self.feature_shape, **self._feature_kwargs))

    def _get_index_slice(self, index):
        return slice(index * self.batch_size, (index + 1) * self.batch_size)

    @abstractmethod
    def _get_labels_slice(self, index_slice):
        pass

    def _get_features_slice(self, index_slice):
        features = np.empty((self.batch_size, *self.feature_shape), dtype='float32')

        with multiprocessing.pool.ThreadPool(self.thread_count) as pool:
            for feature_index, feature in pool.imap_unordered(lambda x: (x[0], self._load_feature(x[1])),
                                                              enumerate(self._feature_paths[index_slice])):
                features[feature_index] = feature

        return features

    def __getitem__(self, index):
        index_slice = self._get_index_slice(index)
        return self._get_features_slice(index_slice), self._get_labels_slice(index_slice)

    def __len__(self):
        return int(np.ceil(self.feature_count / self.batch_size))

    @abstractmethod
    def _get_labels_subset(self, index_slices):
        pass

    def _get_feature_paths_subset(self, index_slices):
        feature_paths = []

        for index_slice in index_slices:
            feature_paths.extend(self._feature_paths[index_slice])

        return feature_paths

    def subset(self, iterable):
        return self._subset(self, iterable)

    @classmethod
    def _subset(cls, instance, iterable):
        index_slices = [instance._get_index_slice(index) for index in iterable]

        labels = instance._get_labels_subset(index_slices)
        feature_paths = instance._get_feature_paths_subset(index_slices)

        return cls(
            labels, feature_paths,
            instance.feature_shape, instance.batch_size,
            instance.thread_count,
            **instance._feature_kwargs
        )


class FlatSequence(AbstractSequence):

    def _get_labels_slice(self, index_slice):
        return self._labels[index_slice]

    def _get_labels_subset(self, index_slices):
        return np.stack([self._labels[index_slice] for index_slice in index_slices])


class StackedSequence(AbstractSequence):

    def _get_labels_slice(self, index_slice):
        return [labels[index_slice] for labels in self._labels]

    def _get_labels_subset(self, index_slices):
        return [
            np.stack([labels[index_slice] for index_slice in index_slices])
            for labels in self._labels
        ]
"""

if __name__ == '__main__':
    pass
