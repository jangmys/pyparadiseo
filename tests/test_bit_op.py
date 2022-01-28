import pyparadiseo as pp

from pyparadiseo import PyEOT
from pyparadiseo import rng

import numpy as np

import unittest
import copy


class TestBitOp(unittest.TestCase):
    def setUp(self):
        self.init = pp.core.BinaryInit(10)

    def test_OneBitFlip(self):
        ind=PyEOT()
        self.init(ind)
        ind2=copy.deepcopy(ind)
        bitFlip = pp.core.OneBitFlip()
        bitFlip(ind)
        #check that exactly one bit is different
        self.assertEqual(1,np.sum(np.logical_xor(ind.encoding,ind2.encoding)))

    def mut_and_count_diff(self,mut_op):
        ind=PyEOT()
        self.init(ind)
        ind.fitness = 1.23
        ind2=copy.deepcopy(ind)
        #if ind changed, nut_op returns true and ind is invalidated
        if mut_op(ind):
            ind.invalidate()
        self.assertTrue(ind.invalid())
        #return number of bits that are changed
        return np.sum(np.logical_xor(ind.encoding,ind2.encoding))

    def test_OneBitFlip(self):
        self.assertEqual(1,self.mut_and_count_diff(pp.core.OneBitFlip()))

    def test_DetBitFlip(self):
        for i in range(1,10):
            self.assertTrue(self.mut_and_count_diff(pp.core.DetBitFlip(i)) <= i)

    def test_DetSingleBitFlip(self):
        for i in range(1,10):
            self.assertEqual(self.mut_and_count_diff(pp.core.DetSingleBitFlip(i)),i)



if __name__ == '__main__':
    unittest.main()
    init = pp.core.BinaryInit(12)

    ind=PyEOT()
    init(ind)
    ind2=copy.deepcopy(ind)
    bitFlip = pp.core.OneBitFlip()
    bitFlip(ind)

    print(ind.encoding)
    print((ind2.encoding))

    print(np.logical_xor(ind.encoding,ind2.encoding))

    print(np.sum(np.logical_xor(ind.encoding,ind2.encoding)))


    bitFlip = pp.core.OneBitFlip()
    detBitFlip = pp.core.DetBitFlip()
    detBitFlip2 = pp.core.DetBitFlip(2)
    detBitFlip8 = pp.core.DetBitFlip(8)

    detSingleBitFlip = pp.core.DetSingleBitFlip()
    detSingleBitFlip2 = pp.core.DetSingleBitFlip(2)
    detSingleBitFlip8 = pp.core.DetSingleBitFlip(8)


    ind1 = PyEOT()
    ind2 = PyEOT()

    onePt = pp.core.OnePtBitCrossover()
    uBit = pp.core.UBitCrossover()
    nPtBit = pp.core.NPtsBitCrossover()

    rng().reseed(42)
    init(ind1)
    init(ind2)

    print(ind1)
    print(ind2)
    onePt(ind1,ind2)
    print(ind1)
    print(ind2)

    print("#"*20)

    print(ind1)
    print(ind2)
    uBit(ind1,ind2)
    print(ind1)
    print(ind2)

    print("#"*20)

    print(ind1)
    print(ind2)
    nPtBit(ind1,ind2)
    print(ind1)
    print(ind2)




    # unittest.main()
