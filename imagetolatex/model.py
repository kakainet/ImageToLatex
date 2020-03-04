import os
import os.path
import logging
from datetime import datetime

from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import to_categorical
import keras.losses
import keras.optimizers


class LayeredModel:

    def __init__(self, input_shape, category_encoder, supported_characters):

        self.get_layer_model = self.get_complex_equation_model

        self._layer_models = [
            self.get_layer_model(input_shape, len(category_encoder))
            for _ in range(supported_characters)
        ]

    def fit_on_flat(self, train_sequences, test_sequences, epochs, verbose, save_weights=False):
        print(self._layer_models)
        for layer_index, layer_model in enumerate(self._layer_models):
            print('Fitting {0}/{1} layer now'.format(layer_index + 1, len(self._layer_models)))
            layer_model.fit_generator(
                generator=train_sequences[layer_index],
                validation_data=test_sequences[layer_index],
                epochs=epochs,
                verbose=verbose,
                workers=12
            )

        if save_weights:
            save_path = 'params/weight_{0}_{1}.ckpt'
            for layer_index, layer_model in enumerate(self._layer_models):
                layer_model.save_weights(save_path.format(layer_index, datetime.now().strftime('%Y-%m-%d-%H:%M:%S')))

    @staticmethod
    def get_complex_equation_model(input_shape, num_classes):
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

        model.summary()

        return model


if __name__ == '__main__':
    print('test')
    from keras import backend as K

    K.tensorflow_backend._get_available_gpus()
    print('test')
    from encoding import CategoryEncoder
    from sequence import load_flat
    from string import digits

    print(*digits)
    latex_encoder = CategoryEncoder()
    latex_encoder.extend([
        '+', '-', '^', '{', '}',
        '^', R'\cdot', 'a', 'x',
        *digits
    ])

    sequences = load_flat(
        os.path.join(os.getcwd(), 'dataset'),
        latex_encoder,
        len(latex_encoder),
        (64, 64, 1),
        50,
        color_mode='grayscale'
    )
    print('seq size', len(sequences))
    train_size = int(0.8 * len(sequences))
    model = LayeredModel((64, 64, 1), latex_encoder, 7)
    model.fit_on_flat(
        sequences,
        sequences, # validation set will be extracted in 0.1.1
        epochs=10,
        verbose=1,
        save_weights=True
    )
