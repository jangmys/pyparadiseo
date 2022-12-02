"""
Select an individual from popululation

Individual-selectors are functors derived from the  :py:class:`eoSelectOne` base class.
Those functors take a population as input and return an individual.



eoSelectOne.h
    __call__(src_pop) --> Solution

 eoSelectOne selects only one element from a whole population.
    Most selection techniques are simply repeated applications
    of eoSelectOne.

"""
from pyparadiseo import config,utils

from .._core import eoSelectOne
eoSelectOne.__doc__="this is the doc of SelectOne"

__all__ = ['select_one','det_tournament','stoch_tournament','truncated','proportional','random','best','round_robin','sequential','elite_sequential','eoSelectOne']


def select_one(klass_or_fun=None,_setup=None,stype=None):
    """
    Make eoSelectOne from python callable.

    The setup function is called ...
    """
    if klass_or_stype is not None and stype is None :
        stype = config._SOLUTION_TYPE

    base_ = utils.get_class("eoSelectOne"+config.TYPES[stype])

    op=None
    setup=None
    if hasattr(klass_or_fun,'__call__'):
        op = klass_or_fun.__call__
    else:
        op = klass_or_fun

    if hasattr(klass_or_fun,"setup"):
        setup = klass_or_fun.setup
    else:
        setup = _setup

    return base_(op,setup)


################################################
################################################
#### SELECT_ONE (src_pop) --> indiv
################################################
################################################
def det_tournament(t_size=2,stype=None):
    """
    eoDetTournamentSelect: a selection method that selects ONE individual by
    deterministic tournament

    Parameters
    ----------
    _tSize tournament size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoDetTournamentSelect"+config.TYPES[stype])
    return class_(t_size)


def stoch_tournament(t_rate=1.0,stype=None):
    """eoStochTournamentSelect: a selection method that selects ONE individual by binary stochastic tournament

    Parameters
    ----------
    t_rate : real,[0.5,1.0]
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoStochTournamentSelect"+config.TYPES[stype])
    return class_(t_rate)


def truncated(select_one,rate_fertile,interpret_as_rate=True,stype=None):
    """eoTruncatedSelectOne selects one individual using eoSelectOne as it's
    mechanism. Therefore eoTruncatedSelectOne needs an eoSelectOne in its ctor

    It will perform selection only from the top guys in the population.

    Parameters
    ----------
    select_one
    rate_fertile
    interpret_as_rate

    Note
    -----
    need to call .setup(pop) method before selecting!
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoTruncatedSelectOne"+config.TYPES[stype])
    return class_(select_one,rate_fertile,interpret_as_rate)


def proportional(stype=None):
    """eoProportionalSelect: select an individual proportional to her stored fitness
    value

    Changed the algorithm to make use of a cumulative array of fitness scores,
    This changes the algorithm from O(n) per call to  O(log n) per call. (MK)

    WARNING! eoProportionalSelect cannot be used with minimizing fitness !
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoProportionalSelect"+config.TYPES[stype])
    return class_()


def random(stype=None):
    """eoRandomSelect: a selection method that selects ONE individual randomly
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoRandomSelect"+config.TYPES[stype])
    return class_()


def best(stype=None):
    """ eoBestSelect: a selection method that always return the best
                (mainly for testing purposes)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoBestSelect"+config.TYPES[stype])
    return class_()


def round_robin(stype=None):
    """returns all individual in order WITHOUT USING FITNESS!!!
       looping back to the beginning when exhasuted
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoNoSelect"+config.TYPES[stype])
    return class_()


def sequential(ordered=True,stype=None):
    """All Individuals in order

Looping back to the beginning when exhausted, can be from best to
worse, or in random order.

It is the eoSelectOne equivalent of eoDetSelect - though eoDetSelect
always returns individuals from best to worst
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSequentialSelect"+config.TYPES[stype])
    return class_(ordered)


def elite_sequential(stype=None):
    """ All Individuals in order

The best individual first, then the others in sequence (random order).
for G3 evolution engine, see Deb, Anad and Joshi, CEC 2002

As eoSequentialSelect, it is an eoSelectOne to be used within the
eoEaseyEA algo, but conceptually it should be a global eoSelect, as it
selects a bunch of guys in one go (done in the setup function now)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEliteSequentialSelect"+config.TYPES[stype])
    return class_()
