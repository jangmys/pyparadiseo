from pyparadiseo.mo import continuator

from pyparadiseo import solution
from pyparadiseo import config
from pyparadiseo import _core


import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_moCheckpoint(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')
        pass
    def tearDown(self):
        pass
    def test_contVector(self):
        contvec = _core._ContinuatorVec()

        contvec.append(continuator.always_true())
        contvec.append(continuator.max_iterations(20,True))

        self.assertEqual(len(contvec),2)
    def test_checkpt(self):
        chk=_core.moCheckpoint(continuator.always_true(),2)

        chk.add(continuator.max_iterations(20,True))

        sol = solution.empty()
        print(chk(sol))
        #
        #
        # print('hi')
        #
        while(chk(sol)):
            print('hi')


if __name__=='__main__':
    unittest.main()
