"""
Solution Types
--------------
Provides factory methods to create solution objects to be used in PyParadiseo algorithms (EO/MO/MOEO)

Different solution types are provided:
    * Generic
    * Binary
    * Real
    * Permutation


"""
from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import bounds

from ._core import _PyEO
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

__all__=['Solution','RealSolution','intSolution','BinarySolution','solution','empty','zeros','random','from_init']


# Documenting C++ class (object creation is deferred to creation methods)
def add_solution_doc(sol):
    sol.__doc__ ="""
    Generic ``pyparadiseo`` Solution

    Attributes
    ==========
    fitness : float
    diversity : float
    objectives : :class:`objectiveVector`
    encoding : object
        solution encoding can be any python object

    See Also
    ========
    :ref:`solution_creation`

    Examples
    --------
    .. code-block:: python

        pass
    """

    sol.invalid.__doc__="""True if fitness is invalid"""
    sol.invalidate.__doc__="""invalidate fitness"""


# for sol in SOLUTIONS.values():
add_solution_doc(Solution)
add_solution_doc(RealSolution)

#############################################
#############################################
#############################################
#############################################
#############################################
def solution(obj,stype=None):
    """
    Create an initialized solution

    For `stype` ='gen', the solution encoding is set to the `obj`.
    For 'bin','rand' or 'perm' `obj` is converted to a ``numpy.array``.

    Parameters
    ==========
    obj : object
        solution encoding
    stype : {'gen','bin','real','perm'}, optional
        solution type

    Returns
    =======
    Solution
        solution object of type `stype`, initialized with `obj`

    Notes
    =====
    - if `stype` is 'bin'/'real'/'perm' `obj` must be ``array_like``
    - if `obj` is already a pyparadiseo solution, a copy is returned

    Example
    =======
    >>> from pyparadiseo import solution

    Make a generic solution (stype='gen' is default)

    >>> solution.solution([1,'A',[1.0,2.0]])
    Solution((,,),[1, 'A', [1.0, 2.0]])

    Make binary solution from list:

    >>> solution.solution([1,0]*3,stype='bin')
    Solution((,,),1 0 1 0 1 0 )

    Make real solution from tuple:

    >>> solution.solution((0,1)*3,stype='real')
    Solution((,,),0.0 1.0 0.0 1.0 0.0 1.0 )

    See also
    ========
    :func:`pyparadiseo.solution.empty`
    """
    #-----if obj is already a Solution, call copy ctor-----
    if issubclass(obj.__class__,_PyEO):
        klass = obj.__class__
        return klass(obj)

    #-----get solution type----------
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    #-----if obj is None, return default solution-----
    if obj is None:
        return klass()

    #-----else make from python object-----
    if stype=='gen':
        return klass(obj)
    else:
        #-----get object length-----
        try:
            ret=klass(len(obj))
        except TypeError:
            print("For stype='",stype,"' solution encoding must have a length. Consider using generic solution type.")
            raise

        #-----set ndarray to obj-----
        try:
            ret.encoding = obj
            # ret.array = obj
        except ValueError:
            print("For stype=",stype,"solution encoding ",obj," must be convertible to numpy array. Returning zeros.")
        return ret


def empty(stype=None):
    """
    Create an empty solution.

    For `stype` ='gen', encoding is set to ``None``.
    For array-like solutions, encoding is set to an array of length zero.

    Note
    ====
    identical to calling solution.solution(stype)

    Parameters
    ==========
    stype : {'gen','bin','real','perm'}, optional
        solution type

    Returns
    =======
    Solution
        solution object of type `stype`, empty-initialized

    Example
    =======
    >>> from pyparadiseo import solution

    >>> solution.empty()
    Solution((,,),None)

    >>> solution.empty('bin')
    Solution((,,),)
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    return SOLUTIONS[stype]()


def zeros(size=0,stype=None):
    """
    Create a fixed size array-like solution filled with zeros.

    Parameters
    ==========
    size : int, default=0
        fixed-size solution length
    stype : {'gen','bin','real','perm'}, optional
        solution type

    Returns
    =======
    Solution
        solution object of type `stype`, zero-initialized

    Notes
    =====
    if `stype` is 'gen', encoding is set to a list of ``None``'s
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    klass=SOLUTIONS[stype]

    if stype=='gen':
        return klass([]*size)

    return klass(size)


def random(size=0,lb=None,ub=None,stype=None,**kwargs):
    """
    Create and intialize fixed size solution ('bin' or 'real')

    Parameters
    ==========
    size : int, default=0
        length of random solution
    stype : {'gen','bin','real','perm'}, optional
        solution type

    Other Parameters
    ================
    bounds : :obj:`eoBounds`
        keyword argument `bounds` for 'real' solution

    Returns
    =======
    Solution
        solution object of type `stype`, random-initialized

    Notes
    =====
    if `stype` is 'gen', encoding is set to ``None``
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    sol = empty(stype)

    if stype == 'gen':
        return sol

    if stype == 'bin':
        init = initializer.random(size=size,stype=stype)
        init(sol)

    if stype == 'real':
        if len(lb)>size:
            size = len(lb)

        init = initializer.random(size,(lb,ub),stype)
        init(sol)

    return sol


def from_init(initializer,stype=None) :
    """
    Create solution from initializer

    Parameters
    ==========
    initializer : eoInit
        initializer
    stype : {'gen','bin','real','perm'}, optional
        solution type
    """
    from pyparadiseo import _core

    #sanity check
    if not isinstance(initializer,_core.eoInit):
        print("need an eoInit")

    sol=empty(stype)
    initializer(sol)
    return sol
