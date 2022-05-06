"""
    Replacement

    base : eoReplacement.h

    __call__(parents,offspring) --> void

    eoMergeReduce.h (Replacement strategies that combine en eoMerge and an eoReduce)


"""

from pyparadiseo import config,utils

from .._core import eoReplacement as Replacement

# from .._core import eoWeakElitistReplacement as WeakElitistReplacement

from .._core import eoMergeReduce as MergeReduce
from .._core import eoPlusReplacement as PlusReplacement
from .._core import eoCommaReplacement as CommaReplacement
from .._core import eoEPReplacement as EPReplacement

from .._core import eoReduceMerge as ReduceMerge
from .._core import eoSSGAWorseReplacement as SSGAWorseReplacement
from .._core import eoSSGADetTournamentReplacement as SSGADetTournamentReplacement
from .._core import eoSSGAStochTournamentReplacement as SSGAStochTournamentReplacement
from .._core import eoMGGReplacement as MGGReplacement


class _GenerationalReplacement():
    """generational replacement

    swap populations"""
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoGenerationalReplacement"+config.TYPES[type])

        return class_()


class _WeakElitistReplacement():
    """a wrapper for other replacement procedures.
Copies in the new pop the best individual from the old pop,
AFTER normal replacement, if the best of the new pop is worse than the best
of the old pop. Removes the worse individual from the new pop.
This could be changed by adding a selector there...

    Parameters
    ----------
    replacement
    """
    def __new__(cls,replacement,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoWeakElitistReplacement"+config.TYPES[type])

        return class_(replacement)


class _MergeReduce():
    """eoMergeReduce: abstract replacement strategy that is just an application of
    an embedded merge, followed by an embedded reduce

    merge(parents,offspring)
    reduce(offspring,parents.size())
    swap(parents,offspring)
    """
    def __new__(cls,merge,reduce,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoWeakElitistReplacement"+config.TYPES[type])

        return class_(merge,reduce)


class _PlusReplace():
    """ES type of replacement strategy: first add parents to population, then truncate"""
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoPlusReplacement"+config.TYPES[type])

        return class_()


class _CommaReplace():
    """ES type of replacement strategy: ignore parents, truncate offspring"""
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoCommaReplacement"+config.TYPES[type])

        return class_()


class _EPReplacement():
    """EP type of replacement strategy: first add parents to population,
       then truncate using EP tournament"""
    def __new__(cls,t_size,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEPReplacement"+config.TYPES[type])

        return class_(t_size)
