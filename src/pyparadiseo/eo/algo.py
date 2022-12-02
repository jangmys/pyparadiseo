"""
Evolutionary Algorithms

There are three genetic algorithms

- :py:class:`~pyparadiseo.eo.algo.eoSGA` - a **simple GA** following Holland and Goldberg
- :py:class:`~pyparadiseo.eo.algo.eoEasyEA`
- :py:class:`~pyparadiseo.eo.algo.eoFastGA`
"""
from typing import Union,Optional

from pyparadiseo import config,utils
from pyparadiseo import eo

#import for typehints
from pyparadiseo.eo.continuator import eoContinue
from pyparadiseo.evaluator import eoEvalFunc,eoPopEvalFunc

from .._core import eoAlgo
eoAlgo.__doc__="""Abstract base class for EO algorithms.

It defines a functor ``__call__(population) -> None``.

Use ``eoAlgo`` creation methods in :py:mod:`~pyparadiseo.eo.algo`
"""

from .._core import eoSGA
from .._core import eoFastGA
from .._core import eoEasyEA

__all__=['simpleGA','easyEA','fastGA',
        'eoAlgo','eoSGA','eoFastGA','eoEasyEA']



def simpleGA(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator,stype: Optional[str]=None):
    """Simple Genetic Algorithm (SGA)

    Parameters
    ==========
    selector : :py:class:`~pyparadiseo.eo.select_one.eoSelectOne`
        select one individual from parent population. is repeated as many times as needed to fill offspring population (same size as parents)
    crossover : :py:class:`~pyparadiseo.operator.eoQuadOp`
        crossover
    p_cross : float
        probability to apply crossover
    mutate : :py:class:`~pyparadiseo.operator.eoMonOp`
        mutation
    p_mutate : float
    f_eval : :py:class:`~pyparadiseo.evaluator.eoEvalFunc`
        fitness evaluation function
    continuator : :py:class:`~pyparadiseo.eo.continuator.eoContinue`
        stopping criterion
    stype : str,optional
        solution type

    Returns
    ========
    eoSGA(:py:class:`eoAlgo`)

    Notes
    ======
    following Holland and Goldberg

    **Pseudo-code**

    .. code-block:: python

        while continuator(pop):
            select(pop,offspring) # create offspring from pop
            crossover(offspring)  # apply crossover to offspring
            mutate(offspring)     # apply mutation to offspring
            swap(offspring,pop)   # full replacement
            eval(pop)


    - `eo/eoSGA.h`
    - `eo/eoAlgo.h`

    This is a paragraph that contains `a link`_.

    .. _a link: https://gitlab.inria.fr/paradiseo/pyparadiseo/-/blob/master/src/paradiseo/eo/eoSGA.h
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSGA"+config.TYPES[stype])

    #check arguments
    return class_(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator)


def easyEA(continuator: eoContinue,eval : Union[eoEvalFunc,eoPopEvalFunc],breed,replace,stype=None):
    """Easy EA

    An easy-to-use evolutionary algorithm. You can use any chromosome, and any selection transformation, merging and evaluation algorithms.

    .. code-block:: python

        pop_eval(pop)
        while continuator(pop):
            breed(pop,offspring)   # select and transform
            pop_eval(offspring)    # evaluate offspring
            replace(pop,offspring) # merge and reduce


    To assemble an EasyEA :


    ``my_ea = easyEA(continuator,eval,breed,replace)``

    where

    - ``eval`` is a :py:class:`eoEvalFunc` or :py:class:`eoPopEvalFunc`
    - ``breed`` is a :py:class:`eoBreed` or a (:py:class:`eoSelect`, :py:class:`eoTransform`) tuple
    - ``replace`` is a :py:class:`eoReplace` or a (:py:class:`eoMerge`, :py:class:`eoReduce`) tuple

    then call

    >>> my_ea(pop)

    to evolve a population

    Parameters
    ==========
    continuator : :py:class:`~pyparadiseo.eo.continuator.eoContinue`
        stopping criterion
    eval : :py:class:`~pyparadiseo.evaluator.eoEvalFunc` or :py:class:`~pyparadiseo.evaluator.eoPopEvalFunc`
        fitness evaluator (individual or population)
    breed : :py:mod:`~pyparadiseo.eo.breeders.eoBreed` or (:py:mod:`~pyparadiseo.eo.selector.eoSelect`, :py:mod:`~pyparadiseo.eo.transform.eoTransform`) tuple
        breeder, selects parents and transforms children
    replace : :py:mod:`~pyparadiseo.eo.replace.eoReplacement` or (:py:mod:`~pyparadiseo.eo.merge.eoMerge`, :py:mod:`~pyparadiseo.eo.reduce.eoReduce`) tuple
        replacement, can be described by merge+reduce
    stype : str,optional
        solution type (default is global solution type)

    Returns
    =============
    an easyEA object (derived from :py:class:`eoAlgo`)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEasyEA"+config.TYPES[stype])

    ### breed can be (select+transform)
    if isinstance(breed,tuple):
        breed = eo.breeders.select_transform(*breed,stype)

    ### replace can be (merge+reduce)
    if isinstance(replace,tuple):
        replace = eo.replace(*replace,stype)

    return class_(continuator,eval,breed,replace)


def fastGA(rate_crossover,select_cross,crossover,select_aftercross,
    rate_mutation,select_mut,mutation,pop_eval,replacer,continuator,offsprings_size=0,stype=None):
    """Fast GA

    Parameters
    ----------
    rate_crossover : double,
        crossover probability
    select_cross : eoSelectOne
        select individual for crossover
    crossover : eoQuadOp
        crossover
    select_aftercross : eoSelectOne
        select one of two crossover children for mutation
    rate_mutation : double
        mutation probability (if crossover)
    select_mut : eoSelectOne
        select for mutation
    mutation : eoMonOp
        mutations
    pop_eval : eoPopEvalFunc
        population evaluator
    replacer : eoReplacement
        replacement
    contiuator : eoContinue
        termination criterion
    offspring_size : double
        by default offspring size is equal to parent pop
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoFastGA"+config.TYPES[stype])

    return class_(
        rate_crossover,
        select_cross,
        crossover,
        select_aftercross,
        rate_mutation,
        select_mut,
        mutation,
        pop_eval,
        replacer,
        continuator,
        offsprings_size
    )
