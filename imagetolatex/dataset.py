import collections
import os.path
import re

from keras.utils import to_categorical
import numpy as np

from .sequence import LayeredSequence

def _list_files(path):
    return [
        os.path.join(path, x)
        for x in filter(lambda x: not x.startswith('.'), os.listdir(path))
        if os.path.isfile(os.path.join(path, x))
    ]

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]

def load_layered(input_path, category_encoder, layer_count,
                 feature_shape=(32, 32, 1), batch_size=1024, thread_count=None, **feature_kwargs):

    ungrouped_feature_paths = _list_files(os.path.join(input_path, 'features'))
    grouped_feature_paths = collections.defaultdict(list)

    for feature_path in ungrouped_feature_paths:
        feature_index, *_ = re.findall(r'[0-9]+', feature_path)
        grouped_feature_paths[feature_index].append(feature_path)

    feature_paths = []

    for feature_index in range(len(ungrouped_feature_paths)):
        feature_paths.extend(grouped_feature_paths[feature_index])

    unparsed_labels = _load_lines(os.path.join(input_path, 'labels.txt'))
    label_layers = np.zeros(
        (len(ungrouped_feature_paths), layer_count, len(category_encoder)),
        dtype='float32'
    )

    for label_line_index, label_line in enumerate(unparsed_labels):
        for label_part_index, label_part in enumerate(label_line.split('\t')):
            encoded_label_part = to_categorical(category_encoder.encode(label_part), num_classes=len(category_encoder))

            for feature_variant_index in range(len(grouped_feature_paths[label_line_index])):
                label_layer_index = label_line_index * len(unparsed_labels) + feature_variant_index

                label_layers[label_layer_index][label_part_index][:] = encoded_label_part

    return LayeredSequence(
        feature_paths, label_layers,
        feature_shape, (layer_count, len(category_encoder)),
        batch_size, thread_count,
        **feature_kwargs
    )

"""
def load(input_path, category_encoder, supported_characters, thread_count=None):
    ungrouped_feature_paths = _list_files(os.path.join(input_path, 'features'))
    unsorted_feature_paths = collections.defaultdict(list)

    # for given image all its versions are pushed to one list.
    # There are also saved path to its feature versions and also ids
    for feature_path in ungrouped_feature_paths:
        feature_index, *sub_feature_indexes = _parse_indexes(feature_path)
        unsorted_feature_paths[feature_index].append(((feature_index, *sub_feature_indexes), feature_path))

    feature_paths = [[] for _ in range(len(unsorted_feature_paths))]

    # Transforming to flatted version i.e. a1a2a4a3b2b1b3b4
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

            for line in lines:
                ungrouped_labels[index].append(
                    to_categorical([
                        category_encoder.encode(x)
                        for x in line.split('\t')
                    ], num_classes=len(category_encoder))
                )

    grouped_labels = itertools.chain.from_iterable(
        itertools.repeat(x, len(unsorted_feature_paths[i]))
        for i, x in enumerate(itertools.chain.from_iterable(ungrouped_labels))
    )

    stacked_labels = np.zeros(
        (len(ungrouped_feature_paths), supported_characters, len(category_encoder)),
        dtype='float32'
    )

    # transforming stacked_labels into structure such that
    # stacked_labels[j] represents j'th label as a matrix of one hot encodings for each character
    # so stacked_labels[j][k] is j'th label one hot encoding for k'th character
    for label_index, label_parts in enumerate(grouped_labels):
        stacked_labels[label_index][:len(label_parts)][:] = label_parts

    # stacked[j][k] is j'th char in k'th label
    return stacked_labels, feature_paths


def load_flat(input_path, category_encoder, supported_characters,
              feature_shape, batch_size, thread_count=None, **feature_kwargs):

    stacked_labels, feature_paths = load(input_path, category_encoder, supported_characters,
                                         feature_shape, batch_size, thread_count)
    return [
        FlatSequence(
            stacked_labels[:, k, :], feature_paths, feature_shape, batch_size,
            thread_count=thread_count,
            **feature_kwargs
        )
        for k in range(supported_characters)
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
"""

if __name__ == '__main__':
    pass

