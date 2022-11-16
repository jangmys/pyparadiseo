from pyparadiseo import config

from pyparadiseo.eo import breeders

from pyparadiseo.eo import selector,select_one
from pyparadiseo.eo import transform


import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoBreeders(unittest.TestCase):
    def setUp(self):
        pass

    def tearDown(self):
        pass

    def test_bases(self):
        for name, obj in inspect.getmembers(breeders):
            if name[0] == "_":
                continue
            if name == "Breed":
                self.assertEqual(obj.__bases__[0].__name__,"instance")
            elif inspect.isclass(obj):
                self.assertEqual(obj.__bases__[0].__name__,"eoBreed")


    def test_select_transform(self):
        pass




if __name__ == '__main__':
    unittest.main()
