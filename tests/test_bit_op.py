# import pyparadiseo as pp

from pyparadiseo import Solution
from pyparadiseo import rng
from pyparadiseo import operator,initializer

import numpy as np

import unittest
import copy


class TestBitOp(unittest.TestCase):
    def setUp(self):
        self.init = initializer.BinaryInit(10)

    def test_OneBitFlip(self):
        ind=Solution()
        self.init(ind)
        ind2=copy.deepcopy(ind)
        bitFlip = operator.OneBitFlip()
        bitFlip(ind)
        #check that exactly one bit is different
        self.assertEqual(1,np.sum(np.logical_xor(ind.encoding,ind2.encoding)))

    def mut_and_count_diff(self,mut_op):
        ind=Solution()
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
        self.assertEqual(1,self.mut_and_count_diff(operator.OneBitFlip()))

    def test_DetBitFlip(self):
        for i in range(1,10):
            self.assertTrue(self.mut_and_count_diff(operator.DetBitFlip(i)) <= i)

    def test_DetSingleBitFlip(self):
        for i in range(1,10):
            self.assertEqual(self.mut_and_count_diff(operator.DetSingleBitFlip(i)),i)



if __name__ == '__main__':
    unittest.main()
    init = initializer.BinaryInit(12)

    ind=Solution()
    init(ind)
    ind2=copy.deepcopy(ind)
    bitFlip = operator.OneBitFlip()
    bitFlip(ind)

    print(ind.encoding)
    print((ind2.encoding))

    print(np.logical_xor(ind.encoding,ind2.encoding))

    print(np.sum(np.logical_xor(ind.encoding,ind2.encoding)))


    bitFlip = operator.OneBitFlip()
    detBitFlip = operator.DetBitFlip()
    detBitFlip2 = operator.DetBitFlip(2)
    detBitFlip8 = operator.DetBitFlip(8)

    detSingleBitFlip = operator.DetSingleBitFlip()
    detSingleBitFlip2 = operator.DetSingleBitFlip(2)
    detSingleBitFlip8 = operator.DetSingleBitFlip(8)


    ind1 = Solution()
    ind2 = Solution()

    onePt = operator.OnePtBitCrossover()
    uBit = operator.UBitCrossover()
    nPtBit = operator.NPtsBitCrossover()

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
