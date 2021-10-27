from pyparadiseo import PyEOT
from pyparadiseo import eo
from pyparadiseo import pyeoInit

from pyparadiseo import pyMonOp
from pyparadiseo import pyBinOp
from pyparadiseo import pyQuadOp

from pyparadiseo import MonOp
from pyparadiseo import BinOp
from pyparadiseo import QuadOp


import unittest
import numpy as np


class test_genops(unittest.TestCase):
    def setUp(self):
        self.init = pyeoInit(lambda : np.random.randint(0,2,10))
        self.ind1 = PyEOT()
        self.init(self.ind1)

        self.ind2 = PyEOT()
        self.init(self.ind2)

    def tearDown(self):
        pass
    def test_pyMonOp(self):
        #this is better : no need to know "genome"
        #...just write a compatible mutation operator and register...
        def foo(ind1):
            for i in range(len(ind1)):
                ind1[i]=0
            return True

        mymut = pyMonOp(foo)

        self.assertTrue(mymut(self.ind1))
        self.assertTrue(np.all(self.ind1.encoding == 0))
        self.assertEqual(str(mymut.getType()),'unary')

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
