"""
MO continuators
"""
from pyparadiseo import utils,config

def always_true(stype=None):
    """
    Iteration-continuator: continues until a number of iterations is reached

    Parameters
    ----------
    nb_iters : max iterations
    verbose : print message at end

    See also
    --------
    eoGenContinue (max_generation)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moTrueContinuator"+config.TYPES[stype])

    return class_()


def max_iterations(nb_iters,verbose=False,stype=None):
    """
    Iteration-continuator: continues until a number of iterations is reached

    Parameters
    ----------
    nb_iters : max iterations
    verbose : print message at end

    See also
    --------
    eoGenContinue (max_generation)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moIterContinuator"+config.TYPES[stype])

    return class_(nb_iters,verbose)


def full_evals(f,nb_evals,restart_counter=True,stype=None):
    """Continues until a number of evaluations has been made

    Parameters
    ----------
    f : an EvalFuncCounter function
    nb_evals : number of evaluations
    restart_counter :

    See also
    --------
    eoEvalContinue (eval_calls)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moFullEvalContinue"+config.TYPES[stype])
    #if fun is callable...
    #if fun is evalFunc...
    return class_(f,nb_evals,restart_counter)


def seconds_elapsed(seconds,verbose=False,stype=None):
    """
    Timed continuator: continues until a number of seconds is used
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moTimeContinuator"+config.TYPES[stype])
    return class_(seconds,verbose)
