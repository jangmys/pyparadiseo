"""
Neighbor evaluators
"""
from pyparadiseo import config,utils

##abstract
from .._core import moEval as Eval
from .._core import moNeighborhoodEvaluation as NeighborhoodEvaluation

from .._core import NeighborEval
from .._core import moFullEvalByCopy as FullEvalByCopy
from .._core import moFullEvalByModif as FullEvalByModif

class _Eval():
    """
    base class
    """
    def __new__(cls,sol_type=None):
        pass


def neighbor_eval(f_eval,stype=None):
    """
    wrap function into neighbor evaluator

    incremental

    f_eval(solution-encoding, solution-fitness, index) --> new-solution-fitness

    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("NeighborEval"+config.TYPES[stype])

    return class_(f_eval)


def neighbor_full_eval(f_eval,stype=None):
    """
    backable : bool (True if Neighbor has moveBack defined)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_=None
    if not backable:
        class_ = utils.get_class("moFullEvalByCopy"+config.TYPES[sol_type])
    else:
        class_ = utils.get_class("moFullEvalByModif"+config.TYPES[sol_type])

    return class_(f_eval)
