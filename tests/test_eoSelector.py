import pyparadiseo

from pyparadiseo import Pop
from pyparadiseo import initializer
from pyparadiseo.eo import selector

from pyparadiseo.eo import select_one

import numpy as np
import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoSelector(unittest.TestCase):
    def setUp(self):
        self.init = initializer.initializer(lambda : np.random.randint(0,2,10))
        self.pop = Pop(10,self.init)
        for i in range(len(self.pop)):
            self.pop[i].fitness = i

    def test_ctor(self):
        #abstract : should not be able to construct this?!
        sel = select_one.SelectOne()
        self.assertTrue(callable(sel))

        sel = select_one.det_tournament()
        self.assertTrue(callable(sel))

        sel = select_one.det_tournament(42)
        self.assertTrue(callable(sel))

        sel = select_one.truncated(sel,0.7)
        self.assertTrue(callable(sel))

        sel = select_one.random()
        self.assertTrue(callable(sel))

        sel = select_one.round_robin()
        self.assertTrue(callable(sel))

        sel = select_one.sequential()
        self.assertTrue(callable(sel))

        sel = select_one.sequential(True)
        self.assertTrue(callable(sel))

        sel = select_one.elite_sequential()
        self.assertTrue(callable(sel))

    def do_test(self,selector):
        # rng = pyparadiseo.rng()
        pyparadiseo.rng().reseed(42)
        nTries = 5000

        print('*'*20, "Testing", str(selector.__class__), '*'*20)
        print('ind#\t', "/",nTries,"\t", "%\t")
        selection = [0. for i in range(len(self.pop))]

        for i in range(nTries):
            selection[ int(selector(self.pop).fitness) ] += 1

        for i in range(len(self.pop)):
            print(i,"\t", selection[i],"\t", selection[i] / nTries)

        return selection, nTries

    def test_DetTournament(self):
        sel = select_one.det_tournament()
        self.do_test(sel)
        sel = select_one.det_tournament(1)
        self.do_test(sel)
        sel = select_one.det_tournament(2)
        self.do_test(sel)
        sel = select_one.det_tournament(3)
        self.do_test(sel)
        sel = select_one.det_tournament(10)
        self.do_test(sel)



if __name__ == "__main__":
    unittest.main()
