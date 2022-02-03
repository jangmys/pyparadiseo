from pyparadiseo import PyEOT
from pyparadiseo import rng

import unittest
import numpy as np
#### FIXME: test more than only basic assumptions...

class TestRandom(unittest.TestCase):
    def setUp(self):
        self.r = rng()
        self.ints = np.asarray([1,1,2,3,5,8,13,21],dtype=int)
        self.reals = np.asarray([0.1,0.1,.2,.3,.5,.8,1.3,2.1])

        # self.r.reseed(42)

    def test_uniform(self):
        #just check if we stay in bounds
        self.assertTrue(self.r.uniform() >= 0.0)
        self.assertTrue(self.r.uniform() < 1.0)

        self.assertTrue(self.r.uniform(99.0) >= 0.0)
        self.assertTrue(self.r.uniform(99.0) < 99.0)

        self.assertTrue(self.r.uniform(111.,222.) >= 111.)
        self.assertTrue(self.r.uniform(222.,333.) < 333.)

    def test_randint(self):
        #just check if we stay in bounds
        self.assertTrue(self.r.random(11) < 11)
        self.assertTrue(self.r.random(11) >= 0)

    def test_flip(self):
        #unbiased
        expect = [False,False,False,False,False,False,True]
        self.r.reseed(42)
        for e in expect:
            self.assertTrue(self.r.flip() == e)
        #biased 0.8
        expect = [True,False,False,True,True,True,True]
        self.r.reseed(42)
        for e in expect:
            self.assertTrue(self.r.flip(0.8) == e)
        #biased always true
        self.r.reseed(rng().random(10000))
        for i in range(10):
            self.assertTrue(self.r.flip(1.0))
        #biased always false
        for i in range(10):
            self.assertFalse(self.r.flip(0.0))

    def test_spin(self):
        self.r.reseed(42)
        expect = [6,7,7,6,7,7,5]
        for e in expect:
            self.assertTrue(self.r.roulette_wheel(self.ints) == e)
        ###
        self.r.reseed(42)
        tot = np.sum(self.ints)
        for e in expect:
            self.assertTrue(self.r.roulette_wheel(self.ints,tot) == e)
        ###
        self.r.reseed(42)
        for e in expect:
            self.assertTrue(self.r.roulette_wheel(self.reals) == e)


if __name__ == '__main__':
    unittest.main()
    # rng().reseed(42)
    #
    # #######################################
    # ### UNIFORM
    # print("[0,1.0) ",rng().uniform())
    # print("[0,100.0) ",rng().uniform(100.0))
    # print("[100.,200.) ",rng().uniform(100.0,200.0))
    #
    # ################################
    # ### RANDOM (INT)
    # count = [0]*10
    # for i in range(1000):
    #     count[rng().random(10)] += 1
    # print(count)
    #
    # ################################
    # ### FLIP
    # rng().reseed(42)
    # for i in range(10):
    #     print(rng().flip())
    #
    # print("="*20)
    #
    # rng().reseed(42)
    # for i in range(10):
    #     print(rng().flip(0.8))
    #
    # print("="*20)
    #
    # rng().reseed(42)
    # for i in range(10):
    #     print(rng().flip(1.5))
    #
    # rng().reseed(42)
    # for i in range(10):
    #     print(rng().flip(-1.5))
    #
    #
    #
    # count = 0
    # for i in range(1000):
    #     if rng().flip():
    #         count += 1
    # print(count/1000)
    #
    # count = 0
    # for i in range(1000):
    #     if rng().flip(0.1):
    #         count += 1
    # print(count/1000)
    #
    #
    # ################################
    # ### RAND (INT, [0,rand_max))
    # print(rng().rand())
    #
    # print(rng().binomial(10,5))
    #
    # ###########################
    # ### NORMAL
    # print(rng().normal(1.0))
    # print(rng().normal(10.0,4.0))
    #
    # ###########################
    # ### NEGEXP
    # print(rng().negexp(42.0))
    #
    # print(rng().rand_max())
    #
    # # import numpy as np
    #
    # ints = np.asarray([1,1,2,3,5,8,13,21],dtype=int)
    # reals = np.asarray([0.1,0.1,.2,.3,.5,.8,1.3,2.1])
    #
    # print("="*20)
    # rng().reseed(42)
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    # print(rng().roulette_wheel(ints))
    #
    #
    #
    #
    # print("="*20)
    # rng().reseed(42)
    # print(rng().roulette_wheel(np.arange(10),45))
    #
    #
    # # print(rng().to_string())
    # # print(rng().flip(0.1))
