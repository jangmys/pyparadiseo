from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import config

from pyparadiseo import mo

from pyparadiseo.mo import algo
from pyparadiseo.mo import eval
from pyparadiseo.mo import sampling

import unittest
import inspect

import numpy as np



DIM = 10

#TODO : for now, just testing imports and base class name ...
class test_mosampling(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('real')

        self.nhood = mo.neighborhood.ordered(DIM)
        self.myeval = evaluator.fitness(lambda x : np.sum(x))
        self.nborEval = mo.eval.neighbor_full_eval(lambda x : np.sum(x))

        self.init = initializer.random(10)
        self.ls = mo.algo.hill_climber(self.nhood,self.myeval,self.nborEval)

        def move(nbor,sol):
            ind = nbor.index()

            if np.random.choice([0,1]):
                sol[ind] = sol[ind]*1.01
            else:
                sol[ind] = sol[ind]*0.99

        self.ls.set_move(move)


        pass
    def tearDown(self):
        pass
    def test_basic(self):
        sampler = sampling.moSampling(self.init,self.ls,stype='real')

        sampler()

        pass


if __name__ == '__main__':
    unittest.main()
