from pyparadiseo import PyEOT

import unittest

class TestEOT(unittest.TestCase):
    def test_ctor(self):
        sol = PyEOT()
        self.assertEqual(sol.fitness,None)

    def test_encoding(self):
        sol = PyEOT()
        sol.encoding = [1,2,3.0]
        self.assertEqual(sol.encoding,[1,2,3.0])
        self.assertEqual(sol[0],1)
        self.assertEqual(sol[1],2)
        self.assertEqual(sol[2],3.0)
        sol[0]=42
        self.assertEqual(sol[0],42)

    def test_setFitness(self):
        sol = PyEOT()
        self.assertEqual(sol.fitness,None)
        sol.fitness = 1.11
        self.assertEqual(sol.fitness,1.11)
        sol.fitness = 42
        self.assertEqual(sol.fitness,42)

    def test_setDiversity(self):
        sol = PyEOT()
        sol.diversity = 1.11
        self.assertEqual(sol.diversity,1.11)
        sol.diversity = 1
        self.assertEqual(sol.diversity,1)

    def test_objectiveVector(self):
        sol = PyEOT()
        sol.objectiveVector = [1.1,2.2]
        self.assertEqual(sol.objectiveVector[0],1.1)
        self.assertEqual(sol.objectiveVector[1],2.2)

    def test_invalidate(self):
        sol = PyEOT()
        sol.objectiveVector = [1.1,2.2]
        sol.invalidateObjectiveVector()
        self.assertTrue(sol.invalidObjectiveVector())

    def test_copy_ctor(self):
        sol = PyEOT()
        sol.encoding = [1,2,3,4]
        sol.fitness = 42.0
        sol.objectiveVector = [1.1,2.2]
        sol.diversity = 4.2

        sol2 = PyEOT(sol)
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        sol.encoding = [0]
        sol.fitness = 2.0
        sol.objectiveVector = [0.1,0.2]
        sol.diversity = 2.2

        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    def test_pickle(self):
        sol = PyEOT()
        sol.encoding = [1,2,3,4]
        sol.fitness = 42.0
        sol.objectiveVector = [1.1,2.2]
        sol.diversity = 4.2

        from copy import copy,deepcopy

        sol2 = PyEOT()
        sol2 = copy(sol)

        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        sol.encoding = [0]
        sol.invalidate()

        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        sol3 = copy(sol2)
        sol2.encoding = [0]
        sol2.invalidate()

        self.assertEqual(sol3.encoding,[1,2,3,4])
        self.assertEqual(sol3.fitness,42.0)
        self.assertEqual(sol3.objectiveVector[0],1.1)
        self.assertEqual(sol3.objectiveVector[1],2.2)
        self.assertEqual(sol3.diversity,4.2)

if __name__ == '__main__':
    unittest.main()
