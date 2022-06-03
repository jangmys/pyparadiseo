"""
    Replacement

    base : eoReplacement.h

    __call__(parents,offspring) --> void

    eoMergeReduce.h (Replacement strategies that combine en eoMerge and an eoReduce)


"""

from pyparadiseo import config,utils

from .._core import eoReplacement as Replacement

# from .._core import eoWeakElitistReplacement as WeakElitistReplacement

# from .._core import eoMergeReduce as MergeReduce
# from .._core import eoPlusReplacement as PlusReplacement
# from .._core import eoCommaReplacement as CommaReplacement
# from .._core import eoEPReplacement as EPReplacement

from .._core import eoReduceMerge as ReduceMerge
from .._core import eoSSGAWorseReplacement as SSGAWorseReplacement
from .._core import eoSSGADetTournamentReplacement as SSGADetTournamentReplacement
from .._core import eoSSGAStochTournamentReplacement as SSGAStochTournamentReplacement
from .._core import eoMGGReplacement as MGGReplacement


def generational(stype=None):
    """generational replacement

    swap populations"""
    if stype is None:
        stype = config._SOLUTION_TYPE

    return utils.get_class("eoGenerationalReplacement"+config.TYPES[stype])()


def weak_elitist(replacement,stype=None):
    """a wrapper for other replacement procedures.
Copies in the new pop the best individual from the old pop,
AFTER normal replacement, if the best of the new pop is worse than the best
of the old pop. Removes the worse individual from the new pop.
This could be changed by adding a selector there...

    Parameters
    ----------
    replacement
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoWeakElitistReplacement"+config.TYPES[stype])
    return class_(replacement)


def merge_reduce(merge,reduce,stype=None):
    """eoMergeReduce: abstract replacement strategy that is just an application of
    an embedded merge, followed by an embedded reduce

    merge(parents,offspring)
    reduce(offspring,parents.size())
    swap(parents,offspring)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoMergeReduce"+config.TYPES[stype])
    return class_(merge,reduce)


def plus(stype=None):
    """ES type of replacement strategy: first add parents to population, then truncate"""
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoPlusReplacement"+config.TYPES[stype])
    return class_()


def comma(stype=None):
    """ES type of replacement strategy: ignore parents, truncate offspring"""
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoCommaReplacement"+config.TYPES[stype])
    return class_()


def ep_replacement(t_size,stype=None):
    """EP stype of replacement strategy: first add parents to population,
       then truncate using EP tournament
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEPReplacement"+config.TYPES[stype])
    return class_(t_size)
