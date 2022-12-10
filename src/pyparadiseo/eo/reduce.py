"""
    Reduce population

    base : eoReduce.h

    __call__(pop,new_size) --> void

     * eoReduce: .reduce the new generation to the specified size
       At the moment, limited to truncation - with 2 different methods,
       one that sorts the whole population, and one that repeatidely kills
       the worst. Ideally, we should be able to choose at run-time!!!
"""
from pyparadiseo import config,utils

from .._core import eoReduce

__all__ = ['truncate','random','ep_reduce','linear','det_tournament','stoch_tournament', 'eoReduce']


def truncate(stype=None):
    """Sort and Truncate (keep best)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoTruncate"+config.TYPES[stype])
    return class_()


def random(stype=None):
    """Shuffle and truncate (keep random)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoRandomReduce"+config.TYPES[stype])
    return class_()


def ep_reduce(tournament_size,stype=None):
    """EP truncation method (some global stochastic tournament +  sort)
    Softer selective pressure than pure truncate

    tournament_size=2
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEPReduce"+config.TYPES[stype])
    return class_(tournament_size)


def linear(stype=None):
    """a truncate class that does not sort, but repeatidely kills the worse.
To be used in SSGA-like replacements (e.g. see eoSSGAWorseReplacement)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoLinearTruncate"+config.TYPES[stype])
    return class_()


def det_tournament(tournament_size,stype=None):
    """a truncate class based on a repeated deterministic (reverse!) tournament
To be used in SSGA-like replacements (e.g. see eoSSGADetTournamentReplacement)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoDetTournamentTruncate"+config.TYPES[stype])
    return class_(tournament_size)


def stoch_tournament(t_rate,stype=None):
    """a truncate class based on a repeated deterministic (reverse!) tournament
To be used in SSGA-like replacements (e.g. see eoSSGADetTournamentReplacement)

    t_rate \in [0.51,1.0]
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoStochTournamentTruncate"+config.TYPES[stype])
    return class_(t_rate)
