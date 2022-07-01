import sys
sys.path.append("..")

from problems import onemax

#provisoire
import pyparadiseo as pp
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
    tsp = tsp.TravelingSalesman("berlin52")

    tsp_init = initializer.random(tsp.ncities,stype='perm')
    toureval = evaluator.fitness(tsp.eval,stype='perm')
    tsp_nbor_eval = mo.eval.neighbor_eval(tsp.eval_incremental,stype='perm')

    sol = solution.empty(stype='perm')
    tsp_init(sol)
    toureval(sol)
    print(sol)


    nbor = _core.NeighborPerm()
    tsp_nbor_eval(sol,nbor)
    print(nbor)

    nhood = mo.neighborhood.ordered(len(tsp.moves),stype='perm')

    comp = lambda n1,n2 : n1 > n2

    hc = mo.algo.simple_hill_climber(
        nhood,
        toureval,
        tsp_nbor_eval,
        mo.continuator.always_true(stype='perm'),
        mo.comparator.neighbor_compare(comp,stype='perm'),
        mo.comparator.sol_neighbor_compare(comp,stype='perm'),
        stype='perm'
    )

    hc.setMove(tsp.move)

    t1 = time.time()
    hc(sol)

    print(sol)
    print("elapsed:\t",time.time()-t1)
