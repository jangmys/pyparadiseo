from pyparadiseo import PyEOT
from pyparadiseo import mo

import unittest
import numpy as np

class TestNeighbor(unittest.TestCase):
    def setUp(self):
        pass
    def tearDown(self):
        pass
    def test_ctor(self):
        sol = mo.Neighbor()
    def test_Fitness(self):
        nbor = mo.Neighbor()
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

        nbor2 = mo.Neighbor()
        nbor2.fitness = 3.0

        self.assertFalse(nbor.equals(nbor2),"default equals is false")

    def test_assign(self):
        nbor = mo.Neighbor()
        nbor.fitness = 3.0
        nbor.index(4)
        self.assertEqual(nbor.fitness,3.0)
        self.assertEqual(nbor.index(),4)

        nbor2 = mo.Neighbor()
        nbor2.reassign(nbor)
        self.assertEqual(nbor2.fitness,3.0)
        self.assertEqual(nbor2.index(),4)

    def test_setMove(self):
        #moves any solution to [0,1,2.0,0]
        def mv(nbor,sol):
            sol.encoding = [0,1,2.0]
        #moves any solution back to None
        def mvBack(nbor,sol):
            sol.encoding = None

        #a solution
        sol = PyEOT()

        nbor = mo.Neighbor()
        nbor.setMove(mv)

        nbor.move(sol)
        self.assertEqual(sol[0],0)
        self.assertEqual(sol[1],1)
        self.assertEqual(sol[2],2.0)

        nbor.setMoveBack(mvBack)
        nbor.moveBack(sol)
        self.assertEqual(sol.encoding,None)

    def test_index(self):
        sol = PyEOT()
        nbor = mo.Neighbor()

        nbor.index(sol,1)
        self.assertEqual(nbor.index(),1)

        nbor.index(2)
        self.assertEqual(nbor.index(),2)


if __name__ == '__main__':
    unittest.main()
