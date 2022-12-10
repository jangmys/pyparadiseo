"""
Select populations

The eo.selector module contains selection operators.

base :
eoSelect.h
``__call__(src_pop,dest_pop) -> void``

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

from .._core import eoSelect

__all__ = ['det_select','select_many','select_number','select_perc','select_trunc','select_truncated_many','eoSelect']

################################################
################################################
#### SELECT (src_pop,dest_pop)
################################################
################################################
def det_select(rate=1.0,interpret_as_rate=True,stype=None):
    """
    eoDetSelect selects many individuals deterministically :
        selects first rate*source.size() individuals

    eoDetSelect.h

    Parameters
    ==========
    rate : int, default=1.0
    interpret_as_rate : bool, default=True
    stype : optional

    Returns
    =======
    An eoDetSelect selector

    Notes
    =====
    math::[r \in [0.0,1.0]]
        select first r*source.size()

    rate=[N>0 positive integer], interpret_as_rate=False
        select first N individuals

    rate=[N<0 negative integer], interpret_as_rate=False
        select first source.size-|N| individuals

    """
    if type is None:
        type = config._SOLUTION_TYPE

    class_ = utils.get_class("eoDetSelect"+config.TYPES[type])

    return class_(rate,interpret_as_rate)


def select_many(select_one,rate=1.0,interpret_as_rate=True,stype=None):
    """
    eoSelectMany selects many individuals using eoSelectOne as it's
        mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor

        It will use an eoHowMnay to determine the number of guys to select,
         and push them to the back of the destination population.

    Parameters
    ----------
    select_one :
    Rate :
    interpret_as_rate :
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSelectMany"+config.TYPES[stype])

    return class_(select_one,rate,interpret_as_rate)



def select_number(select_one,nb_to_select=1,stype=None):
    """select a specified number of Individuals
    """
    if stype is None:
        stype = config._SOLUTION_TYPE
    class_ = utils.get_class("eoSelectNumber"+config.TYPES[stype])

    return class_(select_one,nb_to_select)


def select_perc(select_one,rate=1.0,stype=None):
    """
    eoSelectPerc selects many individuals using eoSelectOne as it's
    mechanism. Therefore eoSelectPerc needs an eoSelectOne in its ctor

    It will select floor(rate*pop.size()) individuals and pushes them to
    the back of the destination population.
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSelectPerc"+config.TYPES[stype])

    return class_(select_one,rate)


def select_trunc(select_one,how_many,type=None):
    """
    eoTruncSelect selects individuals after truncating the population using eoSelectOne as it's mechanism.
    Therefore eoSelectMany needs an eoSelectOne in its ctor
    It will use an eoHowMnay to determine the number of guys to keep,
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoTruncSelect"+config.TYPES[stype])

    return class_(select_one,how_many)


def select_truncated_many(select_one,rate_genitors,rate_fertile, _interpret_as_rateG=True,_interpret_as_rateF=True,type=None):
    """selects many individuals using eoSelectOne as it's
    mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor

    It will use an eoHowMnay to determine the number of guys to select, and push them to the back of the destination population.

    And it will only perform selection from the top guys in the population.

    It is NOT a special case of eoSelectMany because it needs to SORT the population to discard the worst guys before doing the selection

    However, the same result can be obtained by embedding an eoTruncatedSelectOne into an eoSelectMany ...
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoTruncSelectMany"+config.TYPES[stype])

    return class_(select_one,rate_genitors,rate_fertile, _interpret_as_rateG,_interpret_as_rateF)
