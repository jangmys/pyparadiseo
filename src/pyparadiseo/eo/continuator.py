from pyparadiseo import config,utils

from .._core import eoContinue as Continue

# move to eval?
# from ..core import eoEvalFuncCounter as EvalFuncCounter

from .._core import eoGenContinue as GenContinue
from .._core import eoCombinedContinue as CombinedContinue
from .._core import eoEvalContinue as EvalContinue
from .._core import eoSteadyFitContinue as SteadyFitContinue
from .._core import eoSecondsElapsedContinue as SecondsElapsedContinue

from .._core import eoFitContinue as FitContinue


class _GenContinue():
    def __new__(cls,nb_gens,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoGenContinue"+config.TYPES[type])
        return class_(nb_gens)

class _EvalContinue():
    def __new__(cls,f,nb_evals,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEvalContinue"+config.TYPES[type])
        #if fun is callable...
        #if fun is evalFunc...
        return class_(f,nb_evals)

class _CombinedContinue():
    def __new__(cls,continue1,continue2=None,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoCombinedContinue"+config.TYPES[type])
        if continue2 is None:
            return class_(continue1)
        else:
            return class_(continue1,continue2)

class _SteadyFitContinue():
    def __new__(cls,min_gens,steady_gens,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSteadyFitContinue"+config.TYPES[type])
        return class_(min_gens,steady_gens)

class _SecondsElapsedContinue():
    def __new__(cls,seconds,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSecondsElapsedContinue"+config.TYPES[type])
        return class_(seconds)
