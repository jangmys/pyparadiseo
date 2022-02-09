from pyparadiseo.eo import replacement

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoReplacement(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(replacement):
            if inspect.isclass(obj):
                if name != "Replacement":
                    self.assertEqual(obj.__bases__[0].__name__,"eoReplacement")
