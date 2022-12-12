from pyparadiseo import initializer,evaluator,config

from pyparadiseo import mo
from pyparadiseo.mo import algo
from pyparadiseo.mo import eval

import unittest
import inspect

import numpy as np

def eval_incremental(sol,nbor):
    if sol.encoding[nbor.index()]:
        return sol.fitness - 1
    else:
        return sol.fitness + 1

class test_moalgo(unittest.TestCase):
    def setUp(self):
        self.DIM = 20
        # initializer
        self.myinit = initializer.initializer(lambda : np.random.choice([True,False],self.DIM))
        # full evaluation
        self.myeval = evaluator.fitness(lambda x : np.sum(x))
        # nbor evaluation
        self.nborEval = mo.eval.neighbor_eval(eval_incremental)
        # neighborhood
        self.nhood = mo.neighborhood.ordered(self.DIM)

        self.hc = mo.algo.hill_climber(self.nhood,self.myeval,self.nborEval)
        config.set_maximize_fitness()


    def test_continuator(self):
        print(self.hc.get_continuator())
        self.hc.set_continuator(mo.continuator.max_iterations(20,True))
        print(self.hc.get_continuator())


    # def test_another(self):
    #     print(self.hc.get_continuator())
    #     self.hc.set_continuator(mo.continuator.always_true())
    #     print(self.hc.get_continuator())


if __name__ == '__main__':
    unittest.main()
