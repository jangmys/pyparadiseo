from pyparadiseo import config




from ._core import Solution
from ._core import RealSolution
from ._core import BinarySolution

SOLUTIONS={
    'gen' : Solution,
    'bin' : BinarySolution,
    'real': RealSolution
}

def empty(stype=None):
    """
    create an empty solution. encoding is None
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    return SOLUTIONS[stype]()


def zeros(size=0,stype=None):
    """
    create a fixed size solution ('bin' or 'real')

    Notes
    -----
    if type is 'gen' encoding is set to None
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    if stype=='gen':
        return klass()

    return klass(size)


def from_object(obj,stype=None):
    """
    create a solution from an object

    # TODO: for bin/real, accept ndarray or list (for now, return zeros of length len(obj))
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    #if obj is a Solution call copy ctor
    if isinstance(obj,klass):
        return klass(obj)

    #else make from python object
    if stype=='gen':
        klass=SOLUTIONS[stype]
        return klass(obj)
    else:
        return zeros(len(obj),stype)


def from_init(initializer,stype=None) :
    """
    create solution from initializer
    """
    from pyparadiseo import _core

    if isinstance(initializer,_core.eoInit):
        print("got init")


    #### try to get SolutionType from initializer-type?
    # pass
