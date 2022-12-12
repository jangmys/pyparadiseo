"""
PyParadisEO initializers generate new solutions.

More precisely, they
1. take an existing solution
2. set the solution encoding
3. reset fitness and objective values

The :func:`random` initializer


Any function-like object that takes
"""
from pyparadiseo import config,utils
from pyparadiseo import bounds as bounds_mod

from ._core import eoInit

__all__=['initializer','empty','random','eoInit']

def initializer(init_fun,stype=None):
    """
    make initializer from function

    Parameters
    ==========
    init_fun : callable
        callable that takes no argument and returns a solution encoding

    Note
    ====
    the encoding returned from `init_fun` is supposed to be compatible with `stype`

    Example
    =======
    >>> s = solution.empty()
    >>> init = initializer.initializer(lambda : np.random.random(6))
    >>> s
    Solution((,,),None)
    >>> init(s)
    >>> s
    Solution((,,),[0.83325227 0.64503181 0.55650262 0.63618153 0.68509981 0.0778035 ])


    `init_fun` can be a class-method as well :

    >>> class problem():
    ...     def __init__(self,dim):
    ...             self.dim = dim
    ...     def sol_generator(self):
    ...             return np.random.random(self.dim)
    ...
    >>> init = initializer.initializer(problem(7).sol_generator)
    >>> init(s)
    >>> s
    Solution((,,),[0.06518429 0.59086712 0.45074121 0.76213368 0.50050522 0.00619574
     0.42788994])

    If your `init_fun` has some additional parameters, functools.partial might be useful

    >>> def foo(spam,more_spam):
    ...     return np.random.random(4)
    ...
    >>> import functools
    >>> init = initializer.initializer(functools.partial(foo,42,21))
    >>> init(s)
    >>> s
    Solution((,,),[0.74647363 0.7738315  0.07647693 0.08171133])
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        class_ = utils.get_class("pyeoInit")
        return class_(init_fun)
    if stype == 'bin' or stype == 'real':
        class_ = utils.get_class("pyeoInit"+config.TYPES[stype])
        return class_(init_fun)
    else:
        raise NotImplementedError("Not yet implemented")


def empty(stype=None):
    """
    Solution
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    pass


def random(size=0,bounds=None,stype=None,**kwargs):
    """
    Random initializer (for bin/real)

    Parameters
    ----------
    size : int
        solution length
    stype : str, optional
        solution type
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        raise TypeError("initializer.random : not availble for generic solution type")

    if stype == 'bin':
        class_ = utils.get_class("BinaryInit")
        return class_(size)

    if stype == 'real':
        class_ = utils.get_class("RealBoundedInit")
        if bounds is not None:
            if isinstance(bounds, bounds_mod.RealVectorBounds):
                return class_(bounds)
            else:
                return class_(bounds_mod.box(bounds[0],bounds[1]))
        else:
            return class_(bounds_mod.box(size,0,1))


    if stype == 'perm':
        class_ = utils.get_class("PermutationInit")
        if "startFrom" in kwargs.keys():
            return class_(size,kwargs["startFrom"])
        else:
            return class_(size)

    if stype == 'real-pso':
        class_ = utils.get_class("RealBoundedParticleInit")
        if bounds is not None:
            if isinstance(bounds, bounds_mod.RealVectorBounds):
                return class_(bounds)
            else:
                return class_(bounds_mod.box(bounds[0],bounds[1]))
        else:
            raise TypeError("need bounds for type 'realparticle'")
