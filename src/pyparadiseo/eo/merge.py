"""
    Mergers

    base : eoMerge.h

    __call__(old_pop,new_pop) --> None

     * Merges the old population (first argument), with the new generation
     *
     * Its signature is exactly
     * that of the selection base eoSelect, but its purpose is to merge the
     * two populations into one (the second argument).
     * Note that the algorithms assume that the second argument denotes the
     * next generation.
 """
from pyparadiseo import config,utils


def merge(fun,stype=None):
    """
    make eoMerge from Python function

    Python callback function takes two arguments old_pop and new_pop.
    Returns None. The first argument (old_pop) should remain unchanged
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    base_class = utils.get_class("eoMerge"+config.TYPES[stype])

    class pyMerge(base_class):
        def __init__(self,fun):
            base_class.__init__(self)
            self.merge_fun = fun
        def __call__(self,old_pop,new_pop):
            self.merge_fun(old_pop,new_pop)

    return pyMerge(fun)


def elitist(rate,interpret_as_rate=True,stype=None):
    """
    Straightforward elitism class, specify the number of individuals to copy
    into new geneneration or the rate w.r.t. pop size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoElitism"+config.TYPES[stype])

    return class_(rate,interpret_as_rate)


def no_elitist(stype=None):
    """No Elitism (rate==0)
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    return utils.get_class("eoNoElitism"+config.TYPES[stype])()


def plus(stype=None):
    """
    copies entire population to offspring

    new size of offspring is pop.size()+offsring.size()
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    return utils.get_class("eoPlus"+config.TYPES[stype])()
