from pyparadiseo import Solution
from pyparadiseo.initializer import BinaryInit

import numpy as np

import unittest

class TestBinary(unittest.TestCase):
    def setUp(self):
        self.sol = Solution()

    def test_len(self):
        init = BinaryInit(10)
        init(self.sol)
        self.assertEqual(len(self.sol),10)

    def test_iter(self):
        init = BinaryInit(10)
        init(self.sol)
        #check that we can iterate solution and it contains bools
        for i in self.sol:
            self.assertTrue(isinstance(i, np.bool_))

if __name__ == '__main__':
    unittest.main()
