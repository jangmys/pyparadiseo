from pyparadiseo import Solution,Pop

from pyparadiseo import evaluator
from pyparadiseo import initializer

import time
import functools

import unittest

import numpy as np
import numba as nb

class test_eval(unittest.TestCase):
    def setUp(self):
        self.sol = Solution(np.arange(10))

    def test_lambda(self):
        ev = evaluator.FitnessEval(lambda x: np.sum(x))
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_function(self):
        # take encoding as argument return fitness
        def foo(x):
            return np.sum(x)
        ev = evaluator.FitnessEval(foo)
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
        ev = evaluator.FitnessEval(foo)
        self.assertTrue(isinstance(ev,evaluator.eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_popEval(self):
        from pyparadiseo import Pop,Solution
        from pyparadiseo.initializer import Init

        p = Pop(5,Init(lambda : np.zeros(5,dtype=int)))
        print(p)

        for i in range(4):
            p[i]=Solution(np.zeros(5,dtype=int))
            for j in range(1,2+i):
                p[i][j]=1

        print(p)

        popeval = evaluator.PopLoopEval(evaluator.FitnessEval(lambda x: np.sum(x)))
        popeval(p,p)
        for i in range(4):
            self.assertEqual(p[i].fitness,i+1)
        print(p)



if __name__ == "__main__":
    unittest.main()
