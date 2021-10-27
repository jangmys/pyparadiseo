from pyparadiseo.eo import algo

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_moalgo(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(algo):
            if inspect.isclass(obj):
                self.assertEqual(obj.__bases__[0].__name__,"eoAlgo")
