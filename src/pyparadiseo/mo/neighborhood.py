"""
A neighborhood
"""
from pyparadiseo import utils,config


##abstract
from .._core import Neighbor
from .._core import moNeighborhood


from .._core import moIndexNeighborhood

from .._core import moOrderNeighborhood as OrderNeighborhood
from .._core import moDummyNeighborhood as DummyNeighborhood

from .._core import moRndWithoutReplNeighborhood as RndWithoutReplNeighborhood
from .._core import moRndWithReplNeighborhood as RndWithReplNeighborhood


class _Neighborhood():
    """
    A Neighborhood (abstract base class)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("moNeighborhood"+config.TYPES[type])
        return class_()


class _IndexNeighborhood():
    """
    A neighborhood based on indices
    """
    def __new__(cls,neighborhood_size,type=None):
        """
        Parameters
        ----------
        neighborhood_size
        """
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("moIndexNeighborhood"+config.TYPES[type])
        return class_(neighborhood_size)


class _OrderNeighborhood():
    """
    An ordered indexed neighborhood

    a special sort of IndexNeighborhood
    """
    def __new__(cls,neighborhood_size,type=None):
        """
        Parameters
        ----------
        neighborhood_size
        """
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("moOrderedNeighborhood"+config.TYPES[type])
        return class_(neighborhood_size)


class _RandomNeighborhood():
    """
    An ordered indexed neighborhood

    a special sort of IndexNeighborhood
    """
    def __new__(cls,neighborhood_size,max_neighbors=0,with_replacement=True,type=None):
        """
        Parameters
        ----------
        neighborhood_size - int, size of neighborhood
        max_neighors - int, max nb visited nbors (0 represents infinity)
        with_replacement = bool
        """
        if type is None:
            type = config._SOLUTION_TYPE

        class_=None
        if with_replacement:
            class_ = utils.get_class("moRndWithReplNeighborhood"+config.TYPES[type])
        else:
            class_ = utils.get_class("moRndWithoutReplNeighborhood"+config.TYPES[type])

        return class_(neighborhood_size,max_neighbors)
