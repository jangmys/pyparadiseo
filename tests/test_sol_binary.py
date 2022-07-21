from pyparadiseo import solution
from pyparadiseo import initializer
from pyparadiseo import config

import numpy as np

import unittest

class TestBinary(unittest.TestCase):
    def setUp(self):
        self.sol = solution.empty(stype='bin')
        # config.set_solution_type('bin')

    def test_len(self):
        init = initializer.random(10,stype='bin')
        init(self.sol)
        self.assertEqual(len(self.sol),10)

    def test_iter(self):
        init = initializer.random(10,stype='bin')
        init(self.sol)
        #check that we can iterate solution and it contains bools
        for i in self.sol.array:
            self.assertTrue(isinstance(i, np.int32))
        for i in self.sol.carray:
            self.assertTrue(isinstance(i, np.int))

if __name__ == '__main__':
    unittest.main()
