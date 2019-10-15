import resource
import logging


def memory_usage():
    return resource.getrusage(resource.RUSAGE_SELF).ru_maxrss / (1024 * 1024)

class Logger(logging.Logger):

    TRACE = 9

    logging.addLevelName(TRACE, 'TRACE')

    def __init__(self, name, level=logging.NOTSET):
        logging.Logger.__init__(self, name, level=level)
        
    def trace(self, message, *args, **kwargs):
        self.log(self.TRACE, message, *args, **kwargs)

