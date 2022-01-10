import pyparadiseo

from pyparadiseo import Init,Pop
from pyparadiseo.eo import selector

import numpy as np
import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoSelector(unittest.TestCase):
    def setUp(self):
        self.init = Init(lambda : np.random.randint(0,2,10))
        self.pop = Pop(10,self.init)
        for i in range(len(self.pop)):
            self.pop[i].fitness = i

    def test_bases(self):
        for name, obj in inspect.getmembers(selector):
            if inspect.isclass(obj):
                self.assertIn(obj.__bases__[0].__name__, {"eoSelect","eoSelectOne","instance"})
    def test_ctor(self):
        #abstract : should not be able to construct this
        sel = selector.SelectOne()
        self.assertTrue(callable(sel))

        sel = selector.DetTournamentSelect()
        self.assertTrue(callable(sel))

        sel = selector.DetTournamentSelect(42)
        self.assertTrue(callable(sel))

        sel = selector.TruncatedSelectOne(sel,0.7)
        self.assertTrue(callable(sel))

        sel = selector.RandomSelect()
        self.assertTrue(callable(sel))

        sel = selector.NoSelect()
        self.assertTrue(callable(sel))

        sel = selector.SequentialSelect()
        self.assertTrue(callable(sel))

        sel = selector.SequentialSelect(True)
        self.assertTrue(callable(sel))

        sel = selector.EliteSequentialSelect()
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
        sel = selector.DetTournamentSelect()
        self.do_test(sel)
        sel = selector.DetTournamentSelect(1)
        self.do_test(sel)
        sel = selector.DetTournamentSelect(2)
        self.do_test(sel)
        sel = selector.DetTournamentSelect(3)
        self.do_test(sel)
        sel = selector.DetTournamentSelect(10)
        self.do_test(sel)



if __name__ == "__main__":
    unittest.main()
