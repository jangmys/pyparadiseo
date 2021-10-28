from pyparadiseo import PyEOT
from pyparadiseo import eo

from pyparadiseo import Init


import unittest
import numpy as np





class test_init(unittest.TestCase):
    def setUp(self):
        pass
    def tearDown(self):
        pass
    def test_pyInit(self):
        #define some function returning a (random) solution
        def init_fun(n):
            def init():
                return np.arange(n)
            return init
        #construct a Pyparadiseo initalizer passing a function
        initializer = Init(init_fun(10))
        #a solution
        sol = PyEOT()
        initializer(sol)
        self.assertEqual(len(sol),10)
        self.assertEqual(len(sol.encoding),10)
        for i in range(10):
            self.assertEqual(sol[i],i)

        #construct a Pyparadiseo initalizer passing a lambda
        initializer = Init(lambda : np.arange(1,5))
        initializer(sol)
        self.assertEqual(len(sol),4)
        self.assertEqual(len(sol.encoding),4)
        for i in range(4):
            self.assertEqual(sol[i],i+1)
