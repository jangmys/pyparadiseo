"""
evaluate solutions

evaluators assign fitness or objective values to solutions
"""
from pyparadiseo import config,utils

#abstract base classes
from ._core import eoEvalFunc
from ._core import eoEvalFuncBin

# __import__("._core.eoEvalFuncBin")

from ._core import eoPopEvalFunc

# from ._core import FitnessEval
from ._core import ObjectiveEval
from ._core import eoPopLoopEval as PopLoopEval

from ._core import FitnessEvalBin
from ._core import ObjectiveEvalBin
from ._core import eoPopLoopEvalBin as PopLoopEvalBin

### counting eval function (for eoContinuator)
from ._core import eoEvalFuncCounter as EvalFuncCounter

# import importlib

# from pyparadiseo import _core


def fitness(fun=None,stype=None):
    """
    make fitness evaluation

    Parameters
    ----------
    fun : callable
        function must take a solution encoding as input and return a scalar fitness value

    Example
    -------

    Notes
    -----
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("FitnessEval"+config.TYPES[stype])

    if fun is not None:
        return class_(fun)
    else:
        return class_()


def objectives(fun=None,stype=None):
    """
    make fitness evaluation

    Parameters
    ----------
    fun : callable
        function must take a solution encoding as input and return a scalar fitness value

    Example
    -------

    Notes
    -----
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("ObjectiveEval"+config.TYPES[stype])

    if fun is not None:
        return class_(fun)
    else:
        return class_()


def pop_eval_from_fitness(f_eval,stype=None):
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoPopLoopEval"+config.TYPES[stype])

    if isinstance(f_eval,utils.get_class("eoEvalFunc"+config.TYPES[stype])):
        return class_(f_eval)
    else:
        return class_(fitness(fun=f_eval,type=stype))

# ===================================
# ===================================
# ===================================

#
# class _FitnessEval():
#     def __new__(cls,fun=None,type=None):
#         if type is None:
#             type = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("FitnessEval"+config.TYPES[type])
#
#         if fun is not None:
#             return class_(fun)
#         else:
#             return class_()
#
#
# class _ObjectiveEval():
#     def __new__(cls,fun=None,type=None):
#         if type is None:
#             type = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("ObjectiveEval"+config.TYPES[type])
#
#         if fun is not None:
#             return class_(fun)
#         else:
#             return class_()
#
#
# class _PopEval():
#     def __new__(cls,f_eval,stype=None):
#         if stype is None:
#             stype = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("eoPopLoopEval"+config.TYPES[stype])
#
#         if isinstance(f_eval,utils.get_class("eoEvalFunc"+config.TYPES[stype])):
#             return class_(f_eval)
#         else:
#             return class_(_FitnessEval(fun=f_eval,type=stype))
