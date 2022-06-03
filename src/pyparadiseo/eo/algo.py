"""
EO Algorithms
=============

simpleGA :

easyEA :

fastGA :
"""



from pyparadiseo import config,utils

def simpleGA(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator,stype=None):
    """
    simple genetic algorithm

    Parameters
    ----------
    selector : selectOne
    crossover : eoQuadOp
    p_cross : float
    mutate : MonOP
    p_mutate : float
    f_eval : EvalFunc
    continuator : eocontinue
    stype : str,optional
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSGA"+config.TYPES[stype])

    #check arguments
    return class_(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator)


def easyEA(continuator,eval,selector,transformer,replacer,stype=None):
    """Easy EA

    Notes
    -----
    (continue,EvalFunc,Breed,Replace)  #order inverse of SGA? (mandatory left, optional right)
    (continue,Evalfunc,Breed,Replace,unsigned)
    (continue,PopEval,Breed,Replace)
    (continue,EvalFunc,Breed,Merge,Reduce)
    (continue,EvalFunc,Select,Transform,Replace)
    (continue,EvalFunc,Select,Transform,Merge,Reduce)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEasyEA"+config.TYPES[type])

    return class_(continuator,eval,selector,transformer,replacer)


def fastGA(rate_crossover,select_cross,crossover,select_aftercross,rate_mutation,
        select_mutation,pop_eval,replacer,continuator,offsprings_size,stype=None):
    """Fast GA

    Parameters
    ----------
    rate_crossover : double,
    select_cross : eoSelectOne<SolutionType>&,
    crossover : eoQuadOp<SolutionType>&,
    select_aftercross : eoSelectOne<SolutionType>&
    rate_mutation : double
    select_mutation : eoMonOp<SolutionType>&,
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
        select_mutation,
        pop_eval,
        replacer,
        continuator,
        offsprings_size
    )
