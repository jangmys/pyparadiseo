# import pyparadiseo as pp
from pyparadiseo import solution
from pyparadiseo import Solution
from pyparadiseo import rng
from pyparadiseo import operator,initializer,bounds

import numpy as np

import unittest
import copy


class TestRealOp(unittest.TestCase):
    def setUp(self):
        self.init = initializer.random(stype='real',bounds=bounds.RealVectorBounds(10,-1,1))
        self.sol = solution.empty(stype='real')

        print(type(self.init))
        print(type(self.sol))

        self.init(self.sol)

    def test_UniformMutation(self):
        mutate = operator.UniformMutation(0.1)
        print()
        print(self.sol)
        print(self.sol.array)
        mutate(self.sol)
        print(self.sol)
        print(self.sol.array)
        # mutate = operator.RealUniformMutation(0.1)
        # print(self.sol)
        # mutate(self.sol)
        # print(self.sol)

    def test_DetUniformMutation(self):
        pass

    def test_SegmentCrossover(self):
        pass

    def test_HypercubeCrossover(self):
        pass


if __name__ == '__main__':
    unittest.main()
