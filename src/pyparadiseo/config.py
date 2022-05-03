from ._core import FitnessTraits
from ._core import ObjectiveVectorTraits

TYPES={
    'gen' : '',
    'bin' : 'Bin',
    'real' : 'Real'
}

_SOLUTION_TYPE = 'gen'

def set_solution_type(sol_type):
    global _SOLUTION_TYPE
    if sol_type not in TYPES.keys():
        print("Solution type must be one of the following : {}".format(list(TYPES.keys())))
    else:
        _SOLUTION_TYPE = sol_type



#setter for fitness traits
def set_maximize_fitness():
    # this is default
    FitnessTraits.set_minimizing(False)

def set_minimize_fitness():
    FitnessTraits.set_minimizing(True)

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
