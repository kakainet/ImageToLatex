import os
import os.path
import logging

from keras.models import Sequential
from keras.layers import Dense, Dropout, Flatten
from keras.layers import Conv2D, MaxPooling2D
from keras.utils import to_categorical
import keras.losses
import keras.optimizers


class LayeredModel:
    
    def __init__(self, input_shape, category_encoder, supported_characters):

        self.get_layer_model = self.get_first_character_model

        self._layer_models = [
            self.get_layer_model(input_shape, len(category_encoder))
            #for _ in range(supported_characters)
            for _ in range(1)
        ]

    def fit_on_flat(self, train_sequences, test_sequences, epochs, verbose):
        #from utils import memory_usage

        #print(memory_usage())
        #x, y = train_sequences[0][0]
        #print(memory_usage())
        
        #print(memory_usage())
        #self._layer_models[0].train_on_batch(x, y)
        #print(memory_usage())

        #exit(0)
        print(self._layer_models)
        for layer_index, layer_model in enumerate(self._layer_models):
            layer_model.fit_generator(
                generator=train_sequences[layer_index],
                validation_data=test_sequences[layer_index],
                epochs=epochs,
                verbose=verbose,
                #workers=0
            )

    @staticmethod
    def get_complex_equation_model(input_shape, num_classes):
        return NotImplementedError()
        model = Sequential()
    
        model.add(Conv2D(64, kernel_size=(5, 5), activation='relu', input_shape=input_shape))
        model.add(MaxPooling2D(pool_size=(2, 2)))
        model.add(Conv2D(128, (5, 5), activation='relu'))
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

    @staticmethod
    def get_first_character_model(input_shape, num_classes):
        model = Sequential()

        model.add(Conv2D(36, kernel_size=(5, 5), activation='relu', input_shape=input_shape))
        model.add(MaxPooling2D(pool_size=(2, 2)))
        model.add(Conv2D(64, (5, 5), activation='relu'))
        model.add(MaxPooling2D(pool_size=(2, 2)))
        model.add(Dropout(0.25))
        model.add(Flatten())
        model.add(Dense(64, activation='relu'))
        model.add(Dropout(0.5))
        model.add(Dense(num_classes, activation='softmax'))

        model.compile(loss=keras.losses.categorical_crossentropy,
                    optimizer=keras.optimizers.Adam(),
                    metrics=['accuracy'])

        model.summary()

        return model

if __name__ == '__main__':
    #pierwszy wychodzi na 5 3 - \frac { 9 4 } { 9 6 }
    #czyli skipnelismy jedno
    #pierwsze 8 ma 5 3 - \frac { 9 4 } { 9 6 } wiec jakby skipniete o 1
    #nastepne 8 ma 2 8 - \frac { 2 4 } { 6 8 }
    #czyli to jest trzeci
    #wyglada na przesuniete o jeden
    from encoding import CategoryEncoder
    from sequence import load_flat
    from string import digits
    print(*digits)
    latex_encoder = CategoryEncoder()
    latex_encoder.extend([
        '+', '-', '/', '^', '{', '}',
        R'\frac', R'\cdot', 'a', 'x',
        *digits
    ])
    #0 ---> 11

    sequences = load_flat(
        os.path.join(os.getcwd(), 'dataset'),
        latex_encoder,
        len(latex_encoder),
        (128, 128, 1),
        50,
        color_mode='grayscale'
    )
    print('seq size', len(sequences))
    #coś jest z tym skopane, że ten size to 20
    model = LayeredModel((128, 128, 1), latex_encoder, 1)
    model.fit_on_flat(
        sequences,
        sequences,
        epochs=5,
        verbose=1
    )


