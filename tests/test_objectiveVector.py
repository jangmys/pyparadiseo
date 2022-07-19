from pyparadiseo._core import ObjectiveVectorTraits
from pyparadiseo._core import ObjectiveVector

import unittest


class TestObjVec(unittest.TestCase):
    def test_ctor(self):
        objvec = ObjectiveVector()
        self.assertEqual(objvec[0],0.0)
        objvec = ObjectiveVector([1,2])
        self.assertEqual(objvec[0],1.0)

    def test_setup(self):
        objvec = ObjectiveVector()
        objvec.setup(2,[True,True])

        self.assertEqual(objvec.nObjectives(),2)
        self.assertTrue(objvec.minimizing(0))
        self.assertTrue(objvec.minimizing(1))
        self.assertFalse(objvec.maximizing(0))
        self.assertFalse(objvec.maximizing(1))

    def test_assign(self):
        objvec = ObjectiveVector()
        objvec.setup(2,[True,False])
        objvec = [1,2]
        self.assertEqual(objvec[0],1)
        self.assertEqual(objvec[1],2)
        # self.assertTrue(ObjectiveVector.minimizing(0))
        # self.assertTrue(ObjectiveVector.minimizing(1))
        objvec[0]=3
        self.assertEqual(objvec[0],3)

    def test_copy_ctor(self):
        objvec = ObjectiveVector()
        objvec.setup(2,[True,True])
        objvec = [1,2]

        objvec2 = ObjectiveVector(objvec)
        objvec[0]=0
        objvec[1]=0

        self.assertEqual(objvec2[0],1)
        self.assertEqual(objvec2[1],2)

    def test_eq(self):
        objvec = ObjectiveVector()
        objvec.setup(2,[True,True])
        objvec = [1,2]

        objvec2 = ObjectiveVector(objvec)
        objvec2.setup(2,[True,False])
        objvec2 = [1,2]
        self.assertTrue(objvec,objvec2)

    def test_dom(self):
        ObjectiveVectorTraits.setup(2,[True,True])
        objvec = ObjectiveVector()
        objvec = [1,2]

        objvec2 = ObjectiveVector(objvec)
        objvec2 = [2,3]
        # self.assertTrue(objvec.dominates(objvec2))

if __name__ == '__main__':
    ObjectiveVector.setup(2,[True,False])

    objvec = ObjectiveVector([1,2])
    objvec2 = ObjectiveVector([2,1])

    print(objvec < objvec2)
    print(objvec2 < objvec)
    print(objvec > objvec2)
    print(objvec == objvec2)

    print(objvec.dominates(objvec2))
    print("#"*10)

    ObjectiveVector.setup(2,[False,True])

    objvec = ObjectiveVector([1,2])
    objvec2 = ObjectiveVector([2,1])

    print(objvec < objvec2)
    print(objvec2 < objvec)
    print(objvec > objvec2)
    print(objvec == objvec2)

    print(objvec.dominates(objvec2))

    print(ObjectiveVector.nObjectives())


    unittest.main()
