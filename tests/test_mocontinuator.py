from pyparadiseo.mo import continuator

import unittest
import inspect

#TODO : for now, just testing imports and base class name ...
class test_mocontinuator(unittest.TestCase):
    def test_bases(self):
        for name, obj in inspect.getmembers(continuator):
            if inspect.isclass(obj):
                if name != "Continuator":
                    self.assertEqual(obj.__bases__[0].__name__,"moContinuator")
