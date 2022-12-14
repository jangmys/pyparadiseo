## import problem
import sys
sys.path.append("..")
from problems import onemax

from pyparadiseo import config
from pyparadiseo import evaluator
import pyparadiseo as pp

from pyparadiseo import mo

import time

import numpy as np

if __name__ == "__main__":
    DIM = 10
    max_one = onemax.OneMax(DIM)

    config.set_solution_type("bin")

    # standard initializer object
    myinit = pp.initializer.random(DIM)
    # full evaluation
    myeval = evaluator.fitness(max_one.sum_bits)
    # nbor evaluation
    nborEval = pp.mo.eval.neighbor_eval(max_one.eval_incremental)
    # neighborhood
    nhood = mo.neighborhood.ordered(DIM)

    # algo
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move)

    ########################################
    # define sol / init / eval
    sol = pp.solution.empty()
    myinit(sol)
    myeval(sol)
    print(sol)

    t1 = time.time()
    hc(sol)
    print("elapsed:\t",time.time()-t1)

    print(sol)
#
