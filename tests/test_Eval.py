#ABC
from pyparadiseo._core import eoEvalFunc

from pyparadiseo import config
from pyparadiseo import solution,population

from pyparadiseo import evaluator
from pyparadiseo import initializer

import time
import functools

import unittest

import numpy as np
# import numba as nb

class test_eval(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')

        self.sol = solution.solution(np.arange(10))

        self.pop = population.from_init(5,initializer.initializer(lambda : np.zeros(5,dtype=int)))

        for i in range(4):
            self.pop[i]=solution.solution(np.zeros(5,dtype=int))
            for j in range(1,2+i):
                self.pop[i][j]=1



    def test_lambda(self):
        ev = evaluator.fitness(lambda x: np.sum(x))
        self.assertTrue(isinstance(ev,eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_function(self):
        # take encoding as argument return fitness
        def foo(x):
            return np.sum(x)
        ev = evaluator.fitness(foo)
        self.assertTrue(isinstance(ev,eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_inherit(self):
        class myEval(eoEvalFunc):
            def __init__(self):
                eoEvalFunc.__init__(self)
            def __call__(self, sol):
                # take solution as argument, set fitness
                sol.fitness = np.sum(sol.encoding)

        ev = myEval()
        self.assertTrue(isinstance(ev,eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_njit(self):
        # @nb.njit
        def foo(x):
            s = 0
            for i in x:
                s += i
            return s
        ev = evaluator.fitness(foo)
        self.assertTrue(isinstance(ev,eoEvalFunc))
        ev(self.sol)
        self.assertEqual(self.sol.fitness,45)

    def test_popEval_from_fitness(self):
        # p = population.from_init(5,initializer.initializer(lambda : np.zeros(5,dtype=int)))
        #
        # for i in range(4):
        #     p[i]=solution.solution(np.zeros(5,dtype=int))
        #     for j in range(1,2+i):
        #         p[i][j]=1

        popeval = evaluator.pop_eval_from_fitness(evaluator.fitness(lambda x: np.sum(x)))
        popeval(self.pop,self.pop)
        for i in range(4):
            self.assertEqual(self.pop[i].fitness,i+1)


    def test_pypopeval(self):
        class popeval():
            def __init__(self):
                self.func = lambda x: np.sum(x)
            def __call__(self,pop1,pop2):
                for ind in pop2:
                    ind.fitness = self.func(ind.encoding)

        from pyparadiseo._core import pyPopEval

        f = pyPopEval(popeval())

        f(self.pop,self.pop)

        for i in range(4):
            self.assertEqual(self.pop[i].fitness,i+1)





if __name__ == "__main__":
    unittest.main()
