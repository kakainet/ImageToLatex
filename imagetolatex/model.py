from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
import keras.losses
import keras.optimizers

import numpy as np


class LayeredModel:
    def __init__(self, input_shape, layer_count, category_encoder, layer_init):
        self._layer_models = [
            layer_init(input_shape, len(category_encoder))
            for _ in range(layer_count)
        ]

    def fit_on_layered(self, train_sequences, test_sequences, epochs):
        for _ in range(epochs):
            train_index, test_index = np.random.randint(len(train_sequences)), np.random.randint(len(test_sequences))
            train_features, train_label_layers = train_sequences[train_index]
            test_features, test_label_layers = test_sequences[train_index]

            for (train_labels, test_labels), layer_model in zip(zip(train_label_layers, test_label_layers), self._layer_models):
                layer_model.train_on_batch(x=train_features, y=train_labels)
                layer_model.test_on_batch(x=test_features, y=test_labels)

    def _fit_on_layered(self, train_sequences, test_sequences, save=None, epochs=10, verbose=True, **kwargs):
        train_sequences = [_Flatten(train_sequences, i) for i in range(train_sequences.layer_count)]
        test_sequences = [_Flatten(test_sequences, i) for i in range(test_sequences.layer_count)]

        for layer_index, layer_model in enumerate(self._layer_models):
            print('Fitting {0}/{1} layer now'.format(layer_index + 1, len(self._layer_models)))
            layer_model.fit_generator(
                generator=train_sequences[layer_index],
                validation_data=test_sequences[layer_index],
                epochs=epochs,
                verbose=verbose,
                **kwargs
            )

        if not save:
            pass

        for layer_index, layer_model in enumerate(self._layer_models):
            layer_model.save(save.format(layer_index))


from keras.utils import Sequence

class _Flatten(Sequence):

    def __init__(self, sequence, layer_index):
        self._sequence = sequence
        self._layer_index = layer_index

    def __getitem__(self, index):
        x, y = self._sequence[index]
        return x, y[self._layer_index]

    def __len__(self):
        return len(self._sequence)


def complex_equation_layer(input_shape, num_classes, verbose=False):
    model = Sequential()

    model.add(Conv2D(64, kernel_size=(5, 5), activation='relu', input_shape=input_shape))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(128, (5, 5), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Conv2D(256, (5, 5), activation='relu'))
    model.add(MaxPooling2D(pool_size=(2, 2)))
    model.add(Dropout(0.25))
    model.add(Flatten())
    model.add(Dense(512, activation='relu'))
    model.add(Dropout(0.5))
    model.add(Dense(num_classes, activation='softmax'))

    model.compile(loss=keras.losses.categorical_crossentropy,
                  optimizer=keras.optimizers.Adam(),
                  metrics=['accuracy'])

    if verbose:
        model.summary()

    return model


if __name__ == '__main__':
    from string import digits

    from encoding import CategoryEncoder
    from dataset import load_layered

    latex_encoder = CategoryEncoder()
    latex_encoder.extend([
        '+', '-', '/', '^', '{', '}',
        R'\frac', R'\cdot', 'a', 'x',
        *digits
    ])

    input_shape = (64, 64, 1)
    layer_count = 5

    sequence = load_layered(
        './dataset/',
        latex_encoder,
        layer_count,
        input_shape,
        batch_size=50,
        color_mode='grayscale'
    )

    train_sequence, test_sequence = sequence.split(0.7)

    model = LayeredModel(input_shape, layer_count, latex_encoder, complex_equation_layer)
    model._fit_on_layered(
        train_sequence,
        test_sequence,
        save='pretrained/itl{0}.h5',
        epochs=5,
        verbose=True
    )
