from pyparadiseo import config,utils

from ._core import eoInit
#
from ._core import pyeoInit as Init
from ._core import BinaryInit

from ._core import BinarySolInit
from ._core import RealBoundedInit
from ._core import PermutationInit


class _Init():
    def __new__(cls,init_f=None,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = None

        if type == 'gen':
            class_ = utils.get_class("pyeoInit")
        if type == 'bin':
            class_ = utils.get_class("BinarySolInit")
        if type == 'real':
            class_ = utils.get_class("RealBoundedInit")
        if type == 'perm':
            class_ = utils.get_class("PermutationInit")

        if init_f is not None:
            return class_(init_f)
        else:
            return class_()
