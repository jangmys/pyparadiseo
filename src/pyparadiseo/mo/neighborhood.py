"""
MO neighborhoods
"""
from pyparadiseo import utils,config

#ABC
from .._core import moNeighborhood
from .._core import moIndexNeighborhood

__all__=['moNeighborhood','moIndexNeighborhood','neighborhood','ordered','random']


def neighborhood(init,next,cont,has_neighbor,is_random,stype=None):
    """
    A user-defined Neighborhood

    Parameters
    ==========
    init : Callable (solution,neighbor)->None
        initialize first neighbor
    next : Callable (solution,neighbor)->None
        set neighbor to next
    cont : Callable (solution)->Bool
        continue ?
    has_neighbor : Callable (solution)->Bool
        solution has neighbor?
    is_random : bool
        True if neighborhood is random
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("pyNeighborhood"+config.TYPES[stype])

    return class_(init,next,cont,has_neighbor,is_random)


def ordered(neighborhood_size: int,stype=None):
    """
    An ordered indexed neighborhood

    a special sort of IndexNeighborhood

    Parameters
    ----------
    neighborhood_size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moOrderNeighborhood"+config.TYPES[stype])
    return class_(neighborhood_size)


def random(neighborhood_size: int,max_neighbors: int=0,with_replacement=True,stype=None):
    """
    A random neighborhood

    a special sort of IndexNeighborhood

    Parameters
    ----------
    neighborhood_size - int, size of neighborhood
    max_neighors - int, max nb visited nbors (0 represents infinity)
    with_replacement = bool
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_=None
    if with_replacement:
        class_ = utils.get_class("moRndWithReplNeighborhood"+config.TYPES[stype])
        return class_(neighborhood_size,max_neighbors)
    else:
        class_ = utils.get_class("moRndWithoutReplNeighborhood"+config.TYPES[stype])
        return class_(neighborhood_size)
