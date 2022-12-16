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


class bitsflip():
    def __init__(self,n,m,max_nbor):
        self.nvar = n
        self.ntoflip = m
        self.maxNeighbors = max_nbor
        self.index = 0

        self.to_flip = np.zeros((max_nbor,n),dtype=int)
        for a in self.to_flip:
            a[:self.ntoflip]=1

    def has_nbor(self,sol):
        return (self.maxNeighbors > 0)

    def init_nhood(self,sol,nbor):
        self.index = 0
        nbor.index(0)

    def next_nhood(self,sol,nbor):
        self.index += 1
        nbor.index(self.index)

    def cont_nhood(self,sol):
        return (self.index < self.maxNeighbors - 1 )

    def eval_incremental(self,sol,nbor):
        """
        incremental eval
        """
        f = sol.fitness

        for i,flip in enumerate(self.to_flip[nbor.index()]):
            if flip:
                if sol[i]:
                    f = f - 1
                else:
                    f = f + 1

        return f

    def move(self,nbor,sol):
        """flip [0,k] bits"""
        for i,e in enumerate(self.to_flip[nbor.index()]):
            if e:
                sol[i] = 1 - sol[i]
        return sol



if __name__ == "__main__":
    DIM = 2000
    max_one = onemax.OneMax(DIM,42.0)

    config.set_solution_type('bin')

    b= bitsflip(DIM,5,10)

    nhood = mo.neighborhood.neighborhood(b.init_nhood,b.next_nhood,b.cont_nhood,b.has_nbor,is_random=False)

    # #use initializer class for binary solutions
    myinit = initializer.random(DIM)
    # #make pyparadiseo fitness evaluator (callable with appropriate signature...)
    myeval = pp.evaluator.fitness(max_one.sum_bits)

    # nbor evaluation
    nborEval = mo.eval.neighbor_eval(b.eval_incremental)

    class mycont(mo.continuator.moContinuatorBin):
        def __init__(self,max_iter):
            mo.continuator.moContinuator.__init__(self)
            self.counter = 0
            self.max_iter = max_iter
        def __call__(self,sol):
            print(sol)
            self.counter += 1
            return self.counter < self.max_iter

    cont = mycont(1000)

    # if isinstance(cont,)



            # continuator=mycont(1000),

    # algo
    hc = mo.algo.hill_climber(nhood,myeval,nborEval,b.move,cont,hc_type='simple')
    # # set move
    # # hc.setMove(max_one.move)
    #
    # # ########################################
    # # define sol / init / eval
    sol = solution.empty('bin')
    myinit(sol)
    myeval(sol)
    #
    # t1 = time.time()
    hc(sol)
    #
    # print("simpleHC elapsed:\t",time.time()-t1)
    print(sol.fitness)
    # print("#"*20)
    # # #######################################
    # #
    # # #######################################
    # hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,hc_type='first_improve')
    # # hc = mo.algo.FirstImprHC(nhood,myeval,nborEval)
    # # set move
    # # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("FirstImprHC elapsed:\t",time.time()-t1)
    # print(sol.fitness)
    # print("#"*20)
    # # #######################################
    # #
    # # #######################################
    # hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,hc_type='random_best')
    # # hc = mo.algo.RandomBestHC(nhood,myeval,nborEval)
    # # set move
    # # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("RandomBestHC elapsed:\t",time.time()-t1)
    # print(sol.fitness)
    # print("#"*20)
    # # #######################################
    # #
    # # #######################################
    # hc = mo.algo.hill_climber(nhood,myeval,nborEval,max_one.move,nneutral_steps=2000,hc_type='neutral')
    #
    # # mo.algo._NeutralHC(nhood,myeval,nborEval,2000)
    # # set move
    # # hc.setMove(max_one.move)
    #
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    #
    # print("NeutralHC elapsed:\t",time.time()-t1)
    # print(sol.fitness)
    # print("#"*20)
    # # #######################################
    # #
    # # #######################################
    # hc = mo.algo.random_search(myinit,myeval,1000)
    # myinit(sol)
    # myeval(sol)
    #
    # t1 = time.time()
    # hc(sol)
    # print("RandomSearch elapsed:\t",time.time()-t1)
    # print(sol.fitness)
    # print("#"*20)
    # #######################################
