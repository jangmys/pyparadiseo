"""
    Population replacement

    base : eoReplacement.h

    __call__(parents,offspring) --> void

    eoMergeReduce.h (Replacement strategies that combine en eoMerge and an eoReduce)


"""

from pyparadiseo import config,utils

from .._core import eoReplacement

__all__ = ['generational','weak_elitist','merge_reduce','plus','comma','ep_replacement','ssga_worse','ssga_stoch_tournament','ssga_det_tournament','eoReplacement']



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
    """ES type of replacement strategy: first add parents to population, then SORT and truncate

        resulting popululation is sorted
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoPlusReplacement"+config.TYPES[stype])
    return class_()


def comma(stype=None):
    """ES type of replacement strategy: ignore parents, truncate offspring

    offspring must be larger than parent pop (or same size)

    if same size : generational

    if offspring larger, sort+truncate
    """
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


def ssga_worse(stype=None):
    """
    ssga worse replacement : a reduce-merge
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSSGAWorseReplacement"+config.TYPES[stype])
    return class_()


def ssga_stoch_tournament(rate,stype=None):
    """SSGA stochastic tournament replacement. Is an eoReduceMerge.

    It much cleaner to insert directly the offspring in the parent population, but it is NOT equivalent in case of more than 1 offspring as already replaced could be removed , which is not possible in the eoReduceMerge

    So what the heck !
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSSGAStochTournamentReplacement"+config.TYPES[stype])
    return class_(rate)


def ssga_det_tournament(t_size,stype=None):
    """
    SSGA deterministic tournament replacement. Is an eoReduceMerge.
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSSGADetTournamentReplacement"+config.TYPES[stype])
    return class_(t_size)
