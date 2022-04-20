import pyparadiseo as pp

from pyparadiseo import eo,initializer,bounds,evaluator
from pyparadiseo.eo import selector,continuator,algo
from pyparadiseo.operator import MonOp,QuadOp

import copy
import numpy as np

def norm2(ind):
    return np.linalg.norm(ind,2)

# mimic eo/tutorial/Lesson1/FirstRealGA.cpp
if __name__ == "__main__":
    VEC_SIZE = 500 # Number of object variables in genotypes
    POP_SIZE = 20 # Size of population
    T_SIZE = 3 # size for tournament selection
    MAX_GEN = 5000 # Maximum number of generation before STOP
    CROSS_RATE = 0.8 # Crossover rate
    EPSILON = 0.01 # range for real uniform mutation
    MUT_RATE = 0.5 # mutation rate

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)


    pp.set_minimize_fitness()

    myeval = pp.evaluator.FitnessEval(lambda sol: norm2(sol))

    myinit = pp.initializer.RealBoundedInit(bounds.RealVectorBounds(VEC_SIZE,-1,1))

    pop = pp.Pop(POP_SIZE,myinit)

    for ind in pop:
        myeval(ind)
    #     v = pp.Solution()
    #     v.encoding = np.random.uniform(-1.0,1.0,VEC_SIZE)
    #     pop.push_back(v)

    select = selector.DetTournamentSelect(3)
    xover = pp.operator.SegmentCrossover(0.0)
    mutat = pp.operator.UniformMutation(EPSILON,1.0)
    conti = pp.eo.continuator.GenContinue(MAX_GEN)

    sga = pp.eo.algo.SGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    sga(pop)

    print(pop.best())
