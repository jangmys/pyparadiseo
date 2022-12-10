"""
Stopping criteria for EO and MOEO algorithms.

Contains object creation functions to make continuators.
Continuators are functors derived from the :py:class:`eoContinue` base class.
Takes the population as input, returns true for continue, false for termination.

Notes
=====
1. use a provided continuator
2. wrap a python callable in a eoContinue
3. inherit from eoContinue ans specialize __call__ operator

See also
========
eoContinue.h
eoGenContinue.h
eoCombinedContinue.h
eoSecondsElapsedContinue.h
eoEvalContinue.h
"""
from pyparadiseo import config,utils
from typing import Callable

from .._core import eoCombinedContinue
from .._core import eoContinue
eoContinue.__doc__="""
eoContinue abstract base class - termination criteria

eoContinue is a `eoUF<const eoPop<EOT>&, bool>` : it takes a population as input and returns a bool (True iff continue)
"""

__all__ = ['continuator','max_generations','eval_calls','combined_continue','steady_fitness','target_fitness','seconds_elapsed','eoContinue','eoCombinedContinue']



def continuator(cont_call: Callable=None,stype: str=None) -> eoContinue:
    """
    Make eoContinue from python callable.

    ``cont_call`` must take :py:class:`~pyparadiseo._core.Pop` as input and return ``boolean`` (True for continue, False for termination)

    Parameters
    ----------
    cont_call : Callable
        takes :py:class:`~pyparadiseo._core.Pop` as input and returns ``True`` for continue
    stype : str, optional
        solution type

    Returns
    -------
    eoContinue
        termination criterion for EO and MOEO algorithms
    """
    if stype is None :
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("PyContinue"+config.TYPES[stype])

    return class_(cont_call)


def max_generations(nb_gens : int,stype: str=None) -> eoContinue:
    """Generational continuator: continues until a number of generations is reached

    Parameters
    ----------
    nb_gens : int
        number of generations to do
    stype : str, optional
        solution type

    Returns
    -------
    eoGenContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoGenContinue"+config.TYPES[stype])

    return class_(nb_gens)


def eval_calls(f: Callable, nb_evals: int, stype : str=None):
    """Continues until a number of evaluations has been made

    Parameters
    ----------
    f : Callable
        an eoEvalFuncCounter or any callable
    nb_evals : int
        number of evaluations
    stype : str, optional
        solution type

    Returns
    -------
    eoEvalContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEvalContinue"+config.TYPES[stype])
    #if fun is callable...
    #if fun is evalFunc...
    return class_(f,nb_evals)


def combined_continue(continue1,continue2=None,stype=None):
    """
    Combined continuator (logical AND)

    Continues until one of the embedded continuators returns ``False``

    Parameters
    ----------
    continue1 : :py:class:`eoContinue`
        first continue
    continue2 : :py:class:`eoContinue`
        second continue
    stype : str, optional
        solution type

    Returns
    -------
    eoCombinedContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms

    Notes
    -----
    The returned object has an ``.add`` method that allows to append additional continuators.
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoCombinedContinue"+config.TYPES[stype])
    if continue2 is None:
        return class_(continue1)
    else:
        cont = class_(continue1)
        cont.add(continue2)
        return cont


def steady_fitness(min_gens: int,steady_gens: int,stype=None):
    """
    Continue for a minimum number of generations, then
    stop whenever a given number of generations takes place without improvement.

    Parameters
    ----------
    min_gens : int
        minimum of generations to perform
    steady_gens : int
        number of generations without improvement needed to stop
    stype : str, optional
        solution type

    Returns
    -------
    eoSteadyFitContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms
    """
    if stype is None:
        stype = config._SOLUTION_TYPE
    class_ = utils.get_class("eoSteadyFitContinue"+config.TYPES[stype])
    return class_(min_gens,steady_gens)


def target_fitness(fitness: float,stype=None):
    """
    Continue until a given target fitness is reached.

    Parameters
    ----------
    fitness : float
        target fitness value
    stype : str, optional
        solution type

    Returns
    -------
    eoFitContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms
    """
    if stype is None:
        stype = config._SOLUTION_TYPE
    class_ = utils.get_class("eoFitContinue"+config.TYPES[stype])
    return class_(fitness)


def seconds_elapsed(seconds: int,stype=None):
    """
    Continue until a number of seconds is elapsed.

    Parameters
    ----------
    seconds : int
        target fitness value
    stype : str, optional
        solution type

    Returns
    -------
    eoSecondsElapsedContinue(eoContinue)
        an :py:class:`eoContinue` for EO and MOEO algorithms
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSecondsElapsedContinue"+config.TYPES[stype])
    return class_(seconds)
