"""
Single-solution based algorithms

- Hill Climbers
- Simulated Annealing
- Tabu Search
"""
from pyparadiseo import utils,config
from typing import Union,Optional,Callable

#ABC
from .._core import moLocalSearch

from .._core import moSimpleHC
# from .._core import moFirstImprHC as FirstImprHC
# from .._core import moRandomBestHC as RandomBestHC
# from .._core import moNeutralHC

# from .._core import moRandomSearch as RandomSearch
# from .._core import moRandomWalk
# from .._core import moRandomNeutralWalk
#
# from .._core import moMetropolisHasting
# from .._core import moSA
# from .._core import moTS

__all__=['hill_climber','random_search','random_walk','random_neutral_walk','metropolis_hastings','simulated_annealing','tabu_search','moLocalSearch','moSimpleHC']


def _set_move(self,move_op,move_back_op=None,index_table=None):
    """
    local function injected in LS algo to set move and index_table
    """
    self.set_move(move_op)

    if move_back_op is not None:
        self.set_move_back(move_back_op)

    if index_table is not None:
        self.set_index_table(index_table)


#SimpleHC(Neighborhood,solEval,nborEval)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator,moNeighborComparator,moSolNeighborComparator)
def hill_climber(neighborhood,f_eval,nbor_eval,move,continuator=None,compareN=None,compareSN=None,nneutral_steps=1,hc_type='simple',stype=None):
    """Hill-Climbing local search

    At each iteration, an improving solution in the neighborhood is selected. If the selected neighbor has higher fitness than the current solution, then the solution is replaced by the selected neighbor. The algorithm stops when there is no higher neighbor.

    Three variants are available
    - 'simple' : accept best neighbor (take first, if multiple best neighbors are found)
    - 'first_improve' : accept first improving neighbor
    - 'random_best' : accept one of the best neighbors at random

    Parameters
    ===========
    neighborhood : moNeighborhood
        a Neighborhood
    f_eval : eoEvalFunc
        full evaluation function
    nbor_eval : moEval
        neighbor evaluation function
    move : Callable
        a move function
    continuator : moContinuator
        default = None
    compareN : moNeighborComparator [optional]
        neighbor vs neighbor comparator, default = None
    compSN  : moNeighborComparator [optional]
        a solution vs neighbor comparator, default = None
    hc_type : str [optional]
        hill-climber type
    stype : str [optional]
        solution type
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    #get hill-climber class
    class_ = None
    if hc_type == 'simple':
        class_ = utils.get_class("moSimpleHC"+config.TYPES[stype])
    elif hc_type == 'first_improve':
        class_ = utils.get_class("moFirstImprHC"+config.TYPES[stype])
    elif hc_type == 'random_best':
        class_ = utils.get_class("moRandomBestHC"+config.TYPES[stype])
    elif hc_type == 'neutral':
        class_ = utils.get_class("moNeutralHC"+config.TYPES[stype])

    if class_ is None:
        raise TypeError("invalid hc_type")

    class_.set_move_ = _set_move

    if config.is_minimizing():
        print("IS MIN\n")

    algo = None
    if compareN is not None:
        if compareSN is None:
            raise TypeError("must provide both, compareN and compareSN.")

        if hc_type == 'neutral':
            algo = class_(neighborhood,f_eval,nbor_eval,continuator,compareN,compareSN,nneutral_steps)
        else:
            algo = class_(neighborhood,f_eval,nbor_eval,continuator,compareN,compareSN)
    elif continuator is not None:
        if hc_type == 'neutral':
            algo = class_(neighborhood,f_eval,nbor_eval,continuator,nneutral_steps)
        else:
            algo = class_(neighborhood,f_eval,nbor_eval,continuator)
    else:
        if hc_type == 'neutral':
            algo = class_(neighborhood,f_eval,nbor_eval,nneutral_steps)
        else:
            algo = class_(neighborhood,f_eval,nbor_eval)

    algo.set_move(move)
    return algo


def random_search(init,eval,max_eval,stype=None):
    """
    (this doesn't optimization anything)

    see Paradiseo...
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moRandomSearch"+config.TYPES[stype])

    return class_(init,eval,max_eval)


def random_walk(neighborhood,eval,neighbor_eval,max_eval=None,continuator=None,stype=None):
    """
    Parameters
    ----------
    neighborhood : moNeighborhood
        explain

    ...
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moRandomWalk"+config.TYPES[stype])

    if 'max_eval' is not None:
        return class_(neighborhood,eval,neighbor_eval,max_eval)
    if 'continuator' is not None:
        return class_(neighborhood,eval,neighbor_eval,continuator)
    else:
        raise ValueError("need a stopping criterion")


### merge with previous
def random_neutral_walk(neighborhood,eval,nborEval,max_eval,continuator=None,SNcomparator=None,stype=None):
    """
    Random Neutral Walk

    allow neutral moves
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moRandomNeutralWalk"+config.TYPES[stype])

    return class_(neighborhood,eval,nborEval,max_eval)


def metropolis_hastings(neighborhood,eval,nborEval,max_eval,stype=None):
    """
    Metropolis-Hastings
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moMetropolisHasting"+config.TYPES[stype])

    return class_(neighborhood,eval,nborEval,max_eval)


def simulated_annealing(neighborhood,eval,nborEval,t_start=10,alpha=0.9,span=100,t_final=0.01,stype=None):
    """
    Simulated Annealing
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moSA"+config.TYPES[stype])

    return class_(neighborhood,eval,nborEval,t_start,alpha,span,t_final)


def tabu_search(neighborhood,eval,nborEval,time_limit,tabu_list_size,stype=None):
    """
    Tabu Search
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moTS"+config.TYPES[stype])

    return class_(neighborhood,eval,nborEval,time_limit,tabu_list_size)
