"""
Population
"""

from pyparadiseo import config,utils

from ._core import Pop
from ._core import BinaryPop
from ._core import RealPop
from ._core import IntPop

__all__=['empty','from_init','from_list','Pop']

### add Pop documentation!
# append
# __str__
# __len__
# sort
# shuffle
# __getitem__
# __setitem__
# best
# worst
# push_back
# resize
# swap


def empty(stype=None):
    """
    create an empty population of solution-type solutions

    Parameters
    ----------
    type : str
        Solution type

    Returns
    -------
    A population (eoPop)
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    class_ = None
    if stype == 'gen':
        class_ = utils.get_class("Pop")
    if stype == 'bin':
        class_ = utils.get_class("BinaryPop")
    if stype == 'real':
        class_ = utils.get_class("RealPop")
    if stype == 'perm':
        class_ = utils.get_class("IntPop")

    if class_ is None:
        raise TypeError("unknown stype")

    return class_()


def from_init(pop_size,f_init,stype=None):
    """
    create a population of pop_size solutions initialized by initializer

    Parameters
    ----------
    pop_size : int
        Population size
    f_init : eoInit
        An Initializer
    type : str
        Solution type

    Returns
    -------
    A population (eoPop)
    """
    if stype is None:
        stype=config._SOLUTION_TYPE

    #choose Pop according to f_init type
    class_ = None
    if f_init.__class__.__name__=='pyeoInit':
        class_ = utils.get_class("Pop")
    if f_init.__class__.__name__=='RealBoundedInit':
        class_ = utils.get_class("Pop")

    if f_init.__class__.__name__=='BinaryInit':
        class_ = utils.get_class("BinaryPop")
    if f_init.__class__.__name__=='RealBoundedInit':
        class_ = utils.get_class("RealPop")

    if class_ is None:
        raise TypeError("population.from_init : invalid init function")

    return class_(pop_size,f_init)


def from_list(pop_list,stype=None):
    """
    create population from python list of solutions
    """
    p=empty(stype)

    # p.resize(len(pop_list))

    for ind in pop_list:
        p.push_back(ind)

    return p
