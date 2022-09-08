"""
Continuators : Stopping criteria

A stopping criterion is called a "continue". This is a functor that is called at each generation end and that return true if one should stop the search.

Termination condition for the genetic algorithm

Takes the population as input, returns true for continue, false for termination

base : eoContinue.h

eoGenContinue.h
eoCombinedContinue.h
eoSecondsElapsedContinue.h
eoEvalContinue.h
 """

from pyparadiseo import config,utils

def continuator(klass_or_stype=None,stype=None):
    """
    class decorator

    make pyparadiseo continuator from python callable class

    callable must take population as input and return boolean (True for continue, False for termination)

    Note
    ====
    see https://github.com/numba/numba/blob/main/numba/experimental/jitclass/decorators.py
    """
    if klass_or_stype is not None and stype is None :
        stype = config._SOLUTION_TYPE

    base_ = utils.get_class("eoContinue"+config.TYPES[stype])

    def wrap(kls):
        class derived(kls,base_):
            pass

        return derived

    if klass_or_stype is None:
        return wrap
    else:
        return wrap(klass_or_stype)

# class decorate(object):
#     def __init__(self, arg):
#         self.arg = arg
#     def __call__(self, cls):
#         class Wrapped(cls):
#             classattr = self.arg
#             def new_method(self, value):
#                 return value * 2
#         return Wrapped



def from_class(_callable_class,stype=None):
    """
    make pyparadiseo continuator from python callable

    callable must take population as input and return boolean (True for continue, False for termination)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    base_ = utils.get_class("eoContinue"+config.TYPES[stype])

    class derived(_callable_class,base_):
        pass

    return derived




def max_generations(nb_gens,stype=None):
    """Generational continuator: continues until a number of generations is reached
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoGenContinue"+config.TYPES[stype])

    return class_(nb_gens)


def eval_calls(f,nb_evals,stype=None):
    """Continues until a number of evaluations has been made

    Parameters
    ----------
    f : an EvalFuncCounter function
    nb_evals : number of evaluations
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEvalContinue"+config.TYPES[stype])
    #if fun is callable...
    #if fun is evalFunc...
    return class_(f,nb_evals)


def combined_continue(continue1,continue2=None,stype=None):
    """
    Combined continuators - logical AND:
    Continues until one of the embedded continuators says halt!
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoCombinedContinue"+config.TYPES[stype])
    if continue2 is None:
        return class_(continue1)
    else:
        return class_(continue1,continue2)


def steady_fitness(min_gens,steady_gens,stype=None):
    """
    A continuator:  does a minimum number of generations, then
    stops whenever a given number of generations takes place without improvement
    """
    if stype is None:
        stype = config._SOLUTION_TYPE
    class_ = utils.get_class("eoSteadyFitContinue"+config.TYPES[stype])
    return class_(min_gens,steady_gens)


def target_fitness(fitness,stype=None):
    """
    A continuator:  does a minimum number of generations, then
    stops whenever a given number of generations takes place without improvement
    """
    if stype is None:
        stype = config._SOLUTION_TYPE
    class_ = utils.get_class("eoFitContinue"+config.TYPES[stype])
    return class_(fitness)


def seconds_elapsed(seconds,stype=None):
    """
    Timed continuator: continues until a number of seconds is used
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSecondsElapsedContinue"+config.TYPES[stype])
    return class_(seconds)
