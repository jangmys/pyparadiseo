import sys
sys.path.append("..")

from problems import onemax

import pyparadiseo as pp

from pyparadiseo import mo
from pyparadiseo import initializer,evaluator

from pyparadiseo.mo import eval,neighborhood,algo,Eval,Neighbor

import time

import numpy as np

if __name__ == "__main__":
    DIM = 2000
    max_one = onemax.OneMax(DIM)

    #use provided initializer class for binary solutions
    myinit = pp.initializer.BinaryInit(DIM)
    #make pyparadiseo fitness evaluator (callable with appropriate signature...)
    myeval = pp.evaluator.FitnessEval(max_one.sum_bits)
    # nbor evaluation
    nborEval = pp.mo.eval.NeighborEval(max_one.eval_incremental)
    # neighborhood
    nhood = mo.neighborhood.OrderNeighborhood(DIM)
    rand_nhood = mo.neighborhood.RndWithoutReplNeighborhood(DIM)
    rand_nhood_repl = mo.neighborhood.RndWithReplNeighborhood(DIM)


    # algo
    hc = mo.algo.SimpleHC(nhood,myeval,nborEval)
    # set move
    hc.setMove(max_one.move)

    ########################################
    # define sol / init / eval
    sol = pp.Solution()
    myinit(sol)
    myeval(sol)
    # print(sol)

    t1 = time.time()
    hc(sol)

    print("simpleHC elapsed:\t",time.time()-t1)
    print(sol)
    #######################################

    #######################################
    hc = mo.algo.FirstImprHC(nhood,myeval,nborEval)
    # set move
    hc.setMove(max_one.move)

    # sol = pp.PyEOT()
    myinit(sol)
    myeval(sol)
    # print(sol)

    t1 = time.time()
    hc(sol)

    print("FirstImprHC elapsed:\t",time.time()-t1)
    print(sol)

    #######################################

    #######################################
    hc = mo.algo.RandomBestHC(nhood,myeval,nborEval)
    # set move
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("RandomBestHC elapsed:\t",time.time()-t1)
    print(sol)
    #######################################

    #######################################
    hc = mo.algo.NeutralHC(nhood,myeval,nborEval,2000)
    # set move
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("NeutralHC elapsed:\t",time.time()-t1)
    print(sol)
    #######################################

    #######################################
    hc = mo.algo.RandomSearch(myinit,myeval,1000)
    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)
    print("RandomSearch elapsed:\t",time.time()-t1)
    print(sol)
    #######################################

    #######################################
    hc = mo.algo.RandomWalk(rand_nhood,myeval,nborEval,1000)
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)
    print("RandomWalk elapsed:\t",time.time()-t1)
    print(sol)
    #######################################

    #######################################
    hc = mo.algo.RandomNeutralWalk(rand_nhood,myeval,nborEval,10000)
    # set move
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("RandomNeutralWalk elapsed:\t",time.time()-t1)
    print(sol)
    #######################################


    # hc = mo.algo.RandomWalk(nhood,myeval,nborEval,100)
    hc = mo.algo.MetropolisHasting(rand_nhood_repl,myeval,nborEval,10000)
    # set move
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("MetropolisHasting elapsed:\t",time.time()-t1)
    print(sol)



    # hc = mo.algo.RandomWalk(nhood,myeval,nborEval,100)
    hc = mo.algo.SA(rand_nhood_repl,myeval,nborEval,100,0.99)
    # set move
    hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("SA elapsed:\t",time.time()-t1)
    print(sol)



    # hc = mo.algo.RandomWalk(nhood,myeval,nborEval,100)
    hc = mo.algo.TS(nhood,myeval,nborEval,100,100)
    # set move
    hc.setMove(max_one.move)
    #
    myinit(sol)
    myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("SA elapsed:\t",time.time()-t1)
    # print(sol)



#
