from ._core import FitnessTraits

def set_maximize_fitness():
    # this is default
    FitnessTraits.set_minimizing(False)

def set_minimize_fitness():
    FitnessTraits.set_minimizing(True)
