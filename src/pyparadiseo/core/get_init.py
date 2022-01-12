from ._core import pyeoInit

import numpy as np

def get_init(solution_encoding,len=None):
    """
    factory function for solution initializer
    """
    if solution_encoding == 'binary':
        #expect len !
        return pyeoInit(lambda : np.random.randint(0,2,len))
    elif solution_encoding == 'integer':
        #expect upper,lower
        pass
    elif solution_encoding == 'permutation':
        #expect len !
        pass
    else:
        pass
