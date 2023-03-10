import pyparadiseo as pp

from pyparadiseo import config
from pyparadiseo import solution

import numpy as np

import unittest

class TestSolution(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('real')

        self.sol = solution.empty('real')

    def test_fitness_none(self):
        self.assertEqual(self.sol.fitness,None)

    def test_zeros(self):
        sol = solution.zeros(4,'real')
        for i in sol.carray:
            self.assertEqual(i,0.0)
        self.assertEqual(len(sol),4)

    def test_modify(self):
        sol = solution.zeros(4,'real')

        #set std::vector ...
        sol.carray[3]=3.0
        self.assertEqual(sol.carray[3],3.0)
        #check that ndarray is modified
        self.assertEqual(sol.array[3],3.0)

        #assign ndarray
        sol.array = np.ones(4)

        #iterate over std::vector, check if all one
        for i in sol.carray:
            self.assertEqual(i,1.0)
        #same for ndarray
        for i in sol.array:
            self.assertEqual(i,1.0)

        #set std::vector again
        sol.carray[2]=2.0
        self.assertEqual(sol.array[2],2.0)
        self.assertEqual(sol.carray[2],2.0)

    def test_copy_ctor(self):
        self.sol.resize(4)
        # self.sol.encoding = np.zeros(4)
        self.sol.array[1] = 1.0
        self.sol.fitness = 42.0
        self.sol.objectives = [1.1,2.2]
        self.sol.diversity = 4.2

        #copy ctor
        sol2 = solution.solution(self.sol)
        self.assertEqual(sol2.array[0],0.0)
        self.assertEqual(sol2.array[1],1.0)
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        #change sol
        self.sol.array[0] = 1.0
        self.sol.fitness = 2.0
        self.sol.objectives = [0.1,0.2]
        self.sol.diversity = 2.2

        #check that sol2 hasn't changed
        self.assertEqual(sol2.array[0],0.0)
        self.assertEqual(sol2.array[1],1.0)
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    # def test_encoding(self):
    #     sol = solution.empty()
    #     sol.encoding = [1,2,3.0]
    #     self.assertEqual(sol.encoding,[1,2,3.0])
    #     self.assertEqual(sol[0],1)
    #     self.assertEqual(sol[1],2)
    #     self.assertEqual(sol[2],3.0)
    #     sol[0]=42
    #     self.assertEqual(sol[0],42)
    #
    def test_setFitness(self):
        self.assertEqual(self.sol.fitness,None)
        self.sol.fitness = 1.11
        self.assertEqual(self.sol.fitness,1.11)
        self.sol.fitness = 42
        self.assertEqual(self.sol.fitness,42)

    def test_compare(self):
        sol1 = solution.empty('real')
        sol2 = solution.empty('real')
        sol1.fitness = 1.0
        sol2.fitness = 2.0

        pp.config.set_minimize_fitness()
        self.assertTrue(sol1 > sol2)
        self.assertFalse(sol1 < sol2)

        pp.config.set_maximize_fitness()
        self.assertFalse(sol1 > sol2)
        self.assertTrue(sol1 < sol2)

    def test_setDiversity(self):
        sol = solution.empty('real')
        sol.diversity = 1.11
        self.assertEqual(sol.diversity,1.11)
        sol.diversity = 1
        self.assertEqual(sol.diversity,1)

    def test_objectiveVector(self):
        sol = solution.empty('real')
        sol.objectiveVector = [1.1,2.2]
        self.assertEqual(sol.objectiveVector[0],1.1)
        self.assertEqual(sol.objectiveVector[1],2.2)

    def test_invalidate(self):
        sol = solution.empty()
        sol.objectiveVector = [1.1,2.2]
        sol.invalidateObjectiveVector()
        self.assertTrue(sol.invalidObjectiveVector())

    # def test_pickle(self):
    #     #make a solution
    #     sol = solution.zeros(5,'real')
    #     sol.fitness = 42.0
    #     sol.objectiveVector = [1.1,2.2]
    #     sol.diversity = 4.2
    #     #copy that solution
    #     from copy import copy,deepcopy
    #     sol2 = copy(sol)
    #     #check new solution is identical to first
    #     # self.assertEqual(sol2.encoding,[1,2,3,4])
    #     self.assertEqual(sol2.fitness,42.0)
    #     self.assertEqual(sol2.objectiveVector[0],1.1)
    #     self.assertEqual(sol2.objectiveVector[1],2.2)
    #     self.assertEqual(sol2.diversity,4.2)
    #     #change original
    #     sol.encoding[0] = 1.0
    #     sol.invalidate()
    #     #check that copy didn't change
    #     # self.assertEqual(sol2.encoding,[1,2,3,4])
    #     self.assertEqual(sol2.fitness,42.0)
    #     self.assertEqual(sol2.objectiveVector[0],1.1)
    #     self.assertEqual(sol2.objectiveVector[1],2.2)
    #     self.assertEqual(sol2.diversity,4.2)
    #
    # def test_solution_creation(self):
    #     from pyparadiseo._core import Solution,BinarySolution,RealSolution
    #
    #     sol=solution.empty()
    #     self.assertTrue(isinstance(sol,Solution))
    #
    #     sol=solution.empty(stype='bin')
    #     self.assertTrue(isinstance(sol,BinarySolution))
    #
    #     sol=solution.empty(stype='real')
    #     self.assertTrue(isinstance(sol,RealSolution))
    #
    #     sol=solution.from_object([1,2,3]*3,stype='real')
    #     self.assertTrue(isinstance(sol,RealSolution))
    #     self.assertEqual(len(sol),9)



        #deepcopy needs pickling for members ? objectiveVector etc
        # sol3 = deepcopy(sol2)
        # sol2.encoding = [0]
        # sol2.invalidate()
        #
        # self.assertEqual(sol3.encoding,[1,2,3,4])
        # self.assertEqual(sol3.fitness,42.0)
        # self.assertEqual(sol3.objectiveVector[0],1.1)
        # self.assertEqual(sol3.objectiveVector[1],2.2)
        # self.assertEqual(sol3.diversity,4.2)

if __name__ == '__main__':
    unittest.main()
