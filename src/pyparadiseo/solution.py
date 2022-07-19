from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import bounds


from ._core import Solution
from ._core import RealSolution
from ._core import BinarySolution
from ._core import IntSolution

SOLUTIONS={
    'gen' : Solution,
    'bin' : BinarySolution,
    'real': RealSolution,
    'perm': IntSolution
}


def solution(obj,stype=None):
    """
    create a solution from an object

    Parameters
    ==========
    obj : python object
        solution encoding
    stype : str (optional)
        solution type ('gen','bin','real','perm')

    Notes
    =====
    - if stype is bin/real/perm obj must be convertible to numpy ndarray
    - if obj is already a pyparadiseo solution, a copy is returned
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    #if obj is a Solution call copy ctor
    if isinstance(obj,klass):
        return klass(obj)

    #else make from python object
    if stype=='gen':
        return klass(obj)
    else:
        ret=klass(len(obj)) #zeros
        ret.array = obj
        return ret


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

    Parameters
    ==========
    size : int
        fixed size solution length

    Notes
    -----
    if type is 'gen', encoding is set to a list of None's
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    if stype=='gen':
        return klass([]*size)

    return klass(size)


def random(size=0,stype=None,**kwargs):
    """
    create and intialize fixed size solution ('bin' or 'real')

    Parameters
    ==========
    size : int, default : 0
        length of random solution
    stype : str
        solution type
    bounds : eoBounds
        bounds for 'real' solution

    Notes
    -----
    if type is 'gen', encoding is set to None
    """
    sol = empty(stype)

    if 'bounds' in kwargs:
        init = initializer.random(size,stype,bounds=kwargs["bounds"])
        init(sol)
    else:
        init = initializer.random(size,stype)
        init(sol)

    return sol


def from_init(initializer,stype=None) :
    """
    create solution from initializer
    """
    from pyparadiseo import _core

    #sanity check
    if not isinstance(initializer,_core.eoInit):
        print("need an eoInit")

    sol=empty(stype)
    initializer(sol)
    return sol


    #### try to get SolutionType from initializer-type?
    # pass
