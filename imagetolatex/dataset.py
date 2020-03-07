import collections
import os.path
import re

import numpy as np

from .sequence import LayeredSequence


def _list_files(path):
    return list(filter(
        os.path.isfile, map(
            lambda x : os.path.join(path, x), filter(
                lambda x : not x.startswith('.'), os.listdir(path)
            )
        )
    ))

def _load_lines(path):
    with open(path, 'r') as ifs:
        return [x.strip() for x in ifs.readlines()]

def _split_line(line, length):
    # TODO reconsider, maybe could be solved in loop range
    parts = line.split('\t')
    return parts[:length] if len(parts) > length else parts + [None] * (length - len(parts))

def load_layered(input_path, category_encoder, layer_count,
                 feature_shape=(32, 32, 3), batch_size=100, thread_count=None, **feature_kwargs):

    ungrouped_feature_paths = _list_files(os.path.join(input_path, 'features'))
    grouped_feature_paths = collections.defaultdict(list)

    for feature_path in ungrouped_feature_paths:
        feature_index, *_ = re.findall(r'[0-9]+', feature_path)
        grouped_feature_paths[int(feature_index)].append(feature_path)

    feature_paths = []

    for feature_index in range(len(grouped_feature_paths)):
        feature_paths.extend(grouped_feature_paths[feature_index])

    unparsed_labels = _load_lines(os.path.join(input_path, 'labels.txt'))

    assert len(unparsed_labels) == len(grouped_feature_paths)

    label_layers = np.empty(
        (layer_count, len(ungrouped_feature_paths), len(category_encoder)),
        dtype='float32'
    )

    for label_line_index, label_line in enumerate(unparsed_labels):
        for label_layer_index, label_part in enumerate(_split_line(label_line, layer_count)):
            encoded_label_part = category_encoder.encode(label_part)

            for feature_variant_index in range(len(grouped_feature_paths[label_line_index])):
                # TODO fix, may break on uneven variant counts
                label_index = label_line_index * len(grouped_feature_paths[label_line_index]) + feature_variant_index
                label_layers[label_layer_index, label_index, :] = encoded_label_part

    return LayeredSequence(
        feature_paths, label_layers,
        feature_shape, (len(category_encoder), ),
        batch_size, thread_count,
        **feature_kwargs
    )


if __name__ == '__main__':
    pass

