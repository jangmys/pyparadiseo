from pyparadiseo.eo import transform

from pyparadiseo.operator import pyMonOp,pyQuadOp

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoTransform(unittest.TestCase):
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

        SGATransform = transform.SGA(mycross,0.5,mymut,0.5)

        self.assertTrue(callable(SGATransform))
