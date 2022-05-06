"""
eoTransform transforms a population by applying genetic operators on it.

base : eoTransform.h

__call__(pop) --> void
"""
from .._core import eoTransform as Transform

from .._core import eoSGATransform as SGATransform


class _SGATransform():
    """eoSGATransform: transforms a population using genetic operators.
     * It does it exactly as class eoSGA, i.e. only accepts
     *    quadratic crossover and unary mutation
     * It is here mainly for tutorial reasons
    """
    def __new__(cls, crossover, c_proba, mutate, m_proba, type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSGATransform"+config.TYPES[type])
        return(crossover,c_proba,mutate,m_proba)
