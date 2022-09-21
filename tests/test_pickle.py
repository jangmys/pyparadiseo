from pyparadiseo import config

from pyparadiseo import solution
# from pyparadiseo import Solution

import pickle
import tempfile
import numpy as np

import unittest

class TestPickle(unittest.TestCase):
    def setUp(self):
        self.sol = solution.empty()
        self.real_sol = solution.empty('real')

    def test_pickle_gen(self):
        sol = solution.solution([1,2,3,4])
        sol.fitness = 42.0
        sol.objectives = [1.1,2.2]
        sol.diversity = 4.2

        d=pickle.dumps(sol)
        sol2=pickle.loads(d)

        self.assertEqual(sol2.encoding,[1,2,3,4])
        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)

    def test_pickle_real(self):
        sol = solution.solution([1.0,2.0,3.0,4.0],'real')
        sol.fitness = 42.0
        sol.objectives = [1.1,2.2]
        sol.diversity = 4.2

        d=pickle.dumps(sol)
        sol2=pickle.loads(d)

        np.testing.assert_almost_equal(sol2.carray,[1,2,3,4])
        np.testing.assert_almost_equal(sol2.array,[1,2,3,4])
        np.testing.assert_almost_equal(sol2.encoding,[1,2,3,4])

        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)


    def test_pickle_binary(self):
        sol = solution.solution([1,0,1,1],'bin')
        sol.fitness = 42.0
        sol.objectives = [1.1,2.2]
        sol.diversity = 4.2

        d=pickle.dumps(sol)
        sol2=pickle.loads(d)

        np.testing.assert_almost_equal(sol2.carray,[1,0,1,1])
        np.testing.assert_almost_equal(sol2.array,[1,0,1,1])
        np.testing.assert_almost_equal(sol2.encoding,[1,0,1,1])

        self.assertEqual(sol2.fitness,42.0)
        self.assertEqual(sol2.objectives[0],1.1)
        self.assertEqual(sol2.objectives[1],2.2)
        self.assertEqual(sol2.diversity,4.2)


    def test_deepcopy(self):
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

        # print(sol)
        # print(sol2)
        # print(sol3)

        #check new solution is identical to first
        self.assertEqual(sol3.fitness,42.0)
        self.assertEqual(sol3.objectives[0],1.1)
        self.assertEqual(sol3.objectives[1],2.2)
        self.assertEqual(sol3.diversity,4.2)
        self.assertEqual(sol3.encoding,[1,2,3,4])




if __name__ == '__main__':
    unittest.main()
