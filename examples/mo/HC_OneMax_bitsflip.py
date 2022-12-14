import sys
sys.path.append(".")

from pyparadiseo import config

from problems import onemax

import pyparadiseo as pp

from pyparadiseo import initializer,evaluator
from pyparadiseo import mo
from pyparadiseo import eo
from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import solution



from pyparadiseo.mo import eval,neighborhood,algo

import time

import numpy as np
import inspect


class rand_move():
    def __init__(self,n,m):
        self.nvar = n
        self.ntoflip = m
        self.to_flip = np.zeros(n,dtype=int)
        self.to_flip[:self.ntoflip]=1


    def eval_incremental(self,nbor,sol):
        """
        incremental eval
        """
        f = sol.fitness

        np.random.shuffle(self.to_flip)




        if sol.encoding[nbor.index()]:
            return sol.fitness - 1
        else:
            return sol.fitness + 1



    def move(self,nbor,sol):
        """flip [0,k] bits"""
        for i,e in enumerate(self.to_flip):
            if e:
                sol[i] = 1 - sol[i]

        i,j=self.moves[nbor.index()]
        while i<j:
            sol.array[i],sol.array[j]=sol.array[j],sol.array[i]
            i += 1
            j -= 1
        return sol


if __name__ == "__main__":
    DIM = 2000
    max_one = onemax.OneMax(DIM,42.0)

    config.set_solution_type('bin')

    #use initializer class for binary solutions
    myinit = initializer.random(DIM)
    #make pyparadiseo fitness evaluator (callable with appropriate signature...)
    myeval = pp.evaluator.fitness(max_one.sum_bits)
    # myeval = max_one.sum_bits

    # print("#"*20)
    # print("myeval : ",myeval)

    sol = solution.empty('bin')
    myinit(sol)
    # max_one.feval(sol)
    myeval(sol)

    # nbor evaluation
    # nborEval = onemax.inherit_incr_eval()
    nborEval = mo.eval.neighbor_eval(max_one.eval_incremental)

    # # neighborhood
    nhood = mo.neighborhood.ordered(DIM)
    rand_nhood = mo.neighborhood.random(neighborhood_size=DIM,with_replacement=False)
    rand_nhood_repl = mo.neighborhood.random(neighborhood_size=DIM,with_replacement=True)


    # algo
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,hc_type='simple')
    # set move
    # hc.setMove(max_one.move)

    # ########################################
    # define sol / init / eval
    sol = solution.empty('bin')
    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("simpleHC elapsed:\t",time.time()-t1)
    print(sol.fitness)
    print("#"*20)
    # #######################################
    #
    # #######################################
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,hc_type='first_improve')
    # hc = mo.algo.FirstImprHC(nhood,myeval,nborEval)
    # set move
    # hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("FirstImprHC elapsed:\t",time.time()-t1)
    print(sol.fitness)
    print("#"*20)
    # #######################################
    #
    # #######################################
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,hc_type='random_best')
    # hc = mo.algo.RandomBestHC(nhood,myeval,nborEval)
    # set move
    # hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("RandomBestHC elapsed:\t",time.time()-t1)
    print(sol.fitness)
    print("#"*20)
    # #######################################
    #
    # #######################################
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,nneutral_steps=2000,hc_type='neutral')

    # mo.algo._NeutralHC(nhood,myeval,nborEval,2000)
    # set move
    # hc.setMove(max_one.move)

    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)

    print("NeutralHC elapsed:\t",time.time()-t1)
    print(sol.fitness)
    print("#"*20)
    # #######################################
    #
    # #######################################
    hc = mo.algo.random_search(myinit,myeval,1000)
    myinit(sol)
    myeval(sol)

    t1 = time.time()
    hc(sol)
    print("RandomSearch elapsed:\t",time.time()-t1)
    print(sol.fitness)
    print("#"*20)
    # #######################################
    #
    # #######################################
    # hc = mo.algo.random_walk(rand_nhood,myeval,nborEval,1000)
    # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    # print("RandomWalk elapsed:\t",time.time()-t1)
    # # print(sol)
    # print("#"*20)
    # # #######################################
    # #
    # # #######################################
    # hc = mo.algo.random_neutral_walk(rand_nhood,myeval,nborEval,1000)
    #
    # # hc = mo.algo.RandomNeutralWalk(rand_nhood,myeval,nborEval,10000)
    # # set move
    # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("RandomNeutralWalk elapsed:\t",time.time()-t1)
    # # print(sol)
    # print("#"*20)
    # # #######################################
    # hc = mo.algo.metropolis_hastings(rand_nhood_repl,myeval,nborEval,10000)
    # # set move
    # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("MetropolisHasting elapsed:\t",time.time()-t1)
    # # print(sol)
    # print("#"*20)
    # ####################################
    # hc = mo.algo.simulated_annealing(rand_nhood_repl,myeval,nborEval,100,0.99)
    # # set move
    # hc.setMove(max_one.move)
    # #
    # myinit(sol)
    # myeval(sol)
    # #
    # t1 = time.time()
    # hc(sol)
    #
    # print("SA elapsed:\t",time.time()-t1)
    # # print(sol)
    # print("#"*20)
    ####################################
    # hc = mo.algo.tabu_search(nhood,myeval,nborEval,100,100)
    # # set move
    # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # # t1 = time.time()
    # hc(sol)
    # #
    # # print("SA elapsed:\t",time.time()-t1)
    # # print(sol)
    # print("#"*20)


#
