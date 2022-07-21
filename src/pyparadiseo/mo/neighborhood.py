"""
A neighborhood
"""
from pyparadiseo import utils,config




# from .._core import moOrderNeighborhood as OrderNeighborhood
# from .._core import moDummyNeighborhood as DummyNeighborhood
#
# from .._core import moRndWithoutReplNeighborhood as RndWithoutReplNeighborhood
# from .._core import moRndWithReplNeighborhood as RndWithReplNeighborhood


class _Neighborhood():
    """
    A Neighborhood (abstract base class)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("moNeighborhood"+config.TYPES[type])
        return class_()


def indexed(neighborhood_size,stype=None):
    """
    A neighborhood based on indices

    Parameters
    ----------
    neighborhood_size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moIndexNeighborhood"+config.TYPES[stype])
    return class_(neighborhood_size)


def ordered(neighborhood_size,stype=None):
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


def random(neighborhood_size,max_neighbors=0,with_replacement=True,stype=None):
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
