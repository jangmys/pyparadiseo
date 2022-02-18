from ._core import FitnessTraits
from ._core import ObjectiveVectorTraits

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
