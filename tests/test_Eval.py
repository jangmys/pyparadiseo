from pyparadiseo import config
from pyparadiseo import solution,pop


from pyparadiseo import Pop

from pyparadiseo import evaluator
from pyparadiseo import initializer

import time
import functools

import unittest

import numpy as np
import numba as nb

class test_eval(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')

        self.sol = solution.from_object(np.arange(10))

    def test_lambda(self):
        ev = evaluator.fitness(lambda x: np.sum(x))
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_function(self):
        # take encoding as argument return fitness
        def foo(x):
            return np.sum(x)
        ev = evaluator.fitness(foo)
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_inherit(self):
        class myEval(evaluator.eoEvalFunc):
            def __init__(self):
                evaluator.eoEvalFunc.__init__(self)
            def __call__(self, sol):
                # take solution as argument, set fitness
                sol.fitness = np.sum(sol.encoding)

        ev = myEval()
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_njit(self):
        @nb.njit
        def foo(x):
            s = 0
            for i in x:
                s += i
            return s
        ev = evaluator.fitness(foo)
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_popEval(self):
        p = pop.from_init(5,initializer.make_initializer(lambda : np.zeros(5,dtype=int)))

        for i in range(4):
            p[i]=solution.from_object(np.zeros(5,dtype=int))
            for j in range(1,2+i):
                p[i][j]=1

        popeval = evaluator.PopLoopEval(evaluator.fitness(lambda x: np.sum(x)))
        popeval(p,p)
        for i in range(4):
            self.assertEqual(p[i].fitness,i+1)



if __name__ == "__main__":
    unittest.main()
