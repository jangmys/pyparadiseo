from pyparadiseo import solution
from pyparadiseo import eo
from pyparadiseo import initializer

from pyparadiseo.operator import pyMonOp
from pyparadiseo.operator import pyBinOp
from pyparadiseo.operator import pyQuadOp

from pyparadiseo.operator import MonOp
from pyparadiseo.operator import BinOp
from pyparadiseo.operator import QuadOp

from pyparadiseo import operator

import unittest
import numpy as np


class test_genops(unittest.TestCase):
    def setUp(self):
        self.init = initializer.initializer(lambda : np.random.randint(0,2,10))
        self.ind1 = solution.empty()
        self.init(self.ind1)

        self.ind2 = solution.empty()
        self.init(self.ind2)

        self.bin_ind = solution.random(10,'bin')

    def tearDown(self):
        pass
    def test_pyMonOp(self):
        #this is better : no need to know "genome"
        #...just write a compatible mutation operator and register...
        def foo(ind1):
            for i in range(len(ind1)):
                ind1[i]=0
            return True

        mymut = operator.make_mutation(foo)

        #apply to gen-sol
        self.assertTrue(mymut(self.ind1))
        self.assertTrue(np.all(self.ind1.encoding == 0))
        self.assertEqual(str(mymut.getType()),'unary')
        #apply to bin-sol
# self.assertTrue(mymut(self.bin_ind))

        class bitflip():
            def __init__(self,data):
                self.data = data
                self.counter = 0
            def __call__(self,bits):
                print("hello "+self.data," mutation ",self.counter)
                self.counter += 1
                for i in range(len(bits)):
                    bits[i]=0
                return True

        mymut = operator.make_mutation(bitflip("bouh"))

        self.assertTrue(mymut(self.ind1))
        self.assertTrue(mymut(self.ind1))
        self.assertTrue(mymut(self.ind1))

        print(mymut.op.counter)

        self.assertTrue(np.all(self.ind1.encoding == 0))


    def test_deriveMonOp(self):
        class myMonOp(MonOp):
            def __call__(self,_pyeo):
                for i in range(len(_pyeo)):
                    _pyeo[i]=0
                return True

        mymut = myMonOp()

        self.assertTrue(mymut(self.ind1))
        self.assertTrue(np.all(self.ind1.encoding == 0))
        self.assertEqual(str(mymut.getType()),'unary')

    def test_pyBinOp(self):
        #...just write a compatible crossover operator and register...
        def foo(ind1,ind2):
            for i in range(len(ind1)):
                ind1[i]=0
            for i in range(len(ind2)):
                ind2[i]=1
            return True

        mycross = pyBinOp(foo)

        self.assertTrue(mycross(self.ind1,self.ind2))
        self.assertTrue(np.all(self.ind1.encoding == 0))
        self.assertTrue(np.all(self.ind2.encoding == 1))
        self.assertEqual(str(mycross.getType()),'binary')

    def test_deriveBinOp(self):
        class myBinOp(BinOp):
            def __call__(self,_pyeo1,_pyeo2):
                for i in range(len(_pyeo1)):
                    _pyeo1[i]=0
                for i in range(len(_pyeo2)):
                    _pyeo2[i]=1
                return True

        mycross = myBinOp()

        self.assertTrue(mycross(self.ind1,self.ind2))
        self.assertTrue(np.all(self.ind1.encoding == 0))
        self.assertTrue(np.all(self.ind2.encoding == 1))
        self.assertEqual(str(mycross.getType()),'binary')


if __name__ == '__main__':
    unittest.main()
