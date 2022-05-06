"""
/** @defgroup Continuators Stopping criteria
 *
 * A stopping criterion is called a "continue". This is a functor that is called at each generation end
 * and that return true if one should stop the search.
 *
 *  @ingroup Utilities
 */

/** Termination condition for the genetic algorithm
 * Takes the population as input, returns true for continue,
 * false for termination
 *
 * @ingroup Continuators
 * @ingroup Core
 */

base : eoContinue.h

eoGenContinue.h
eoCombinedContinue.h
eoSecondsElapsedContinue.h
eoEvalContinue.h
...
 """


from pyparadiseo import config,utils

from .._core import eoGenContinue as GenContinue
from .._core import eoCombinedContinue as CombinedContinue
from .._core import eoEvalContinue as EvalContinue
from .._core import eoSteadyFitContinue as SteadyFitContinue
from .._core import eoSecondsElapsedContinue as SecondsElapsedContinue

from .._core import eoFitContinue as FitContinue


class _GenContinue():
    """Generational continuator: continues until a number of generations is reached
    """
    def __new__(cls,nb_gens,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoGenContinue"+config.TYPES[type])
        return class_(nb_gens)

class _EvalContinue():
    """Continues until a number of evaluations has been made

    Parameters
    ----------
    - an EvalFuncCounter function
    - number of evaluations
    """
    def __new__(cls,f,nb_evals,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEvalContinue"+config.TYPES[type])
        #if fun is callable...
        #if fun is evalFunc...
        return class_(f,nb_evals)

class _CombinedContinue():
    """
    Combined continuators - logical AND:
    Continues until one of the embedded continuators says halt!
    """
    def __new__(cls,continue1,continue2=None,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoCombinedContinue"+config.TYPES[type])
        if continue2 is None:
            return class_(continue1)
        else:
            return class_(continue1,continue2)

class _SteadyFitContinue():
    """
    A continuator:  does a minimum number of generations, then
    stops whenever a given number of generations takes place without improvement
    """
    def __new__(cls,min_gens,steady_gens,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSteadyFitContinue"+config.TYPES[type])
        return class_(min_gens,steady_gens)

class _SecondsElapsedContinue():
    """
    Timed continuator: continues until a number of seconds is used
    """
    def __new__(cls,seconds,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSecondsElapsedContinue"+config.TYPES[type])
        return class_(seconds)
