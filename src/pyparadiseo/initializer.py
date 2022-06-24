"""
generate solutions

initializers generate new solutions
"""

from pyparadiseo import config,utils

from ._core import eoInit
##TODO : document base class

from ._core import pyeoInit as Init
from ._core import BinaryInit

# from ._core import BinarySolInit
# from ._core import RealBoundedInit


def random(size=0,stype=None,**kwargs):
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
        class_ = utils.get_class("RealBoundedInitReal")
        if "bounds" in kwargs.keys():
            return class_(kwargs["bounds"])
        else:
            raise TypeError("need bounds for type 'real'")

    if stype == 'perm':
        class_ = utils.get_class("PermutationInit")
        if "startFrom" in kwargs.keys():
            return class_(size,kwargs["startFrom"])
        else:
            return class_(size)


def make_initializer(init_fun,stype=None):
    """
    make initializer from function
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        class_ = utils.get_class("pyeoInit")
        return class_(init_fun)
    else:
        NotImplementedError("Not yet implemented")
