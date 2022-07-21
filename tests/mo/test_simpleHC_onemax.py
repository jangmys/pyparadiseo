from pyparadiseo import config

from pyparadiseo import mo

from pyparadiseo import solution
from pyparadiseo import evaluator
from pyparadiseo import initializer

import numpy as np
import unittest

def sum_bits(sol):
    return np.count_nonzero(sol)

def eval_incremental(sol,nbor):
    if sol.encoding[nbor.index()]:
        return sol.fitness - 1
    else:
        return sol.fitness + 1

class test_simpleHC_onemax(unittest.TestCase):
    def setUp(self):
        self.DIM = 20
        # initializer
        self.myinit = initializer.initializer(lambda : np.random.choice([True,False],self.DIM))
        # full evaluation
        self.myeval = evaluator.fitness(sum_bits)
        # nbor evaluation
        self.nborEval = mo.eval.neighbor_eval(eval_incremental)
        # neighborhood
        self.nhood = mo.neighborhood.ordered(self.DIM)

        config.set_maximize_fitness()


    def test_onemax(self):
        """
        run simple hill climber on OneMax. expect to find global optimum [True ... True]
        """
        # from pyparadiseo import _core
        #
        # #assemble algo
        hc = mo.algo.simple_hill_climber(self.nhood,self.myeval,self.nborEval)
        # #

        #when running under pytest defining this move globally, causes segfault on shutdown (even though all tests pass)
        def move(nbor,sol):
            ind = nbor.index()
            if sol[ind]:
                sol[ind] = False
            else:
                sol[ind] = True

        hc.set_move(move)

        # # # define sol / init / eval
        sol = solution.empty()
        self.myinit(sol)
        #
        self.myeval(sol)
        print(sol)
        #
        # run algo
        hc(sol)
        #
        print(sol)
        self.assertTrue(np.all(sol))
        self.assertAlmostEqual(sol.fitness, self.DIM)
        #
        # del(hc)


if __name__ == "__main__":
    unittest.main()
