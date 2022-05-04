import pyparadiseo as pp

from pyparadiseo import mo
from pyparadiseo import config
from pyparadiseo.mo import eval,neighborhood,algo

from pyparadiseo import Pop
from pyparadiseo.initializer import Init
from pyparadiseo.eo import selector

import numpy as np
import unittest
import inspect

def sum_bits(sol):
    return np.count_nonzero(sol)


def eval_incremental(sol_enc,sol_fit,index) -> float:
    if sol_enc[index]:
        return sol_fit - 1
    else:
        return sol_fit + 1



class test_simpleHC_onemax(unittest.TestCase):
    def setUp(self):
        self.DIM = 20
        # initializer
        self.myinit = pp.initializer.Init(lambda : np.random.choice([True,False],self.DIM))
        # self.myinit = pp.initializer.BinaryInit(self.DIM)
        # full evaluation
        self.myeval = pp.evaluator.FitnessEval(sum_bits)
        # nbor evaluation
        self.nborEval = pp.mo.eval.NeighborEval(eval_incremental)
        # neighborhood
        self.nhood = mo.neighborhood.OrderNeighborhood(self.DIM)

        pp.config.set_maximize_fitness()

    def move(self,nbor,sol):
        ind = nbor.index()
        if sol[ind]:
            sol[ind] = False
        else:
            sol[ind] = True


    def test_onemax(self):
        """
        run simple hill climber on OneMax. expect to find global optimum [True ... True]
        """
        # algo
        hc = mo.algo.SimpleHC(self.nhood,self.myeval,self.nborEval)
        # set move
        hc.setMove(self.move)

        # define sol / init / eval
        sol = pp.Solution()
        self.myinit(sol)
        self.myeval(sol)

        print(sol)

        # run algo
        hc(sol)

        print(sol)

        self.assertTrue(np.all(sol))
        self.assertAlmostEqual(sol.fitness, self.DIM)




if __name__ == "__main__":
    unittest.main()
