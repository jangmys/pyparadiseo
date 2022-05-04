from pyparadiseo import Solution
from pyparadiseo import BinarySolution

from pyparadiseo.initializer import BinaryInit
from pyparadiseo.initializer import BinarySolInit

import numpy as np

import unittest

class TestBinary(unittest.TestCase):
    def setUp(self):
        self.sol = BinarySolution()

    def test_len(self):
        init = BinarySolInit(10)
        init(self.sol)
        self.assertEqual(len(self.sol),10)

    def test_iter(self):

        init = BinarySolInit(10)
        init(self.sol)
        #check that we can iterate solution and it contains bools
        for i in self.sol:
            self.assertTrue(isinstance(i, np.bool_))

if __name__ == '__main__':
    sol = BinarySolution()
    print(sol)

    sol = BinarySolution(5)
    print(sol)
    print(type(sol))

    init = BinarySolInit(5)
    init(sol)
    print(sol)


    unittest.main()
