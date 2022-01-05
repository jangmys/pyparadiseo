from pyparadiseo.eo import selector

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoSelector(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(selector):
            if inspect.isclass(obj):
                self.assertIn(obj.__bases__[0].__name__, {"eoSelect","eoSelectOne","instance"})
