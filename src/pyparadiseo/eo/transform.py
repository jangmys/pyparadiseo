"""
Transform population

base : eoTransform.h

__call__(pop) --> void
"""
from pyparadiseo import config,utils

from .._core import eoTransform

__all__ = ['SGA','eoTransform']

def SGA(crossover, c_proba, mutate, m_proba, stype=None):
    """transform a population using genetic operators.

    First, apply crossover to pairs [ind_1,ind_2],...,[ind_(n-1),ind_n] with probability `c_proba` then mutate each indiidual with probability `m_proba`.

    Parameters
    ==========
    crossover : `eoQuadOp`
        crossover operator
    c_proba : float
        crossover probability
    mutate : `eoMonOp`
        mutation operator
    m_proba : float
        mutation probabiliy

    Notes
    =====
    Included mainly for tutorial reasons
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSGATransform"+config.TYPES[stype])
    return class_(crossover,c_proba,mutate,m_proba)
