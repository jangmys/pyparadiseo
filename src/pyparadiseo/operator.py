"""
Genetic Operators
"""
# EO OPS ....

from pyparadiseo import config,utils

# Base classes C++
from ._core import eoMonOp as MonOp
from ._core import eoBinOp as BinOp
from ._core import eoQuadOp as QuadOp

# Python injectors
from ._core import pyMonOp
from ._core import pyBinOp
from ._core import pyQuadOp

from ._core import eoSGAGenOp as SGAGenOp


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
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        class_ = utils.get_class("pyMonOp")
        return class_(mutate)
    else:
        NotImplementedError("Not yet implemented")


def make_crossover(xover,stype=None):
    """
    make crossover from callable
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    if stype == 'gen':
        class_ = utils.get_class("pyQuadOp")
        return class_(xover)
    else:
        NotImplementedError("Not yet implemented")
