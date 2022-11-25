"""
Evolutionary Algorithms

There are three genetic algorithms

- `simpleGA` following Holland and Goldberg
- `easyEA`
- `fastGA`
"""

from pyparadiseo import config,utils
from pyparadiseo import eo
from pyparadiseo.eo import breeders

def simpleGA(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator,stype=None):
    """Simple Genetic Algorithm (SGA)

    **Pseudo-code**

    >>> while continuator(pop):
    >>>    select : create offspring from pop
    >>>    crossover : apply crossover to offspring
    >>>    mutate : apply mutation to offspring
    >>>    swap(offspring,pop) : full replacement
    >>>    eval : evaluate pop

    Parameters
    ==========
    selector : selectOne
    crossover : eoQuadOp
    p_cross : float
    mutate : MonOP
    p_mutate : float
    f_eval : EvalFunc
    continuator : eocontinue
    stype : str,optional

    Notes
    ======
    The components of SGA are

    - *selector* :
    - *crossover* :
    - *mutate*:
    - *continuator* :
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSGA"+config.TYPES[stype])

    #check arguments
    return class_(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator)


def easyEA(continuator,eval,breed,replace,stype=None):
    """Easy EA

    An easy-to-use evolutionary algorithm. You can use any chromosome, and any selection transformation, merging and evaluation algorithms.

    **Pseudo-code**

    >>> pop_eval(pop)
    >>> while continuator(pop):
    >>>    breed(pop,offspring)   # select and transform
    >>>    pop_eval(offspring)    # evaluate offspring
    >>>    replace(pop,offspring) # merge and reduce

    -----

    To assemble an EasyEA :

    >>> my_ea = easyEA(continuator,eval,breed,replace)

    where

    - evalFunc is a `fitness eval` or `pop_fitness_eval`
    - breed is a `breed` or a (select,transform) tuple
    - replace is a `replace` or a (merge,reduce) tuple

    then call

    >>> my_ea(pop)

    to evolve a population

    Parameters
    ==========
    continuator : an eoContinue
    f_eval : eoEvalFunc or eoPopEval
    breed : eoBreed or (eoSelect,eoTransform)
    replace : eoReplacement or (eoMerge,eoReduce)
    stype : solution type

    Returns
    =============
    an easyEA callable (derived from eoAlgo)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEasyEA"+config.TYPES[stype])

    ### breed can be (select+transform)
    if isinstance(breed,tuple):
        breed = breeders.select_transform(*breed,stype)

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
    select_cross : eoSelectOne<SolutionType>&,
    crossover : eoQuadOp<SolutionType>&,
    select_aftercross : eoSelectOne<SolutionType>&
    rate_mutation : double
    select_mut : eoSelectOne<EOT>&
    mutation : eoMonOp<SolutionType>&,
    pop_eval : eoPopEvalFunc<SolutionType>&,
    replacer : eoReplacement<SolutionType>&,
    contiuator : eoContinue<SolutionType>&,
    offspring_size : double
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
