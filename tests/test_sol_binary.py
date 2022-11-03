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
            self.assertTrue(isinstance(i, int))

    def test_setter(self):
        init = initializer.random(10,stype='bin')
        init(self.sol)

        self.sol.carray = np.random.randint(0,2,10)
        print([i for i in self.sol.carray])
        print(self.sol.array)

        self.sol.array = np.random.randint(0,2,10)#,dtype=np.uintc)
        print([i for i in self.sol.carray])
        print(self.sol.array)

        self.sol.encoding = np.random.randint(0,2,10)
        print([i for i in self.sol.carray])
        print(self.sol.array)

        self.sol.encoding = [0,1,0,1]
        print([i for i in self.sol.carray])
        print(self.sol.array)

        self.sol.encoding[0] = 1
        print([i for i in self.sol.carray])
        print(self.sol.array)

if __name__ == '__main__':
    unittest.main()
