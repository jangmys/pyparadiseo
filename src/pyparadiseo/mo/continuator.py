"""
MO stopping criteria
"""
from pyparadiseo import utils,config

from .._core import moContinuator

__all__=['moContinuator','always_true','max_iterations','full_evals','seconds_eseconds_elapsed','pymoCheckpoint']

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

    class_ = utils.get_class("moFullEvalContinuator"+config.TYPES[stype])

    return class_(f,nb_evals,restart_counter)


def seconds_elapsed(seconds,verbose=False,stype=None):
    """
    Timed continuator: continues until a number of seconds is used
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moTimeContinuator"+config.TYPES[stype])
    return class_(seconds,verbose)


# like moCheckpoint
class pymoCheckpoint(moContinuator):
    def __init__(self, cont, freq_interval=1):
        moContinuator.__init__(self)
        self.continuators = [cont]
        self.stats = []

        self.interval = freq_interval
        self.counter = 0

    def add_cont(self,_cont):
        self.continuators.append(_cont)

    def add_stat(self,_stat):
        self.stats.append(_stat)

    def init(self,sol):
        for stat in self.stats:
            stat.init(sol)

        for cont in self.continuators:
            cont.init(sol)

    def __call__(self,sol):
        if self.counter % self.interval == 0:
            for stat in self.stats:
                stat(sol)

        for cont in self.continuators:
            if not cont(sol):
                return False

        return True

    def lastCall(self,solution):
        for stat in self.stats:
            stat.lastCall(sol)


def checkpoint(continu,interval_freq=1,stype=None):
    """
    A checkpointing continuator

    Parameters
    ==========
    continu : moContinuator
        a continuator
    interval_freq : int
        sampling frequency for statistics

    Returns
    ========
    moCheckpoint : moCheckpoint
        as it says
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moCheckpoint"+config.TYPES[stype])

    return class_(continu,interval_freq)
