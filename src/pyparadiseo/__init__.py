from pyparadiseo import eo, mo, moeo

from pyparadiseo import bounds
from pyparadiseo import evaluator
from pyparadiseo import initializer
from pyparadiseo import operator
from pyparadiseo import factory

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
