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
    if stype is None:
        stype=config._SOLUTION_TYPE

    return RealParticlePop()
