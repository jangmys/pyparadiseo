# EO OPS ....

# Base classes C++
from .core import eoMonOp as MonOp
from .core import eoBinOp as BinOp
from .core import eoQuadOp as QuadOp

# Python injectors
from .core import pyMonOp
from .core import pyBinOp
from .core import pyQuadOp

#binary ops
from .core import OneBitFlip
from .core import DetBitFlip
from .core import DetSingleBitFlip
from .core import BitMutation
from .core import OnePtBitCrossover
from .core import UBitCrossover
from .core import NPtsBitCrossover

# real ops
from .core import UniformMutation
from .core import DetUniformMutation
from .core import SegmentCrossover
from .core import HypercubeCrossover
