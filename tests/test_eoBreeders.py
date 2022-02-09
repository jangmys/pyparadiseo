from pyparadiseo.eo import breeders

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoBreeders(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(breeders):
            if name == "Breed":
                self.assertEqual(obj.__bases__[0].__name__,"instance")
            elif inspect.isclass(obj):
                self.assertEqual(obj.__bases__[0].__name__,"eoBreed")
