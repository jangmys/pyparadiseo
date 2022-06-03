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
