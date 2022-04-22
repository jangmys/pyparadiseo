import pyparadiseo as pp

from pyparadiseo import Solution

import unittest

class TestSolution(unittest.TestCase):
    def test_ctor(self):
        sol = pp.create_solution()
        self.assertEqual(sol.fitness,None)

    def test_ctor_noinit(self):
        sol = Solution([1,2,3.0,'arf'])
        self.assertEqual(sol.encoding,[1,2,3.0,'arf'])
        self.assertEqual(sol[0],1)
        self.assertEqual(sol[1],2)
        self.assertEqual(sol[2],3.0)
        self.assertEqual(sol[3],'arf')

    def test_copy_ctor(self):
        sol = Solution()
        sol.encoding = [1,2,3,4]
        sol.fitness = 42.0
        sol.objectiveVector = [1.1,2.2]
        sol.diversity = 4.2

        #copy ctor
        sol2 = Solution(sol)
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        #change sol
        sol.encoding = [0]
        sol.fitness = 2.0
        sol.objectiveVector = [0.1,0.2]
        sol.diversity = 2.2

        #check that sol2 hasn't changed
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    def test_encoding(self):
        sol = Solution()
        sol.encoding = [1,2,3.0]
        self.assertEqual(sol.encoding,[1,2,3.0])
        self.assertEqual(sol[0],1)
        self.assertEqual(sol[1],2)
        self.assertEqual(sol[2],3.0)
        sol[0]=42
        self.assertEqual(sol[0],42)

    def test_setFitness(self):
        sol = Solution()
        self.assertEqual(sol.fitness,None)
        sol.fitness = 1.11
        print(sol)
        self.assertEqual(sol.fitness,1.11)
        sol.fitness = 42
        self.assertEqual(sol.fitness,42)

    def test_compare(self):
        sol1 = Solution()
        sol2 = Solution()
        sol1.fitness = 1.0
        sol2.fitness = 2.0

        pp.set_minimize_fitness()
        self.assertTrue(sol1 > sol2)
        self.assertFalse(sol1 < sol2)

        pp.set_maximize_fitness()
        self.assertFalse(sol1 > sol2)
        self.assertTrue(sol1 < sol2)

    def test_setDiversity(self):
        sol = Solution()
        sol.diversity = 1.11
        self.assertEqual(sol.diversity,1.11)
        sol.diversity = 1
        self.assertEqual(sol.diversity,1)

    def test_objectiveVector(self):
        sol = Solution()
        sol.objectiveVector = [1.1,2.2]
        self.assertEqual(sol.objectiveVector[0],1.1)
        self.assertEqual(sol.objectiveVector[1],2.2)

    def test_invalidate(self):
        sol = Solution()
        sol.objectiveVector = [1.1,2.2]
        sol.invalidateObjectiveVector()
        self.assertTrue(sol.invalidObjectiveVector())

    def test_pickle(self):
        #make a solution
        sol = Solution([1,2,3,4])
        sol.fitness = 42.0
        sol.objectiveVector = [1.1,2.2]
        sol.diversity = 4.2
        #copy that solution
        from copy import copy,deepcopy
        sol2 = copy(sol)
        #check new solution is identical to first
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)
        #change original
        sol.encoding = [0]
        sol.invalidate()
        #check that copy didn't change
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectiveVector[0],1.1)
        self.assertEqual(sol2.objectiveVector[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    def test_create_solution(self):
        sol = pp.create_solution([1,2,3.0,'arf'])
        self.assertEqual(sol.encoding,[1,2,3.0,'arf'])

        sol = pp.create_solution(stype="real")


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
    print("hello")

    sol = pp.RealSolution()
    print(sol)

    sol = pp.RealSolution(10)


    sol = pp.BinarySolution()
    print(sol)

    sol = pp.BinarySolution(10)
    print(sol)

    # print(sol[1])


    unittest.main()
