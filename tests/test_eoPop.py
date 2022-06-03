import pyparadiseo as pp

from pyparadiseo import config
from pyparadiseo import population
from pyparadiseo import solution

from pyparadiseo import initializer
from pyparadiseo import bounds

import unittest
import numpy as np

def isNonIncreasing(pop):
    for i in range(1,len(pop)):
        if pop[i-1].fitness < pop[i].fitness:
            return False
    return True


class Test_Pop(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')
        config.set_maximize_fitness()

        self.pop = population.empty()
        self.binpop = population.empty(stype='bin')
        self.realpop = population.empty(stype='real')

        self.init = initializer.make_initializer(lambda : np.arange(10)) #just something
        self.bininit = initializer.random(10,'bin')
        self.realinit = initializer.random(stype='real',bounds=bounds.RealVectorBounds(10,-1,1))

    def tearDown(self):
        self.pop.resize(0)
        self.binpop.resize(0)
        self.realpop.resize(0)

    def test_from_init(self):
        def t_from_init(init):
            pop = population.from_init(10,init)
            self.assertEqual(len(pop),10)
        t_from_init(self.init)
        t_from_init(self.bininit)
        t_from_init(self.realinit)

    def test_resize(self):
        def t_resize(pop):
            self.assertEqual(len(pop),0)
            pop.resize(100)
            self.assertEqual(len(pop),100)

            for i,ind in enumerate(pop):
                ind.fitness = i

            pop.resize(10)
            self.assertEqual(len(pop),10)
            self.assertEqual(pop[9].fitness,9)

        t_resize(self.pop)
        t_resize(self.binpop)
        t_resize(self.realpop)

    def test_append(self):
        def t_append(pop,init):
            pop.append(10,init)
            self.assertEqual(len(pop),10)
            pop.append(20,init)
            self.assertEqual(len(pop),20)
            with self.assertRaises(RuntimeError):
                pop.append(10,init)
        t_append(self.pop,self.init)
        t_append(self.binpop,self.bininit)
        t_append(self.realpop,self.realinit)

    def test_sort(self):
        def t_sort(pop):
            pop.resize(20)
            # sort fails if fitness not set
            with self.assertRaises(RuntimeError):
                pop.sort()
            for i in pop:
                i.fitness = np.random.randint(100)

            pop.sort()
            self.assertTrue(isNonIncreasing(pop),"maximize : sorted pop should be non-increasing")
        t_sort(self.pop)
        t_sort(self.binpop)
        t_sort(self.realpop)

    def test_shuffle(self):
        self.pop.resize(100)
        for i,ind in enumerate(self.pop):
            ind.fitness = i
        self.pop.shuffle()
        self.assertFalse(isNonIncreasing(self.pop),"shuffled pop is sorted. this is highly unlikely.")

    def test_best_worst(self):
        self.pop.resize(100)
        for i,ind in enumerate(self.pop):
            ind.fitness = i
        self.pop.shuffle()

        self.assertEqual(self.pop.best().fitness,99,"maximize : 99.0 is best")
        self.assertEqual(self.pop.worst().fitness,0,"maximize : 0.0 is worst")

        pp.config.set_minimize_fitness()
        self.assertEqual(self.pop.best().fitness,0,"minimize : 0.0 is best")
        self.assertEqual(self.pop.worst().fitness,99,"minimize : 99.0 is worst")


    def test_push_back(self):
        self.pop.append(10,self.init)
        ind = solution.empty()
        self.init(ind)
        ind.fitness = 42
        self.pop.push_back(ind)
        self.assertEqual(len(self.pop), 11)
        self.assertEqual(self.pop[10].fitness, 42)

    def test_swap(self):
        def t_swap(pop,init):
            pop2 = population.from_init(10,init)
            self.assertEqual(len(pop2),10)
            for i,ind in enumerate(pop2):
                ind.fitness = i

            pop2.swap(pop)
            self.assertEqual(len(pop),10)
            for i,ind in enumerate(pop):
                self.assertEqual(ind.fitness,i)

            self.assertEqual(len(pop2),0)
        t_swap(self.pop,self.init)
        t_swap(self.binpop,self.bininit)
        t_swap(self.realpop,self.realinit)

if __name__ == '__main__':
    unittest.main()
