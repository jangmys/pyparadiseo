# EO OPS ....

# Base classes C++
from ._core import eoMonOp as MonOp
from ._core import eoBinOp as BinOp
from ._core import eoQuadOp as QuadOp

# Python injectors
from ._core import pyMonOp
from ._core import pyBinOp
from ._core import pyQuadOp

#binary ops (defined on BinarySolution)
from ._core import OneBitFlip
from ._core import DetBitFlip
from ._core import DetSingleBitFlip
from ._core import BitMutation
from ._core import BitInversion
from ._core import BitNext
from ._core import BitPrevious

from ._core import OnePtBitCrossover
from ._core import UBitCrossover
from ._core import NPtsBitCrossover
from ._core import BitGxOver

# real ops (defined on RealSolution)
from ._core import UniformMutation
from ._core import DetUniformMutation
from ._core import SegmentCrossover
from ._core import HypercubeCrossover
from ._core import SBXCrossover
from ._core import RealUXover






def make_mutation(mutate,stype=None):
    """
    make mutation from callable
    """
    pass

def make_crossover(xover,stype=None):
    """
    make crossover from callable
    """
    pass

# def real_crossover()
