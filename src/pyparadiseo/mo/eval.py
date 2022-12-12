"""
Neighbor evaluators
"""
from pyparadiseo import config,utils
from typing import Union,Optional,Callable

##abstract
from pyparadiseo import evaluator

from .._core import moEval
from .._core import moNeighborhoodEvaluation as NeighborhoodEvaluation

from .._core import NeighborEval
from .._core import moFullEvalByCopy as FullEvalByCopy
from .._core import moFullEvalByModif as FullEvalByModif

__all__=['moEval','moNeighborhoodEvaluation','neighbor_eval','neighbor_full_eval']


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

    Parameters
    ==========
    f_eval : Callable
        neighbor evaluation function

    Returns
    =======
    NeighborEval (a :py:class:`~pyparadiseo.mo.eval.moEval`)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("NeighborEval"+config.TYPES[stype])

    return class_(f_eval)



def neighbor_full_eval(f_eval,backable=False,stype=None):
    """
    Parameters
    ==========
    f_eval : evaluation function
    backable : bool (True if Neighbor has moveBack defined), default: False

    returns
    =======
    moFullEvalByCopy (default) - __call__(sol,neighbor) evaluates neighbor by making a tmp copy of solution, move it, evaluate and set fitness of neighbor
    moFullEvalByModif (backable=True) - __call__(sol,neighbor) moves solution, evaluates it an moves it back : requires moveBack to be defined in PyNeighbor
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_=None
    if not backable:
        class_ = utils.get_class("moFullEvalByCopy"+config.TYPES[stype])
    else:
        class_ = utils.get_class("moFullEvalByModif"+config.TYPES[stype])

    if isinstance(f_eval,utils.get_class("eoEvalFunc"+config.TYPES[stype])):
        return class_(f_eval)
    else:
        return class_(evaluator.fitness(f_eval))
