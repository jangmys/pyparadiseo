import pyparadiseo as pp

from pyparadiseo import eo
from pyparadiseo.eo import selector,continuator,algo
from pyparadiseo import initializer,evaluator,operator


import copy
import numpy as np

def mut(ind) -> bool:
    #choose random bits to flip with 5% proba
    flip = np.random.choice([0, 1], size=len(ind), p=[.95, .05])

    for e,i in enumerate(flip):
        if i:
            ind[e] = 1 - ind[e]
    return True


def cross(ind1,ind2):
    tmp = copy.deepcopy(ind1)

    point = np.random.randint(len(ind1))

    ind1[point:] = ind2[point:]
    ind2[point:] = tmp[point:]

    return True


if __name__ == "__main__":
    myinit = initializer.Init(lambda : np.random.randint(0,2,16))
    myeval = evaluator.fitness(lambda sol: np.sum(sol))

    sga = eo.algo.SGA(
        selector.DetTournamentSelect(),
        operator.pyQuadOp(cross),0.2,
        operator.pyMonOp(mut),0.5,
        evaluator.fitness(lambda sol: np.sum(sol)),
        continuator.GenContinue(100)
    )

    pop = pp.Pop(20, myinit)
    for ind in pop:
        myeval(ind)

    # print(pop)
    sga(pop)
    print(pop.best())
