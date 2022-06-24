"""
eoTransform transforms a population by applying genetic operators on it.

base : eoTransform.h

__call__(pop) --> void
"""
from pyparadiseo import config,utils

from .._core import eoSGATransform as SGATransform

def SGA(crossover, c_proba, mutate, m_proba, stype=None):
    """eoSGATransform: transforms a population using genetic operators.
     * It does it exactly as class eoSGA, i.e. only accepts
     *    quadratic crossover and unary mutation
     * It is here mainly for tutorial reasons
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSGATransform"+config.TYPES[stype])
    return class_(crossover,c_proba,mutate,m_proba)
