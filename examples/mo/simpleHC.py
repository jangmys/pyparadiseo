import pyparadiseo as pp

from pyparadiseo import mo
from pyparadiseo.mo import eval

import numpy as np
import numba as nb

@nb.njit
def sum_bits(sol):
    return np.sum(sol)


def incr_eval(sol,neigh):
    if sol.encoding[neigh.index()] == 0:
        neigh.fitness = sol.fitness + 1
    else:
        neigh.fitness = sol.fitness - 1


if __name__ == "__main__":
    DIM = 20

    myinit = pp.core.BinaryInit(DIM)
    myeval = pp.FitnessEval(sum_bits)


    myMoEval = mo.eval.NeighborEval(incr_eval)
