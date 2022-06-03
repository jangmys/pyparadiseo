import pyparadiseo as pp


from pyparadiseo import config
from pyparadiseo import evaluator
from pyparadiseo import population
from pyparadiseo import operator

from pyparadiseo import eo,initializer,bounds
from pyparadiseo.eo import select_one,continuator,algo
from pyparadiseo.operator import MonOp,QuadOp

import time
import copy
import numpy as np

def norm2(ind):
    return np.linalg.norm(ind,2)

VEC_SIZE = 500 # Number of object variables in genotypes
POP_SIZE = 20 # Size of population
T_SIZE = 3 # size for tournament selection
MAX_GEN = 5000 # Maximum number of generation before STOP
CROSS_RATE = 0.8 # Crossover rate
EPSILON = 0.1 # range for real uniform mutation
MUT_RATE = 0.5 # mutation rate

def realGA_generic_sol():
    config.set_solution_type('gen')

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)

    config.set_minimize_fitness()

    # myeval = pp.evaluator.EvalFuncProxy(lambda sol: norm2(sol))
    myeval = pp.evaluator.FitnessEval(lambda sol: norm2(sol))

    mybounds=bounds.RealVectorBounds(VEC_SIZE,-1,1)
    myinit = pp.initializer.RealBoundedInit(mybounds)

    # print(type(myinit))

    p = population.from_init(POP_SIZE,myinit)
    # pop = pp.Pop(POP_SIZE,myinit)

    for ind in p:
        myeval(ind)

    # print(p)

    select = select_one.det_tournament(3)

    # print(select(p))
    # print(select(p))
    # print(select(p))
    # print('-'*10)

    # select = selector.DetTournamentSelect(3)
    xover = pp.operator.SegmentCrossover(0.0)
    # mutat = pp.operator.UniformMutation(mybounds,EPSILON,1.0)
    mutat = pp.operator.UniformMutation(bounds=mybounds,epsilon=EPSILON,p_change=1.0)
    conti = pp.eo.continuator.max_generations(MAX_GEN)

    sga = pp.eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    t1=time.time()
    sga(p)
    t=time.time()-t1

    # print(p)
    print(p.best().fitness)
    print("Time: ",t)


def realGA_real_sol():
    config.set_solution_type('real')

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)

    config.set_minimize_fitness()

    # myeval = pp.evaluator.EvalFuncProxy(lambda sol: norm2(sol),"real")
    myeval = pp.evaluator.fitness(lambda sol: norm2(sol),"real")

    mybounds = bounds.RealVectorBounds(VEC_SIZE,-1,1)

    myinit = initializer.random(stype='real',bounds=mybounds)
    #
    print(type(myinit))

    #
    p = population.from_init(POP_SIZE,myinit,stype='real')
    print(type(p[0]))
    #
    for ind in p:
        myeval(ind)

    # print(p)

    select = select_one.det_tournament(3)

    # print(select(p))
    # print(select(p))
    # print(select(p))
    # print('-'*10)

    # select = selector._DetTournamentSelect(3)
    xover = operator.SegmentCrossoverReal(0.0)
    mutat = operator.UniformMutationReal(bounds=mybounds,epsilon=EPSILON,p_change=1.0)

    # print(type(mutat))

    # for ind in p:
    #     mutat(ind)
    #     print(ind)
    # print(">"*10)


    conti = pp.eo.continuator.max_generations(MAX_GEN)
    #
    sga = pp.eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)
    #
    t1=time.time()
    sga(p)
    t=time.time()-t1
    #

    # print(p)
    print(p.best().fitness)
    print("Time: ",t)



# mimic eo/tutorial/Lesson1/FirstRealGA.cpp
if __name__ == "__main__":
    realGA_real_sol()
    print("#"*20)
    realGA_generic_sol()
