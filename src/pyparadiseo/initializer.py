"""
Initializer : Generate Solutions

PyParadisEO algorithms use initializers to generate new solutions
"""
from pyparadiseo import config,utils

def random(size=0,stype=None,**kwargs):
    """
    Random initializer (for bin/real)

    Parameters
    ----------
    size : int
        solution length
    stype : str, optional
        solution type
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        raise TypeError("initializer.random : not availble for generic solution type")

    if stype == 'bin':
        class_ = utils.get_class("BinaryInit")
        return class_(size)

    if stype == 'real':
        class_ = utils.get_class("RealBoundedInitReal")
        if "bounds" in kwargs.keys():
            return class_(kwargs["bounds"])
        else:
            raise TypeError("need bounds for type 'real'")

    if stype == 'perm':
        class_ = utils.get_class("PermutationInit")
        if "startFrom" in kwargs.keys():
            return class_(size,kwargs["startFrom"])
        else:
            return class_(size)


####"decorator"
def initializer(init_fun,stype=None):
    """
    make initializer from function
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        class_ = utils.get_class("pyeoInit")
        return class_(init_fun)
    else:
        NotImplementedError("Not yet implemented")
