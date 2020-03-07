from keras.utils import to_categorical
import numpy as np

class CategoryEncoder:

    def __init__(self):
        self.__category_to_expression = [None]
        self.__expression_to_category = {0 : None}

    def extend(self, expressions):
        for expression in expressions:
            if expression not in self.__expression_to_category:
                self.__category_to_expression.append(expression)
                self.__expression_to_category[expression] = len(self) - 1
        
    def encode(self, expression):
        try:
            expression = str(expression)
        except TypeError:
            raise TypeError('Expression is not convertible to str')

        try:
            return to_categorical(self.__expression_to_category[expression], num_classes=len(self))
        except KeyError:
            raise ValueError('Expression: %s is not supported' % (expression))
    
    def decode(self, category):
        try:    
            if isinstance(category, np.ndarray):
                category = int(np.argmax(category))
            else:
                category = int(category)
        except TypeError:
            raise TypeError('Category is not convertible to int or a numpy.ndarray of int or float')

        try:
            return self.__category_to_expression[category]
        except IndexError:
            raise ValueError('Category: %s is not supported' % (category))

    def __len__(self):
        return len(self.__category_to_expression)

