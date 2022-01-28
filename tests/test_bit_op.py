import pyparadiseo as pp

from pyparadiseo import PyEOT
from pyparadiseo import rng

import unittest

class TestBitOp(unittest.TestCase):
    def test_ctor(self):
        self.init = pp.core.BinaryInit(12)

if __name__ == '__main__':
    init = pp.core.BinaryInit(12)

    bitFlip = pp.core.OneBitFlip()
    detBitFlip = pp.core.DetBitFlip()
    detBitFlip2 = pp.core.DetBitFlip(2)
    detBitFlip8 = pp.core.DetBitFlip(8)

    detSingleBitFlip = pp.core.DetSingleBitFlip()
    detSingleBitFlip2 = pp.core.DetSingleBitFlip(2)
    detSingleBitFlip8 = pp.core.DetSingleBitFlip(8)


    ind1 = PyEOT()
    ind2 = PyEOT()

    init(ind1)
    print(ind1)
    bitFlip(ind1)
    print(ind1)

    print("#"*20)

    init(ind1)
    print(ind1)
    detBitFlip(ind1)
    print(ind1)

    print("#"*20)

    init(ind1)
    print(ind1)
    detBitFlip2(ind1)
    print(ind1)

    print("#"*20)

    init(ind1)
    print(ind1)
    detBitFlip8(ind1)
    print(ind1)


    print("#"*20)

    init(ind1)
    print(ind1)
    detSingleBitFlip(ind1)
    print(ind1)

    print("#"*20)

    init(ind1)
    print(ind1)
    detSingleBitFlip2(ind1)
    print(ind1)

    print("#"*20)

    init(ind1)
    print(ind1)
    detSingleBitFlip8(ind1)
    print(ind1)



    print("#"*20)

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
