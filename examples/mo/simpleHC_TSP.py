import sys
sys.path.append("..")

from problems import tsp
from problems import onemax

import pyparadiseo as pp

from pyparadiseo import mo
from pyparadiseo.mo import continuator,comparator
from pyparadiseo.mo import eval,neighborhood,algo,Eval,Neighbor

import time

import numpy as np




if __name__ == "__main__":
    tsp = tsp.TravelingSalesman("berlin52")

    tsp_init = pp.initializer.PermutationInit(tsp.ncities)
    toureval = pp.FitnessEval(tsp.eval)

    sol = pp.Solution()
    tsp_init(sol)
    toureval(sol)
    print(sol)

    tsp_nbor_eval = mo.eval.NeighborEval(tsp.eval_incremental)

    nbor = mo.Neighbor()
    tsp_nbor_eval(sol,nbor)

    # print(nbor)

    nhood = mo.neighborhood.OrderNeighborhood(len(tsp.moves))

    comp = lambda n1,n2 : n1 > n2

    hc = mo.algo.SimpleHC(
        nhood,
        toureval,
        tsp_nbor_eval,
        mo.continuator.TrueContinuator(),
        mo.comparator.moNeighborComparator(comp),
        mo.comparator.moSolNeighborComparator(comp)
    )
    hc.setMove(tsp.move)

    t1 = time.time()
    hc(sol)

    print(sol)
    print("elapsed:\t",time.time()-t1)
