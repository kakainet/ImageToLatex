import multiprocessing.pool
import os

from keras.utils import Sequence
from keras.preprocessing.image import load_img, img_to_array
import numpy as np


class LayeredSequence(Sequence):
    
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
        return self._label_layers[:, index_slice, :]

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
        label_layers = np.empty(
            (self.layer_count, self.batch_size * len(index_slices), *self.label_shape),
            dtype='float32'
        )

        for i, label_layer in enumerate(self._label_layers):
            for j, index_slice in enumerate(index_slices):
                label_layers[i, self._index_slice(j)] = label_layer[index_slice]

        return label_layers

    def subset(self, batch_indexes):
        index_slices = [self._index_slice(i) for i in sorted(batch_indexes)]
        feature_paths, label_layers = self._feature_subset(index_slices), self._label_subset(index_slices)

        return LayeredSequence(
            feature_paths, label_layers,
            self.feature_shape, self.label_shape,
            self.batch_size,
            self.thread_count,
            **self._feature_kwargs
        )

    def split(self, batch_percent):
        index = int(len(self) * batch_percent)
        return self.subset(range(0, index)), self.subset(range(index, len(self)))


if __name__ == '__main__':
    pass
