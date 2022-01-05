from pyparadiseo.eo import transform

from pyparadiseo import pyMonOp
from pyparadiseo import pyQuadOp

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoTransform(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(transform):
            if inspect.isclass(obj):
                self.assertEqual(obj.__bases__[0].__name__,"eoTransform")
    def test_ctor(self):
        def mutate(ind1):
            for i in range(len(ind1)):
                ind1[i]=0
            return True

        def xover(ind1,ind2):
            for i in range(len(ind1)):
                ind1[i]=0
            for i in range(len(ind2)):
                ind2[i]=1
            return True

        mymut = pyMonOp(mutate)
        mycross = pyQuadOp(xover)

        SGATransform = transform.SGATransform(mycross,0.5,mymut,0.5)

        self.assertTrue(callable(SGATransform))
