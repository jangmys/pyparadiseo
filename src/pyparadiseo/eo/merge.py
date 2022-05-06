"""
    Mergers

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

from .._core import eoMerge as Merge

from .._core import eoElitism as Elitism
from .._core import eoNoElitism as NoElitism
from .._core import eoPlus as Plus

class _Elitism():
    """Straightforward elitism class, specify the number of individuals to copy
    into new geneneration or the rate w.r.t. pop size
    """
    def __new__(cls,rate,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoElitism"+config.TYPES[type])

        return class_(rate,interpret_as_rate)


class _NoElitism():
    """No Elitism (rate==0)
    """
    def __new__(cls,rate,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoNoElitism"+config.TYPES[type])

        return class_()


class _Plus():
    """Very elitist class, copies entire population into next gen
    """
    def __new__(cls,rate,interpret_as_rate=True,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoPlus"+config.TYPES[type])

        return class_()
