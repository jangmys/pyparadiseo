import pyparadiseo as pp

from pyparadiseo import ObjectiveVectorTraits

import unittest


class TestObj(unittest.TestCase):
    def test_MinMaxMin(self):
        pp.setup_objectives(3,[True,False,True])

        self.assertEqual(ObjectiveVectorTraits.nb_objectives(),3)
        self.assertTrue(ObjectiveVectorTraits.minimizing(0))
        self.assertTrue(ObjectiveVectorTraits.maximizing(1))
        self.assertTrue(ObjectiveVectorTraits.minimizing(2))

    def test_MinMin(self):
        ObjectiveVectorTraits.setup(2,[True,True])

        self.assertEqual(ObjectiveVectorTraits.nb_objectives(),2)
        self.assertTrue(ObjectiveVectorTraits.minimizing(0))
        self.assertTrue(ObjectiveVectorTraits.minimizing(1))
        self.assertFalse(ObjectiveVectorTraits.maximizing(0))
        self.assertFalse(ObjectiveVectorTraits.maximizing(1))

    def test_MinMax(self):
        ObjectiveVectorTraits.setup(2,[True,False])

        self.assertEqual(ObjectiveVectorTraits.nb_objectives(),2)
        self.assertTrue(ObjectiveVectorTraits.minimizing(0))
        self.assertFalse(ObjectiveVectorTraits.minimizing(1))
        self.assertFalse(ObjectiveVectorTraits.maximizing(0))
        self.assertTrue(ObjectiveVectorTraits.maximizing(1))

    def test_Min(self):
        # second arg must be iterable !!!
        # moeoObjectiveVectorTraits.setup(1,True)
        ObjectiveVectorTraits.setup(1,[True])
        self.assertEqual(ObjectiveVectorTraits.nb_objectives(),1)
        self.assertTrue(ObjectiveVectorTraits.minimizing(0))
        self.assertFalse(ObjectiveVectorTraits.maximizing(0))

    def test_Max(self):
        # second arg must be iterable !!!
        ObjectiveVectorTraits.setup(1,[False])
        self.assertEqual(ObjectiveVectorTraits.nb_objectives(),1)
        self.assertFalse(ObjectiveVectorTraits.minimizing(0))
        self.assertTrue(ObjectiveVectorTraits.maximizing(0))


if __name__ == '__main__':
    unittest.main()
