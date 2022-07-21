"""
neighbor comparators
"""
from pyparadiseo import utils,config

# from .._core import moNeighborComparator
# from .._core import moSolNeighborComparator

def neighbor_compare(cmpfun,stype=None):
    """
    compare neighbor-neighbor
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moNeighborComparator"+config.TYPES[stype])

    return class_(cmpfun)

    #lt,gt,leq,geq


def sol_neighbor_compare(cmpfun,stype=None):
    """
    compare solution-neighbor
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moSolNeighborComparator"+config.TYPES[stype])

    return class_(cmpfun)
