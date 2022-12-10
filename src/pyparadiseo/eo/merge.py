"""
    Merge populations

    base : eoMerge.h

    __call__(old_pop,new_pop) --> None

     * Merges the old population (first argument), with the new generation
     *
     * Its signature is exactly
     * that of the selection base eoSelect, but its purpose is to merge the
     * two populations into one (the second argument).
     * Note that the algorithms assume that the second argument denotes the
     * next generation.
 """
from pyparadiseo import config,utils

from .._core import eoMerge

__all__ = ['elitist','plus','no_elitist','eoMerge']


def elitist(rate,interpret_as_rate=True,stype=None):
    """
    Straightforward elitism class, specify the number of individuals to copy
    into new geneneration or the rate w.r.t. pop size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoElitism"+config.TYPES[stype])

    return class_(rate,interpret_as_rate)


def plus(stype=None):
    """
    copies entire population to offspring

    new size of offspring is pop.size()+offsring.size()
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    return utils.get_class("eoPlus"+config.TYPES[stype])()


def no_elitist(stype=None):
    """No Elitism (rate==0)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

        return utils.get_class("eoNoElitism"+config.TYPES[stype])()
