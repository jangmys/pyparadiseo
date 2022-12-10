"""
Config
^^^^^^^

set global pyparadseo parameters
"""
__all__=['set_solution_type','set_maximize_fitness','set_minimize_fitness']

from ._core import FitnessTraits
from ._core import ObjectiveVectorTraits

TYPES={
    'gen' : '',
    'bin' : 'Bin',
    'real' : 'Real',
    'perm' : 'Perm',
    'real-pso' : 'RealPart',
    'int-pso' : 'IntPart'
}

_SOLUTION_TYPE = 'gen'

def set_solution_type(sol_type):
    """set solution type globally"""
    global _SOLUTION_TYPE
    if sol_type not in TYPES.keys():
        print("Solution type must be one of the following : {}".format(list(TYPES.keys())))
    else:
        _SOLUTION_TYPE = sol_type


#setter for fitness traits
def set_maximize_fitness():
    """sets fitness maximization (default)"""
    FitnessTraits.set_minimizing(False)

def set_minimize_fitness():
    """sets fitness minimization (maximize is default)"""
    FitnessTraits.set_minimizing(True)

def is_minimizing():
    """check if pyparadiseo is minimizing"""
    return FitnessTraits.is_minimizing()

def setup_objectives(nObj,bObj):
    """set number of objectives and optimization directions

    Parameters
    ----------
    nObj : int
        The first parameter.
    bObj : list of bool
        The second parameter.
    """
    ObjectiveVectorTraits.setup(nObj,bObj)
