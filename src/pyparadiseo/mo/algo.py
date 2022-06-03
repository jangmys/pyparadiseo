"""
Local Search algorithms
"""
from pyparadiseo import utils,config


#ABC
from .._core import moLocalSearch as LocalSearch



from .._core import moSimpleHC as SimpleHC
from .._core import moFirstImprHC as FirstImprHC
from .._core import moRandomBestHC as RandomBestHC
from .._core import moNeutralHC as NeutralHC

from .._core import moRandomSearch as RandomSearch
from .._core import moRandomWalk as RandomWalk
from .._core import moRandomNeutralWalk as RandomNeutralWalk

from .._core import moMetropolisHasting as MetropolisHasting
from .._core import moSA as SA
from .._core import moTS as TS




#SimpleHC(Neighborhood,solEval,nborEval)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator,moNeighborComparator,moSolNeighborComparator)
def simple_hill_climber(neighborhood,f_eval,nbor_eval,continuator=None,compareN=None,compareSN=None,hc_type='simple',stype=None):
    """
     * Simple HC:
     * Hill-Climbing local search
     *
     * At each iteration,
     *   the first best solution in the neighborhood is selected
     *   if the selected neighbor have higher fitness than the current solution
     *       then the solution is replaced by the selected neighbor
     *   the algorithm stops when there is no higher neighbor

     * Simple constructor for a hill-climber
     * @param _neighborhood the neighborhood
     * @param _fullEval the full evaluation function
     * @param _eval neighbor's evaluation function
     * @param _cont an external continuator
     * @param _compN  a neighbor vs neighbor comparator
     * @param _compSN a solution vs neighbor comparator
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = None
    if hc_type == 'simple':
        class_ = utils.get_class("moSimpleHC"+config.TYPES[stype])
    elif hc_type == 'first_improve':
        class_ = utils.get_class("moFirstImprHC"+config.TYPES[stype])
    elif hc_type == 'random_best':
        class_ = utils.get_class("moRandomBestHC"+config.TYPES[stype])

    if class_ is None:
        raise TypeError("invalid hc_type")

    if compareN is not None:
        if compareSN is None:
            raise TypeError("must provide both, compareN and compareSN.")
        return class_(neighborhood,f_eval,nbor_eval,compareN,compareSN,continuator)
    elif continuator is not None:
        return class_(neighborhood,f_eval,nbor_eval,continuator)
    else:
        return class_(neighborhood,f_eval,nbor_eval)



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

#SimpleHC(Neighborhood,solEval,nborEval)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator,moNeighborComparator,moSolNeighborComparator)
class _SimpleHC():
    """
     * Simple HC:
     * Hill-Climbing local search
     *
     * At each iteration,
     *   the first best solution in the neighborhood is selected
     *   if the selected neighbor have higher fitness than the current solution
     *       then the solution is replaced by the selected neighbor
     *   the algorithm stops when there is no higher neighbor
    """
    def __new__(cls,neighborhood,f_eval,nbor_eval,continuator=None,compareN=None,compareSN=None,hc_type='simple',sol_type=None):
        """
         * Simple constructor for a hill-climber
         * @param _neighborhood the neighborhood
         * @param _fullEval the full evaluation function
         * @param _eval neighbor's evaluation function
         * @param _cont an external continuator
         * @param _compN  a neighbor vs neighbor comparator
         * @param _compSN a solution vs neighbor comparator
        """
        if sol_type is None:
            sol_type = config._SOLUTION_TYPE

        class_ = None
        if hc_type == 'simple':
            class_ = utils.get_class("moSimpleHC"+config.TYPES[sol_type])
        elif hc_type == 'first_improve':
            class_ = utils.get_class("moFirstImprHC"+config.TYPES[sol_type])
        elif hc_type == 'random_best':
            class_ = utils.get_class("moRandomBestHC"+config.TYPES[sol_type])

        if class_ is None:
            print("invalid hc_type")

        if compareN is not None:
            if compareSN is None:
                print("need compareSN!")
            return class_(neighborhood,f_eval,nbor_eval,compareN,compareSN,continuator)
        elif continuator is not None:
            return class_(neighborhood,f_eval,nbor_eval,continuator)
        else:
            return class_(neighborhood,f_eval,nbor_eval)


class _NeutralHC():
    """
     * Hill-Climbing local search
     *
     * At each iteration,
     *   one of the random best solution in the neighborhood is selected
     *   if the selected neighbor have higher or equal fitness than the current solution
     *       then the solution is replaced by the selected neighbor
     *   the algorithm stops when there is no higher or equal neighbor, or if the number of iterations is too large
    """
    def __new__(cls,neighborhood,f_eval,nbor_eval,max_steps,continuator=None,compareN=None,compareSN=None,hc_type='simple',stype=None):
        """
         * Simple constructor for a hill-climber
         * @param _neighborhood the neighborhood
         * @param _fullEval the full evaluation function
         * @param _eval neighbor's evaluation function
         * @param _cont an external continuator
         * @param _compN  a neighbor vs neighbor comparator
         * @param _compSN a solution vs neighbor comparator
        """
        if stype is None:
            stype = config._SOLUTION_TYPE

        class_ = utils.get_class("moNeutralHC"+config.TYPES[stype])

        if compareN is not None:
            if compareSN is None:
                print("need compareSN!")
            return class_(neighborhood,f_eval,nbor_eval,max_steps,compareN,compareSN,continuator)
        elif continuator is not None:
            return class_(neighborhood,f_eval,nbor_eval,max_steps,continuator)
        else:
            return class_(neighborhood,f_eval,nbor_eval,max_steps)
