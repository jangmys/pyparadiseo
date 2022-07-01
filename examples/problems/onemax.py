import numpy as np

from pyparadiseo._core import moEvalBin
from pyparadiseo import decorators
from pyparadiseo import evaluator

import numba

@numba.njit
def eval_increm(sol_encoding,sol_fitness,nbor_index):
    if sol_encoding[nbor_index]:
        return sol_fitness - 1
    else:
        return sol_fitness + 1

class inherit_incr_eval(moEvalBin):
    def __init__(self,fun):
        moEvalBin.__init__(self)
        self.fun = fun
    def __call__(self,sol,neighbor):
        neighbor.fitness = self.fun(sol.array,sol.fitness,neighbor.index())


class OneMax():
    def __init__(self,dim,value):
        self.dim = dim
        self.value = value

    def sum_bits(self,sol) -> float:
        return float(np.count_nonzero(sol))+self.value

    def eval_incremental(self,sol_encoding,sol_fitness,nbor_index):
        if sol_encoding[nbor_index]:
            return sol_fitness - 1
        else:
            return sol_fitness + 1

    def move(self,nbor,sol):
        ind = nbor.index()
        if sol.array[ind]:
            sol.array[ind] = False
        else:
            sol.array[ind] = True
