import pyparadiseo as pp

from pyparadiseo import eo
from pyparadiseo import Init
from pyparadiseo.eo import selector,continuator,algo
from pyparadiseo import MonOp,QuadOp

import copy
import numpy as np

class BinaryRandomInit(Init):
    def __init__(self,n_vars):
        Init.__init__(self,lambda : np.random.randint(0,2,n_vars))

class BinaryBitflipMutation(MonOp):
    def __init__(self,_proba):
        MonOp.__init__(self)
        self._mProba = _proba
    def __call__(self,ind) -> bool:
        ind.invalidate()
        #choose random bits to flip with 5% proba
        flip = np.random.choice([0, 1], size=len(ind), p=[1.0-self._mProba, self._mProba])

        for e,i in enumerate(flip):
            if i:
                ind.encoding[e] = 1 - ind.encoding[e]
        return True

class BinaryOnepointCrossover(QuadOp):
    def __init__(self):
        QuadOp.__init__(self)
    def __call__(self,ind1,ind2) -> bool:
        tmp = copy.deepcopy(ind1.encoding)
        point = np.random.randint(len(ind1.encoding))
        ind1.encoding[point:] = ind2.encoding[point:]
        ind2.encoding[point:] = tmp[point:]
        return True


if __name__ == "__main__":
    myeval = pp.FitnessEval(lambda sol: np.sum(sol))

    sga = eo.algo.SGA(
        selector.DetTournamentSelect(),
        BinaryOnepointCrossover(),.2,
        BinaryBitflipMutation(.05),.5,
        myeval,
        continuator.GenContinue(100)
    )

    pop = pp.Pop(20, pp.get_init("binary",16))
    # pop = pp.Pop(20, BinaryRandomInit(16))
    for ind in pop:
        myeval(ind)

    # print(pop)
    sga(pop)
    print(pop.best())
