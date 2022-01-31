import pyparadiseo as pp

from pyparadiseo import eo
from pyparadiseo import Init
from pyparadiseo.eo import selector,continuator,algo
from pyparadiseo import MonOp,QuadOp

import copy
import numpy as np

# class BinaryRandomInit(Init):
#     def __init__(self,n_vars):
#         Init.__init__(self,lambda : np.random.randint(0,2,n_vars))
#
# class BinaryBitflipMutation(MonOp):
#     def __init__(self,_proba):
#         MonOp.__init__(self)
#         self._mProba = _proba
#     def __call__(self,ind) -> bool:
#         ind.invalidate()
#         #choose random bits to flip with 5% proba
#         flip = np.random.choice([0, 1], size=len(ind), p=[1.0-self._mProba, self._mProba])
#
#         for e,i in enumerate(flip):
#             if i:
#                 ind.encoding[e] = 1 - ind.encoding[e]
#         return True
#
# class BinaryOnepointCrossover(QuadOp):
#     def __init__(self):
#         QuadOp.__init__(self)
#     def __call__(self,ind1,ind2) -> bool:
#         tmp = copy.deepcopy(ind1.encoding)
#         point = np.random.randint(len(ind1.encoding))
#         ind1.encoding[point:] = ind2.encoding[point:]
#         ind2.encoding[point:] = tmp[point:]
#         return True
#
#
# class intSolution():
#     def __init__(self,len,lb,ub):
#         self.lb = lb
#         self.ub = ub
#         self.x = np.zeros(len)
#         for i,_ in enumerate(self.x):
#             self.x[i]=np.random.randint(self.lb[i],self.ub[i])
#     def __str__(self):
#         return str(self.x)
#
# def int_sol_generator(len,lb,ub):
#     x=intSolution(len,lb,ub)
#     return x

###
### negative because maximization is default
###
def norm2(ind):
    return -np.linalg.norm(ind,2)

# + str(self.lb) + str(self.ub)
    # def generate_int_sol(self):
    #     for i,_ in enumerate(self.x):
    #         self.x[i]=np.random(self.lb[i],self.ub[i])
    #     return self

# mimic eo/tutorial/Lesson1/FirstRealGA.cpp


if __name__ == "__main__":
    VEC_SIZE = 8 # Number of object variables in genotypes
    POP_SIZE = 20 # Size of population
    T_SIZE = 3 # size for tournament selection
    MAX_GEN = 500 # Maximum number of generation before STOP
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
    conti = pp.eo.continuator.GenContinue(MAX_GEN)]

    sga = pp.eo.algo.SGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    sga(pop)

    print(pop.best())
