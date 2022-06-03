import sys
sys.path.append("..")

from problems import onemax

from pyparadiseo import evaluator
import pyparadiseo as pp

from pyparadiseo import mo
from pyparadiseo.mo import eval,neighborhood,algo,Eval,Neighbor

import time

import numpy as np

if __name__ == "__main__":
    DIM = 200
    max_one = onemax.OneMax(DIM)

    # standard initializer object
    myinit = pp.initializer.BinaryInit(DIM)
    # full evaluation
    myeval = evaluator.fitness(max_one.sum_bits)
    # nbor evaluation
    nborEval = pp.mo.eval.NeighborEval(max_one.eval_incremental)
    # neighborhood
    nhood = mo.neighborhood.OrderNeighborhood(DIM)

    # algo
    hc = mo.algo.SimpleHC(nhood,myeval,nborEval)
    # set move
    hc.setMove(max_one.move)

    ########################################
    # define sol / init / eval
    sol = pp.Solution()
    myinit(sol)
    myeval(sol)
    print(sol)

    t1 = time.time()
    hc(sol)
    print("elapsed:\t",time.time()-t1)

    print(sol)
#
