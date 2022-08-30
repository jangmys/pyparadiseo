"""
Swarm
"""
from pyparadiseo import config,utils

from ._core import RealParticlePop

def empty(stype=None):
    """
    create empty swarm

    Parameters
    ----------
    type : str
        Particle type

    Returns
    -------
    A swarm (eoPop)
    """
    # if stype is None:
    #     stype=config._SOLUTION_TYPE

    return RealParticlePop()


def from_init(pop_size,f_init,stype=None):
    """
    create a population of pop_size solutions initialized by initializer

    Parameters
    ----------
    pop_size : int
        Population size
    f_init : eoInit
        An Initializer
    type : str
        Solution type

    Returns
    --------
    A swarm (eoPop)
    """
    return RealParticlePop(pop_size,f_init)
