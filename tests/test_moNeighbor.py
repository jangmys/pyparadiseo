from pyparadiseo import config

from pyparadiseo import Solution

from pyparadiseo import solution
from pyparadiseo.mo import neighborhood
from pyparadiseo.mo.neighborhood import Neighbor

import unittest
import numpy as np

class TestNeighbor(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')
        pass
    def tearDown(self):
        pass
    def test_ctor(self):
        sol = Neighbor()
    def test_Fitness(self):
        nbor = Neighbor()
        nbor.fitness = 3.0
        self.assertEqual(nbor.fitness,3.0)

        nbor.fitness = 4
        self.assertEqual(nbor.fitness,4)

        nbor.fitness = np.int32(6)
        self.assertEqual(nbor.fitness,6)

        nbor.fitness = np.sum([3.0,4])
        self.assertEqual(nbor.fitness,7.0)

        nbor.fitness = np.int64(5)
        self.assertEqual(nbor.fitness,5)

        nbor2 = Neighbor()
        nbor2.fitness = 3.0

        # self.assertFalse(nbor.equals(nbor2),"default equals is false")

    def test_assign(self):
        nbor = Neighbor()
        nbor.fitness = 3.0
        nbor.index(4)
        self.assertEqual(nbor.fitness,3.0)
        self.assertEqual(nbor.index(),4)

        nbor2 = Neighbor()
        nbor2.reassign(nbor)
        self.assertEqual(nbor2.fitness,3.0)
        self.assertEqual(nbor2.index(),4)

    def test_setMove(self):
        #moves any solution to [0,1,2.0,0]
        def mv(nbor,sol):
            print("call this")
            sol.encoding = np.array([3.0,2.0,1.0])
        #moves any solution back to None
        def mvBack(nbor,sol):
            sol.encoding = None

        #a solution
        sol = solution.empty()
        sol.encoding = np.array([0,1,2.0])
        print(sol)
        print(sol[0])
        print(sol[1])
        print(sol[2])

        # sol = solution.()
        sol.encoding = np.array([0,1,2.0])
        print(sol)


        #
        nbor = Neighbor()
        print(nbor)
        nbor.setMove(mv)
        nbor.move(sol)

        print("SOL "*10)
        print(sol)

        self.assertEqual(sol[0],3.0)
        self.assertEqual(sol[1],2.0)
        self.assertEqual(sol[2],1.0)
        #
        nbor.setMoveBack(mvBack)
        nbor.moveBack(sol)
        self.assertEqual(sol.encoding,None)

    def test_index(self):
        sol = Solution()
        nbor = Neighbor()

        nbor.index(sol,1)
        self.assertEqual(nbor.index(),1)

        nbor.index(2)
        self.assertEqual(nbor.index(),2)


if __name__ == '__main__':
    unittest.main()
