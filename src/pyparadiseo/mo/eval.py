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


class _NeighborEval():
    """
    neighbor evaluator
    """
    def __new__(cls,f_eval=None,sol_type=None):
        if sol_type is None:
            sol_type = config._SOLUTION_TYPE

        class_ = utils.get_class("NeighborEval"+config.TYPES[sol_type])

        if f_eval is not None:
            return class_(f_eval)
        else:
            print("warning : no evaluation function provided to NeighborEval")
            return class_()


def neighbor_eval(f_eval,sol_type=None):
    """
    wrap function into neighbor evaluator

    incremental
    """
    if sol_type is None:
        sol_type = config._SOLUTION_TYPE

    class_ = utils.get_class("NeighborEval"+config.TYPES[sol_type])

    return class_(f_eval)



class _NeighborFullEval():
    """
    backable : bool (True if Neighbor has moveBack defined)
    """
    def __new__(cls,f_eval,backable=False,sol_type=None):
        if sol_type is None:
            sol_type = config._SOLUTION_TYPE

        class_=None
        if not backable:
            class_ = utils.get_class("moFullEvalByCopy"+config.TYPES[sol_type])
        else:
            class_ = utils.get_class("moFullEvalByModif"+config.TYPES[sol_type])

        return class_(f_eval)
