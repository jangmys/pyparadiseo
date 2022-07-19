from pyparadiseo import solution

from pyparadiseo._core import moNeighborhood
from pyparadiseo._core import moIndexNeighborhood


from pyparadiseo import mo
from pyparadiseo.mo import neighborhood
from pyparadiseo._core import Neighbor

import unittest
import numpy as np

class TestNeighborhood(unittest.TestCase):
    def setUp(self):
        pass
    def tearDown(self):
        pass
    def test_derived(self):
        class derivedNeighborhood(moNeighborhood):
            def __init__(self):
                moNeighborhood.__init__(self)
            def hasNeighbor(self,pyeot):
                return True
            def init(self,pyeot,nbor):
                pass
            def cont(self,pyeot):
                pass
            def next(self,pyeot,nbor):
                pass

        nhood = derivedNeighborhood()
        self.assertEqual(nhood.__class__.__bases__[0].__name__,"moNeighborhood")

    def test_derivedIndex(self):
        class derivedIndexNeighborhood(moIndexNeighborhood):
            def __init__(self,nhoodSize):
                moIndexNeighborhood.__init__(self,nhoodSize)
            def hasNeighbor(self,pyeot):
                return True
            def init(self,pyeot,PyNeighbor):
                pass
            def next(self,pyeot,PyNeighbor):
                pass
            def cont(self,pyeot):
                pass
        nhood = derivedIndexNeighborhood(10)
        self.assertEqual(nhood.__class__.__bases__[0].__name__,"moIndexNeighborhood")
        self.assertEqual(nhood.neighborhood_size,10)

    def test_orderNeighborhood(self):
        sol = solution.empty()
        nbor= Neighbor()

        nhood = mo.neighborhood.OrderNeighborhood()
        self.assertFalse(nhood.has_neighbor(sol)) #no size given in ctor
        self.assertEqual(nhood.neighborhood_size,0)

        nhood = mo.neighborhood.OrderNeighborhood(42)
        self.assertTrue(nhood.has_neighbor(sol)) #size given in ctor
        self.assertEqual(nhood.neighborhood_size,42)

        nhood.init(sol,nbor)
        self.assertEqual(nbor.index(),0)
        self.assertEqual(nhood.position,0)

        i=0
        while nhood.cont(sol):
            nhood.next(sol,nbor)
            i = i + 1

            self.assertEqual(nhood.position,i)
            self.assertEqual(nbor.index(),i)



if __name__ == '__main__':
    unittest.main()
