"""
Neighbor evaluators
"""


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
    base class
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
