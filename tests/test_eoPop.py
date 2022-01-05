from pyparadiseo import PyEOT,Pop,Init

import unittest
import numpy as np

def isNonIncreasing(pop):
    for i in range(1,len(pop)):
        if pop[i-1].fitness < pop[i].fitness:
            return False
    return True

class Test_Pop(unittest.TestCase):
    def setUp(self):
        self.pop = Pop()
        self.init = Init(lambda : np.arange(10)) #just something

    def tearDown(self):
        self.pop.resize(0)
    def test_resize(self):
        self.assertEqual(len(self.pop),0)
        self.pop.resize(100)
        self.assertEqual(len(self.pop),100)

        for i,ind in enumerate(self.pop):
            ind.fitness = i
        self.pop.resize(10)
        self.assertEqual(len(self.pop),10)
        self.assertEqual(self.pop[9].fitness,9)
    def test_append(self):
        self.pop.append(10,self.init)
        self.assertEqual(len(self.pop),10)
        self.pop.append(20,self.init)
        self.assertEqual(len(self.pop),20)
        with self.assertRaises(RuntimeError):
            self.pop.append(10,self.init)
    def test_sort(self):
        self.pop.resize(20)
        # sort fails if fitness not set
        with self.assertRaises(TypeError):
            self.pop.sort()
        for i in self.pop:
            i.fitness = np.random.randint(100)
        self.pop.sort()
        self.assertTrue(isNonIncreasing(self.pop))
    def test_shuffle(self):
        self.pop.resize(100)
        for i,ind in enumerate(self.pop):
            ind.fitness = i
        self.pop.shuffle()
        self.assertFalse(isNonIncreasing(self.pop),"this is highly unlikely")
    def test_best(self):
        self.pop.resize(100)
        for i,ind in enumerate(self.pop):
            ind.fitness = i
        self.pop.shuffle()
        bestind = self.pop.best()
        self.assertEqual(bestind.fitness,99)
    def test_push_back(self):
        self.pop.append(10,self.init)
        ind = PyEOT()
        self.init(ind)
        ind.fitness = 42
        self.pop.push_back(ind)
        self.assertEqual(len(self.pop), 11)
        self.assertEqual(self.pop[10].fitness, 42)
    def test_swap(self):
        pop2 = Pop(10,self.init)
        self.assertEqual(len(pop2),10)
        pop2.swap(self.pop)
        self.assertEqual(len(self.pop),10)
        self.assertEqual(len(pop2),0)



if __name__ == '__main__':
    unittest.main()
