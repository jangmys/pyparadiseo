from pyparadiseo import bounds
from pyparadiseo import rng

import unittest
import numpy as np


class TestBounds(unittest.TestCase):
    def setUp(self):
        self.r = rng()
        self.ints = np.asarray([1,1,2,3,5,8,13,21],dtype=int)
        self.reals = np.asarray([0.1,0.1,.2,.3,.5,.8,1.3,2.1])


    def test_realInterval(self):
        interval = bounds.RealInterval(-1.0,1.0)
        self.assertTrue(interval.isBounded())
        self.assertFalse(interval.hasNoBoundAtAll())
        self.assertTrue(interval.isMinBounded())
        self.assertTrue(interval.isMaxBounded())

        self.assertTrue(interval.isInBounds(-1.0))
        self.assertTrue(interval.isInBounds(1.0))
        self.assertTrue(interval.isInBounds(0.0))
        self.assertFalse(interval.isInBounds(1.1))
        self.assertFalse(interval.isInBounds(-1.1))

    def test_intInterval(self):
        interval = bounds.IntInterval(-10,10)
        self.assertTrue(interval.isBounded())
        self.assertFalse(interval.hasNoBoundAtAll())
        self.assertTrue(interval.isMinBounded())
        self.assertTrue(interval.isMaxBounded())

        self.assertTrue(interval.isInBounds(-10))
        self.assertTrue(interval.isInBounds(10))
        self.assertTrue(interval.isInBounds(0))
        self.assertFalse(interval.isInBounds(11))
        self.assertFalse(interval.isInBounds(-11))



    def test_Interval_fold(self):
        interval = bounds.RealInterval(-1.0,1.0)

        self.assertAlmostEqual(interval.foldsInBounds(1.1), 0.9)
        self.assertAlmostEqual(interval.foldsInBounds(-1.1), -0.9)
        self.assertAlmostEqual(interval.foldsInBounds(40.0), 0.0)

        interval = bounds.IntInterval(-10,10)

        self.assertAlmostEqual(interval.foldsInBounds(11), 9)
        self.assertAlmostEqual(interval.foldsInBounds(-11), -9)
        self.assertAlmostEqual(interval.foldsInBounds(400), 0)


    def test_Interval_truncate(self):
        interval = bounds.RealInterval(-1.0,1.0)

        self.assertAlmostEqual(interval.truncate(1.1), 1.0)
        self.assertAlmostEqual(interval.truncate(-1.1), -1.0)

        interval = bounds.IntInterval(-10,10)
        self.assertAlmostEqual(interval.truncate(11), 10)
        self.assertAlmostEqual(interval.truncate(-11), -10)


    def test_Interval_minmax(self):
        interval = bounds.RealInterval(-1.0,1.0)

        self.assertEqual(interval.minimum(),-1.0)
        self.assertEqual(interval.maximum(),1.0)
        self.assertEqual(interval.range(),2.0)

        interval = bounds.IntInterval(-10,10)

        self.assertEqual(interval.minimum(),-10)
        self.assertEqual(interval.maximum(),10)
        self.assertEqual(interval.range(),20)


    def test_realVecBounds1(self):
        b = bounds.RealVectorBounds(3,-1.0,1.0)

        self.assertTrue(b.isBounded())
        for i in range(3):
            self.assertTrue(b.isBounded(i))
            self.assertFalse(b.hasNoBoundAtAll(i))
            self.assertTrue(b.isMinBounded(i))
            self.assertTrue(b.isMaxBounded(i))
            self.assertAlmostEqual(b.foldsInBounds(i,1.0+i*0.1),1.0-i*0.1)
            self.assertAlmostEqual(b.truncate(i,1.0+i*0.1),1.0)

        v = np.asarray([1.0+0.1*i for i in range(3)])
        b.foldsInBounds(v)
        np.testing.assert_almost_equal(v,np.asarray([1.0,0.9,0.8]))

        v = np.asarray([1.0+0.1*i for i in range(3)])
        b.truncate(v)
        np.testing.assert_almost_equal(v,np.asarray([1.0,1.0,1.0]))

        self.assertTrue(b.uniform(0) > -1.0)
        self.assertTrue(b.uniform(1) < 1.0)


    def test_intVecBounds1(self):
        b = bounds.IntVectorBoundsBase(3,bounds.IntInterval(-10,10))
        #
        self.assertTrue(b.isBounded())
        for i in range(3):
            self.assertTrue(b.isBounded(i))
            self.assertFalse(b.hasNoBoundAtAll(i))
            self.assertTrue(b.isMinBounded(i))
            self.assertTrue(b.isMaxBounded(i))
            self.assertAlmostEqual(b.foldsInBounds(i,10+i),10-i)
            self.assertAlmostEqual(b.truncate(i,10+i),10)

        v = np.asarray([10+i for i in range(3)])
        b.foldsInBounds(v)
        np.testing.assert_almost_equal(v,np.asarray([10,9,8]))
        #
        v = np.asarray([10+i for i in range(3)])
        b.truncate(v)
        np.testing.assert_almost_equal(v,np.asarray([10,10,10]))

        self.assertTrue(b.uniform(0) > -10)
        self.assertTrue(b.uniform(1) < 10)


    def test_realVecBounds2(self):
        b = bounds.RealVectorBounds(3,bounds.RealInterval(-1.0,1.0))

        self.assertTrue(b.isBounded())
        for i in range(3):
            self.assertTrue(b.isBounded(i))
            self.assertFalse(b.hasNoBoundAtAll(i))
            self.assertTrue(b.isMinBounded(i))
            self.assertTrue(b.isMaxBounded(i))
            self.assertAlmostEqual(b.foldsInBounds(i,1.0+i*0.1),1.0-i*0.1)
            self.assertAlmostEqual(b.truncate(i,1.0+i*0.1),1.0)

        v = np.asarray([1.0+0.1*i for i in range(3)])
        b.foldsInBounds(v)
        np.testing.assert_almost_equal(v,np.asarray([1.0,0.9,0.8]))

        v = np.asarray([1.0+0.1*i for i in range(3)])
        b.truncate(v)
        np.testing.assert_almost_equal(v,np.asarray([1.0,1.0,1.0]))

        self.assertTrue(b.uniform(0) > -1.0)
        self.assertTrue(b.uniform(1) < 1.0)


    def test_realVecBounds3(self):
        lb = np.asarray([-1.0*i for i in range(1,4)])
        ub = np.asarray([i*1.0 for i in range(1,4)])
        b = bounds.RealVectorBounds(lb,ub)

        self.assertTrue(b.isBounded())
        for i in range(0,3):
            self.assertTrue(b.isBounded(i))
            self.assertFalse(b.hasNoBoundAtAll(i))
            self.assertTrue(b.isMinBounded(i))
            self.assertTrue(b.isMaxBounded(i))
            self.assertAlmostEqual(b.foldsInBounds(i,(i+1)*1.0+0.1),(i+1)*1.0-0.1)
            self.assertAlmostEqual(b.truncate(i,(i+1)*1.0+0.1),(i+1)*1.0)

        v = np.asarray([i*1.0+0.1 for i in range(1,4)])
        b.foldsInBounds(v)
        np.testing.assert_almost_equal(v,np.asarray([0.9,1.9,2.9]))

        v = np.asarray([i*1.0+0.1 for i in range(1,4)])
        b.truncate(v)
        np.testing.assert_almost_equal(v,ub)

        self.assertTrue(b.uniform(0) < 1.0)
        self.assertTrue(b.uniform(1) < 2.0)
        self.assertTrue(b.uniform(2) < 3.0)


if __name__ == '__main__':
    unittest.main()
