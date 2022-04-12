# import pyparadiseo as pp

from pyparadiseo import Solution
from pyparadiseo import rng
from pyparadiseo import operator,initializer,bounds

import numpy as np

import unittest
import copy


class TestRealOp(unittest.TestCase):
    def setUp(self):
        self.init = initializer.RealBoundedInit(bounds.RealVectorBounds(10,-1,1))
        self.sol = Solution()
        self.init(self.sol)

    def test_UniformMutation(self):
        mutate = operator.UniformMutation(0.1)
        print(self.sol)
        mutate(self.sol)
        print(self.sol)

        mutate = operator.UniformMutation(0.8,0.2)
        print(self.sol)
        mutate(self.sol)
        print(self.sol)

    def test_DetUniformMutation(self):
        pass

    def test_SegmentCrossover(self):
        pass

    def test_HypercubeCrossover(self):
        pass


if __name__ == '__main__':
    unittest.main()
