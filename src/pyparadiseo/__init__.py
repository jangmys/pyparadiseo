from pyparadiseo import eo, mo, moeo

from .core import *

from ._core import FitnessTraits

from ._core import Solution
from ._core import RealSolution
from ._core import BinarySolution

from ._core import Pop
from ._core import RealPop
from ._core import BinaryPop


from ._core import ObjectiveVectorTraits
from ._core import ObjectiveVector

#  EO RNG
from ._core import eoRng
from ._core import rng

#ObjectiveVector.setup(0,[])

def create_population(pop_size=None,f_init=None,stype="generic"):
    """
    create and initialize a Population class object

    Parameters
    ----------
    stype : Solution type
        "generic" (default)
        "real"
        "perm"
        "particle"

    pop_size, optional : int
    f_init : initialization function

    Returns
    -------
    A Population
    """
    if stype == "generic":
        if pop_size is None:
            return Pop()
        else:
            return Pop(pop_size,f_init)
    if stype == "binary":
        if pop_size is None:
            return BinaryPop()
        else:
            return BinaryPop(pop_size,f_init)


def create_solution(encoding=None,stype="generic"):
    """
    create and initialize a Solution

    Parameters
    ----------
    stype : Solution type
    encoding : object

    Returns
    -------
    A Solution
    """
    if stype == "generic":
        if encoding is None:
            return Solution()
        else:
            return Solution(encoding)
    if stype == "real":
        return RealSolution()
