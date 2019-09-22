class Encoding:
    def __init__(self):
        self.init_bijection()

    def init_bijection(self):
        self.bijection = dict()
        self.bijection['null'] = 0
        for i in range(ord('a'), ord('z')+1):
            self.bijection[chr(i)] = i - ord('a') + 1
        length_stamp = len(self.bijection)
        for i in range(ord('0'), ord('9')+1):
            self.bijection[chr(i)] = length_stamp + i - ord('0')
        self.bijection['\frac'] = len(self.bijection)
        self.bijection['\cdot'] = len(self.bijection)
        self.bijection['{'] = len(self.bijection)
        self.bijection['}'] = len(self.bijection)
        self.bijection['('] = len(self.bijection)
        self.bijection[')'] = len(self.bijection)
        self.bijection['/'] = len(self.bijection)
        self.bijection['+'] = len(self.bijection)
        self.bijection['-'] = len(self.bijection)
