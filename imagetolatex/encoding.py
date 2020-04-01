from keras.utils import to_categorical
import numpy as np


class CategoryEncoder:

    def __init__(self):
        self.__category_to_expression = [None]
        self.__expression_to_category = {None : 0}

    def extend(self, expressions):
        for expression in expressions:
            if expression not in self.__expression_to_category:
                self.__category_to_expression.append(expression)
                self.__expression_to_category[expression] = len(self) - 1
        
    def encode(self, expression):
        try:
            if isinstance(expression, list):
                category = [self.__expression_to_category[x] for x in expression]
            else:
                category = self.__expression_to_category[expression]
            return to_categorical(category, num_classes=len(self))
        except KeyError:
            raise ValueError('Expression: %s is not supported' % (expression))
    
    def decode(self, category):
        if isinstance(category, np.ndarray):
            try:
                category = int(np.argmax(category))
            except TypeError:
                raise TypeError('Category is not convertible to int or a numpy.ndarray of int or float')

        try:
            return self.__category_to_expression[category]
        except (IndexError, TypeError):
            raise ValueError('Category: %s is not supported' % (category))

    def __len__(self):
        return len(self.__category_to_expression)


if __name__ == '__main__':
    pass
