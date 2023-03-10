import pyparadiseo as pp

from pyparadiseo import config

from pyparadiseo import solution
# from pyparadiseo import Solution

import unittest

class TestSolution(unittest.TestCase):
    def setUp(self):
        self.sol = solution.empty()

    def test_fitness_none(self):
        self.assertEqual(self.sol.fitness,None)

    def test_ctor_noinit(self):
        sol = solution.solution([1,2,3.0,'arf'])
        self.assertEqual(sol.encoding,[1,2,3.0,'arf'])
        self.assertEqual(sol[0],1)
        self.assertEqual(sol[1],2)
        self.assertEqual(sol[2],3.0)
        self.assertEqual(sol[3],'arf')

    def test_copy_ctor(self):
        self.sol.encoding = [1,2,3,4]
        self.sol.fitness = 42.0
        self.sol.objectives = [1.1,2.2]
        self.sol.diversity = 4.2

        #copy ctor
        sol2 = solution.solution(self.sol)
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        #change sol
        self.sol.encoding = [0]
        self.sol.fitness = 2.0
        self.sol.objectives = [0.1,0.2]
        self.sol.diversity = 2.2

        #check that sol2 hasn't changed
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    def test_encoding(self):
        sol = solution.empty()
        sol.encoding = [1,2,3.0]
        self.assertEqual(sol.encoding,[1,2,3.0])
        self.assertEqual(sol[0],1)
        self.assertEqual(sol[1],2)
        self.assertEqual(sol[2],3.0)
        sol[0]=42
        self.assertEqual(sol[0],42)

    def test_setFitness(self):
        sol = solution.empty()
        self.assertEqual(sol.fitness,None)
        sol.fitness = 1.11
        self.assertEqual(sol.fitness,1.11)
        sol.fitness = 42
        self.assertEqual(sol.fitness,42)

    def test_compare(self):
        sol1 = solution.empty()
        sol2 = solution.empty()
        sol1.fitness = 1.0
        sol2.fitness = 2.0

        pp.config.set_minimize_fitness()
        self.assertTrue(sol1 > sol2)
        self.assertFalse(sol1 < sol2)

        pp.config.set_maximize_fitness()
        self.assertFalse(sol1 > sol2)
        self.assertTrue(sol1 < sol2)

    def test_setDiversity(self):
        sol = solution.empty()
        sol.diversity = 1.11
        self.assertEqual(sol.diversity,1.11)
        sol.diversity = 1
        self.assertEqual(sol.diversity,1)

    def test_objectiveVector(self):
        sol = solution.empty()
        sol.objectives = [1.1,2.2]
        self.assertEqual(sol.objectives[0],1.1)
        self.assertEqual(sol.objectives[1],2.2)

    def test_invalidate(self):
        sol = solution.empty()
        sol.objectives = [1.1,2.2]
        sol.invalidateObjectiveVector()
        self.assertTrue(sol.invalidObjectiveVector())

    def test_pickle(self):
        #make a solution
        sol = solution.solution([1,2,3,4])
        sol.fitness = 42.0
        sol.objectives = [1.1,2.2]
        sol.diversity = 4.2
        #copy that solution
        from copy import copy,deepcopy
        sol2 = copy(sol)
        #check new solution is identical to first
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)
        #change original
        sol.encoding = [0]
        sol.invalidate()
        #check that copy didn't change
        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

        sol3 = deepcopy(sol2)
        sol2.encoding = [0]
        sol2.invalidate()

        self.assertEqual(sol3.encoding,[1,2,3,4])
        self.assertEqual(sol3.fitness,42.0)
        self.assertEqual(sol3.objectives[0],1.1)
        self.assertEqual(sol3.objectives[1],2.2)
        self.assertEqual(sol3.diversity,4.2)


    def test_solution_creation(self):
        from pyparadiseo._core import Solution,BinarySolution,RealSolution

        sol=solution.empty()
        self.assertTrue(isinstance(sol,Solution))

        sol=solution.empty(stype='bin')
        self.assertTrue(isinstance(sol,BinarySolution))

        sol=solution.empty(stype='real')
        self.assertTrue(isinstance(sol,RealSolution))

        sol=solution.solution([0,1,2]*3,stype='real')
        self.assertTrue(isinstance(sol,RealSolution))
        self.assertEqual(len(sol),9)
        for i in range(9):
            self.assertEqual(sol.array[i],i%3)
            self.assertEqual(sol.carray[i],i%3)

        self.assertRaises(TypeError,solution.solution,3,stype='real')



if __name__ == '__main__':

    config.set_solution_type('real')

    print('hi')
    sol=solution.solution([0,1,2]*3,stype='real')

    # for item in dir(sol):
    #     print(item,getattr(sol,item))

    # print('obj: ',sol.objectives)
    # print('__class__: ',sol.__class__)
    # print('__name__: ',sol.__name__)

    sol.data = dict()
    sol.data.update({'sim' : True})
    print(sol)
    print(sol.data)

    from pyparadiseo._core import _PyEO

    print(issubclass(sol.__class__, _PyEO))

    klass = sol.__class__
    print("#"*20," new (copy)")
    sol3 = klass(sol)
    print(sol3)
    print(sol3.data)


    # sol.data['sim']=False
    #
    # import copy
    #
    print("#"*20," new (copy)")
    sol2 = solution.solution(sol)
    print(sol2)
    print(sol2.data)
    # print("#"*20," copy")
    #
    # sol2.data = copy.deepcopy(sol.data)
    # print(sol2)
    # print(sol2.data)

    # unittest.main()
