"""
    Reducers

    base : eoReduce.h

    __call__(pop,new_size) --> void

 * eoReduce: .reduce the new generation to the specified size
   At the moment, limited to truncation - with 2 different methods,
   one that sorts the whole population, and one that repeatidely kills
   the worst. Ideally, we should be able to choose at run-time!!!
"""
from pyparadiseo import config,utils

class _Truncate():
    """Sort and Truncate (keep best)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoTruncate"+config.TYPES[type])
        return class_()


class _RandomReduce():
    """Shuffle and truncate (keep random)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoRandomReduce"+config.TYPES[type])
        return class_()


class  _EPReduce():
    """EP truncation method (some global stochastic tournament +  sort)
    Softer selective pressure than pure truncate

    tournament_size=2
    """
    def __new__(cls,tournament_size=2,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEPReduce"+config.TYPES[type])
        return class_(tournament_size)


class _LinearTruncate():
    """a truncate class that does not sort, but repeatidely kills the worse.
To be used in SSGA-like replacements (e.g. see eoSSGAWorseReplacement)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoLinearReduce"+config.TYPES[type])
        return class_()


class _DetTournamentTruncate():
    """a truncate class based on a repeated deterministic (reverse!) tournament
To be used in SSGA-like replacements (e.g. see eoSSGADetTournamentReplacement)
    """
    def __new__(cls,tournament_size=2,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoDetTournamentTruncate"+config.TYPES[type])
        return class_(tournament_size)


class _StochTournamentTruncate():
    """a truncate class based on a repeated deterministic (reverse!) tournament
To be used in SSGA-like replacements (e.g. see eoSSGADetTournamentReplacement)

    t_rate \in [0.51,1.0]
    """
    def __new__(cls,t_rate,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoStochTournamentTruncate"+config.TYPES[type])
        return class_(t_rate)
