"""
Test docstring for eo.selector module

The eo.selector module contains selection operators.

base :
eoSelect.h
    __call__(src_pop,dest_pop) --> void

eoSelect selects a number of individuals from the first argument and
puts it in the second. To emphasize that it should not try to enlarge
the population, the second argument is an eoPopRange, a simple struct
that holds a begin and end iterator to the population


eoSelectOne.h
    __call__(src_pop) --> Solution

 eoSelectOne selects only one element from a whole population.
    Most selection techniques are simply repeated applications
    of eoSelectOne.

"""
from pyparadiseo import config,utils


from .._core import eoSelect as Select

# from selectors.cpp
from .._core import eoDetSelect as DetSelect
from .._core import eoSelectMany as SelectMany
from .._core import eoSelectNumber as SelectNumber
from .._core import eoSelectPerc as SelectPerc
from .._core import eoTruncSelect as TruncSelect
from .._core import eoTruncatedSelectMany as TruncatedSelectMany

# from selectOne.cpp
from .._core import eoSelectOne as SelectOne
SelectOne.__doc__="this is the doc of SelectOne"


from .._core import eoDetTournamentSelect as DetTournamentSelect
from .._core import eoStochTournamentSelect as StochTournamentSelect

from .._core import eoTruncatedSelectOne as TruncatedSelectOne
from .._core import eoProportionalSelect as ProportionalSelect
from .._core import eoRandomSelect as RandomSelect
from .._core import eoNoSelect as NoSelect
from .._core import eoSequentialSelect as SequentialSelect
from .._core import eoEliteSequentialSelect as EliteSequentialSelect


################################################
################################################
#### SELECT_ONE (src_pop) --> indiv
################################################
################################################
class _DetTournamentSelect():
    """
    eoDetTournamentSelect: a selection method that selects ONE individual by
     deterministic tournament

    Parameters
    ----------
    _tSize tournament size
    """
    def __new__(cls,t_size=2,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoDetTournamentSelect"+config.TYPES[type])
        return class_(t_size)


class _StochTournamentSelect():
    """eoStochTournamentSelect: a selection method that selects ONE individual by
 binary stochastic tournament

     Parameters
     ----------
     t_rate [0.5,1.0]
    """
    def __new__(cls,t_rate=1.0,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoStochTournamentSelect"+config.TYPES[type])
        return class_(t_rate)


class _TruncatedSelectOne():
    """eoTruncatedSelectOne selects one individual using eoSelectOne as it's
    mechanism. Therefore eoTruncatedSelectOne needs an eoSelectOne in its ctor

    It will perform selection only from the top guys in the population.

     Parameters
     ----------
     select_one
     rate_fertile
     interpret_as_rate
    """
    def __new__(cls,select_one,rate_fertile,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoTruncatedSelectOne"+config.TYPES[type])
        return class_(select_one,rate_fertile,interpret_as_rate)


class _ProportionalSelect():
    """eoProportionalSelect: select an individual proportional to her stored fitness
    value

    Changed the algorithm to make use of a cumulative array of fitness scores,
    This changes the algorithm from O(n) per call to  O(log n) per call. (MK)

    WARNING! eoProportionalSelect cannot be used with minimizing fitness !
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoProportionalSelect"+config.TYPES[type])
        return class_()


class _RandomSelect():
    """eoRandomSelect: a selection method that selects ONE individual randomly
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoRandomSelect"+config.TYPES[type])
        return class_()


class _BestSelect():
    """ eoBestSelect: a selection method that always return the best
                (mainly for testing purposes)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoBestSelect"+config.TYPES[type])
        return class_()


class _NoSelect():
    """returns all individual in order WITHOUT USING FITNESS!!!
       looping back to the beginning when exhasuted
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoNoSelect"+config.TYPES[type])
        return class_()


class _SequentialSelect():
    """All Individuals in order

Looping back to the beginning when exhausted, can be from best to
worse, or in random order.

It is the eoSelectOne equivalent of eoDetSelect - though eoDetSelect
always returns individuals from best to worst
    """
    def __new__(cls,ordered=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSequentialSelect"+config.TYPES[type])
        return class_(ordered)


class _EliteSequentialSelect():
    """ All Individuals in order

The best individual first, then the others in sequence (random order).
for G3 evolution engine, see Deb, Anad and Joshi, CEC 2002

As eoSequentialSelect, it is an eoSelectOne to be used within the
eoEaseyEA algo, but conceptually it should be a global eoSelect, as it
selects a bunch of guys in one go (done in the setup function now)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEliteSequentialSelect"+config.TYPES[type])
        return class_()






################################################
################################################
#### SELECT (src_pop,dest_pop)
################################################
################################################
class _DetSelect():
    """
    eoDetSelect selects many individuals deterministically
    """
    def __new__(cls,rate=1.0,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoDetSelect"+config.TYPES[type])

        return class_(rate,interpret_as_rate)


class _SelectMany():
    """
    eoSelectMany selects many individuals using eoSelectOne as it's
        mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor

        It will use an eoHowMnay to determine the number of guys to select,
         and push them to the back of the destination population.

    Parameters
    ----------
    select_one
    Rate
    interpret_as_rate
    """
    def __new__(cls,select_one,rate=1.0,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSelectMany"+config.TYPES[type])

        return class_(select_one,rate,interpret_as_rate)


class _SelectNumber():
    """eoSelectNumber selects many individuals using eoSelectOne as it's mechanism. Therefore eoSelectNumber needs an eoSelectOne in its ctor.
    It will select a fixed number of individuals and pushes them to the back of the destination population.
    """
    def __new__(cls,select_one,nb_to_select=1,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSelectNumber"+config.TYPES[type])

        return class_(select_one,nb_to_select)



class _SelectPerc():
    """
    eoSelectPerc selects many individuals using eoSelectOne as it's
    mechanism. Therefore eoSelectPerc needs an eoSelectOne in its ctor

    It will select floor(rate*pop.size()) individuals and pushes them to
    the back of the destination population.
    """
    def __new__(cls,select_one,rate=1.0,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSelectPerc"+config.TYPES[type])

        return class_(select_one,rate)


class _TruncSelect():
    """
    eoTruncSelect selects individuals after truncating the population
 * using eoSelectOne as it's mechanism.
 * Therefore eoSelectMany needs an eoSelectOne in its ctor
 * It will use an eoHowMnay to determine the number of guys to keep,
    """
    def __new__(cls,select_one,how_many,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoTruncSelect"+config.TYPES[type])

        return class_(select_one,how_many)


class _TruncatedSelectMany():
    """selects many individuals using eoSelectOne as it's
    mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor

    It will use an eoHowMnay to determine the number of guys to select,
     and push them to the back of the destination population.

     And it will only perform selection from the top guys in the population.

     It is NOT a special case of eoSelectMany because it needs to SORT
     the population to discard the worst guys before doing the selection

     However, the same result can be obtained by embedding an
     eoTruncatedSelectOne into an eoSelectMany ...
    """
    def __new__(cls,select_one,rate_genitors,rate_fertile, _interpret_as_rateG=True,_interpret_as_rateF=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoTruncSelectMany"+config.TYPES[type])

        return class_(select_one,rate_genitors,rate_fertile, _interpret_as_rateG,_interpret_as_rateF)
