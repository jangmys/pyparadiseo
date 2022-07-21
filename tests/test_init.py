from pyparadiseo import eo
from pyparadiseo import initializer
from pyparadiseo import solution

#ABC
from pyparadiseo._core import eoInit

import inspect
import unittest
import numpy as np

class test_init(unittest.TestCase):
    def setUp(self):
        self.sol = solution.empty()
        self.bin_sol = solution.empty('bin')
        self.real_sol = solution.empty('real')
    def tearDown(self):
        pass
    def test_gen_init(self):
        #define some function returning a (random) solution
        def init_fun(n):
            def init():
                return np.arange(n)
            return init
        #construct a Pyparadiseo initalizer passing a function
        init = initializer.initializer(init_fun(10))
        #check that we have an instance of eoInit
        self.assertTrue(isinstance(init,eoInit))

        # #a solution
        init(self.sol)
        self.assertEqual(len(self.sol),10)
        self.assertEqual(len(self.sol.encoding),10)
        for i in range(10):
            self.assertEqual(self.sol[i],i)

        #construct a Pyparadiseo initalizer passing a lambda
        init = initializer.initializer(lambda : np.arange(1,5))
        self.assertTrue(isinstance(init,eoInit))

        init(self.sol)
        self.assertEqual(len(self.sol),4)
        self.assertEqual(len(self.sol.encoding),4)
        for i in range(4):
            self.assertEqual(self.sol[i],i+1)

    def test_init_with_callable_class(self):
        # callable class
        class testProblem(object):
            def __init__(self, arg):
                super(testProblem, self).__init__()
                self.arg = arg
            def __call__(self):
                return np.arange(self.arg)

        pb = testProblem(9)
        init = initializer.initializer(pb)
        self.assertTrue(isinstance(init,eoInit))

        init(self.sol)
        self.assertEqual(len(self.sol),9)
        #
    def test_inherit(self):
        #inherit
        class myInit(eoInit):
            def __init__(self,n):
                eoInit.__init__(self)
                self.size = n
            def __call__(self,sol):
                sol.encoding = np.random.randint(0,2,self.size)

        init = myInit(13)
        self.assertTrue(isinstance(init,eoInit))

        init(self.sol)
        self.assertEqual(len(self.sol),13)

        myInit(11)(self.sol)
        self.assertEqual(len(self.sol),11)

    def test_binary_init(self):
        init = initializer.random(10,'bin')
        init(self.bin_sol)
        self.assertEqual(len(self.bin_sol),10)

    def test_real_bounded_init(self):
        from pyparadiseo import bounds

        b = bounds.bound_box(9,-1.0,1.0)
        init = initializer.random(stype='real',bounds=b)
        # init(self.real_sol)
    #     self.assertEqual(len(self.real_sol),9)
    #
    #     b = bounds.RealVectorBounds(7,-1,1)
    #     init = initializer.random(stype='real',bounds=b)
    #     init(self.real_sol)
    #     self.assertEqual(len(self.real_sol),7)

        # self.assertEqual(len(self.bin_sol),10)
        # self.assertTrue(isinstance(init,eoInitBin))

if __name__ == "__main__":
    unittest.main()

    sol = Solution()

    # simple function
    def fun() : return set(range(10))

    initializer = Init(fun)
    initializer(sol)
    print(sol)

    # lambda
    initializer = Init(lambda : set(range(7)))
    initializer(sol)
    print(sol)

    # partial
    from functools import partial

    def bar(n) : return set(range(n))
    initializer = Init(partial(bar,8))
    initializer(sol)
    print(sol)

    # a class is callable ... but need to implement __len__, __str__ etc if needed
    class A():
        def __init__(self):
            self.var = np.arange(1,5)
            print("init")
        def __str__(self):
            return '{self.var}'.format(self=self)
        def __len__(self):
            return len(self.var)

    initializer = Init(A)
    initializer(sol)

    # print(A())
    print(sol)
    print(len(sol))
    print(sol.encoding)
