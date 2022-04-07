from pyparadiseo import Solution
from pyparadiseo import eo

from pyparadiseo.initializer import eoInit
from pyparadiseo.initializer import Init

import inspect
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
        #check that we have an instance of eoInit
        self.assertTrue(isinstance(initializer,eoInit))

        #a solution
        sol = Solution()
        initializer(sol)
        self.assertEqual(len(sol),10)
        self.assertEqual(len(sol.encoding),10)
        for i in range(10):
            self.assertEqual(sol[i],i)

        #construct a Pyparadiseo initalizer passing a lambda
        initializer = Init(lambda : np.arange(1,5))
        self.assertTrue(isinstance(initializer,eoInit))

        initializer(sol)
        self.assertEqual(len(sol),4)
        self.assertEqual(len(sol.encoding),4)
        for i in range(4):
            self.assertEqual(sol[i],i+1)

        # class
        class testProblem(object):
            def __init__(self, arg):
                super(testProblem, self).__init__()
                self.arg = arg
            def __call__(self):
                return np.arange(self.arg)

        pb = testProblem(9)
        initializer = Init(pb)
        self.assertTrue(isinstance(initializer,eoInit))

        initializer(sol)
        self.assertEqual(len(sol),9)

        #inherit
        class myInit(eoInit):
            def __init__(self,n):
                eoInit.__init__(self)
                self.size = n
            def __call__(self,sol):
                sol.encoding = np.random.randint(0,2,self.size)

        initializer = myInit(13)
        self.assertTrue(isinstance(initializer,eoInit))

        initializer(sol)
        self.assertEqual(len(sol),13)

        myInit(11)(sol)
        self.assertEqual(len(sol),11)


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
