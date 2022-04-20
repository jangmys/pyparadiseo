from pyparadiseo import eo, mo, moeo

# from .operator import *
# from .initializer import *
# from .evaluator import *
# from .bounds import *

from .core import *

from ._core import FitnessTraits

### basic stuff common to MO,EO and MOEO

#SOLUTION (PyEOT)
from ._core import Solution
from ._core import Pop
from ._core import ObjectiveVectorTraits
from ._core import ObjectiveVector

# from ._core import RealParticle
# from ._core import RealSolution

# from ._core import testSolution

#  EO RNG
from ._core import eoRng
from ._core import rng

#ObjectiveVector.setup(0,[])
