from pyparadiseo import config,utils

# from ._core import Pop
# from ._core import RealPop
# from ._core import BinaryPop

class _Pop():
    def __new__(cls,size=None,f_init=None,type=None):
        if type is None:
            type=config._SOLUTION_TYPE

        class_ = None
        if type == 'gen':
            class_ = utils.get_class("Pop")
        if type == 'bin':
            class_ = utils.get_class("BinaryPop")
        if type == 'real':
            class_ = utils.get_class("RealPop")

        if size is None:
            return class_()
        elif f_init is None:
            print("_Pop : need f_init")
        else:
            return class_(size,f_init)
