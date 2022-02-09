from pyparadiseo.mo import algo

# from pyparadiseo.mo.algo import SimpleHC
# from pyparadiseo.mo.algo import FirstImprHC
# from pyparadiseo.mo.algo import RandomBestHC
# from pyparadiseo.mo.algo import NeutralHC
# from pyparadiseo.mo.algo import RandomSearch
# from pyparadiseo.mo.algo import RandomWalk
# from pyparadiseo.mo.algo import RandomNeutralWalk

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_moalgo(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(algo):
            if inspect.isclass(obj):
                if name != "LocalSearch":
                    self.assertEqual(obj.__bases__[0].__name__,"moLocalSearch")
