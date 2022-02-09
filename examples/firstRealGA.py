import pyparadiseo as pp

from pyparadiseo import eo
from pyparadiseo import Init
from pyparadiseo.eo import selector,continuator,algo
from pyparadiseo import MonOp,QuadOp

import copy
import numpy as np

###
### negative because maximization is default
###
def norm2(ind):
    return -np.linalg.norm(ind,2)

# mimic eo/tutorial/Lesson1/FirstRealGA.cpp
if __name__ == "__main__":
    VEC_SIZE = 500 # Number of object variables in genotypes
    POP_SIZE = 20 # Size of population
    T_SIZE = 3 # size for tournament selection
    MAX_GEN = 1000 # Maximum number of generation before STOP
    CROSS_RATE = 0.8 # Crossover rate
    EPSILON = 0.01 # range for real uniform mutation
    MUT_RATE = 0.5 # mutation rate

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)
    # ...
    # np.random.seed(42)

    myeval = pp.FitnessEval(lambda sol: norm2(sol))

    pop = pp.Pop()

    for i in range(POP_SIZE):
        v = pp.PyEOT()
        v.encoding = np.random.uniform(-1.0,1.0,VEC_SIZE)
        myeval(v)
        pop.push_back(v)

    select = selector.DetTournamentSelect(3)
    xover = pp.core.SegmentCrossover(0.0)
    mutat = pp.core.UniformMutation(EPSILON,1.0)
    conti = pp.eo.continuator.GenContinue(MAX_GEN)

    sga = pp.eo.algo.SGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    sga(pop)

    print(pop.best())
