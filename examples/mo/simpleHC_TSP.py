import sys
sys.path.append("..")

from problems import onemax

#provisoire
import pyparadiseo as pp

from pyparadiseo import config
from pyparadiseo import _core

from pyparadiseo import evaluator
from pyparadiseo import initializer

from pyparadiseo import solution

from pyparadiseo import mo

from pyparadiseo.mo import continuator,comparator
from pyparadiseo.mo import eval,neighborhood,algo,Eval,Neighbor

import time

import numpy as np


from problems import tsp
#### tsp class is pure python containing
#



if __name__ == "__main__":
    #Permutation encoding / minimize fitness
    config.set_solution_type('perm')
    config.set_minimize_fitness()

    #read instance data
    tsp_inst = tsp.TravelingSalesman("berlin52")
    #solution initializer
    tsp_init = initializer.random(tsp_inst.ncities)
    #full and incremental evaluation
    toureval = evaluator.fitness(tsp_inst.eval)
    tsp_nbor_eval = mo.eval.neighbor_eval(tsp_inst.eval_incremental)

    #move and neighborhood


    moves = tsp.TwoOptMove(tsp_inst.ncities)
    nhood = mo.neighborhood.ordered(len(moves.moves),stype='perm')

    sol = solution.empty()
    tsp_init(sol)
    toureval(sol)
    print(sol)

    # nbor = _core.NeighborPerm()
    # tsp_nbor_eval(sol,nbor)
    # print(nbor)


    # comp = lambda n1,n2 : n1 > n2

    hc = mo.algo.simple_hill_climber(
        nhood,
        toureval,
        tsp_nbor_eval,
        moves,
        mo.continuator.always_true()
    )


    # hc = mo.algo.simple_hill_climber(
    #     nhood,
    #     toureval,
    #     tsp_nbor_eval,
    #     mo.continuator.always_true(),
    #     mo.comparator.neighbor_compare(comp),
    #     mo.comparator.sol_neighbor_compare(comp)
    # )

    hc.setMove(moves)

    t1 = time.time()
    hc(sol)

    print(sol)
    print("elapsed:\t",time.time()-t1)
