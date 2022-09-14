"""
Variation Operators

* eoMonOp
    - mutation
* eoBinOp
    - ...
* eoQuadOp
    - crossover

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
from ._core import StandardBitMutation
from ._core import UniformBitMutation
from ._core import ConditionalBitMutation
from ._core import ShiftedBitMutation
from ._core import NormalBitMutation
from ._core import FastBitMutation

from ._core import OneBitFlip
from ._core import DetBitFlip
from ._core import DetSingleBitFlip

from ._core import BitMutation
from ._core import BitInversion
from ._core import BitNext
from ._core import BitPrevious

from ._core import OnePtBitCrossover
from ._core import UBitXover
from ._core import NPtsBitXover
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

    Any Python callable that modifies athe solution encoding and returns a ``bool`` can be wrapped in a eoMonOp.

    Parameters
    ==========
    mutate : callable
        signature must be compatible with solution definition
    stype : {'gen','bin','real','perm'}, optional
        solution type

    Returns
    =======
    pyMonOp
        an eoMonOp wrapping `mutate`

    Example
    =======
    >>> def bitflip(bits):
    >>>     flip = random.randint(0,len(bits))
    >>>     bits[flip] = not bits[flip]
    >>>     return True
    >>>
    >>> from pyparadiseo import operator
    >>> my_mutation = operator.make_mutation(bitflip)

    As a class

    >>> class bitflip():
    >>>     def __init__(self,data):
    >>>         self.data = data
    >>>         self.counter = 0
    >>>     def __call__(bits):
    >>>         print("hello..."+self.data)
    >>>         self.counter += 1
    >>>         flip = random.randint(0,len(bits))
    >>>         bits[flip] = not bits[flip]
    >>>         return True
    >>>
    >>> mymut = operator.make_mutation(bitflip("bouh"))
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


### ==================================================
### BINARY
### ==================================================
def standard_bit_mutation(rate=0.5):
    """
    Standard bit mutation with mutation rate p:
        choose k from the binomial distribution Bin(n,p) and apply flip_k(x).

    Returns
    =======
    eoMonOp<BinarySolution>
    """
    return StandardBitMutation(rate)


def uniform_bit_mutation(rate=0.5):
    """
    Uniform bit mutation with mutation rate p:
        choose k from the uniform distribution U(0,n) and apply flip_k(x).

    Returns
    =======
    eoMonOp<BinarySolution>
    """
    return UniformBitMutation(rate)



def bit_flip(flip_prob=0.01,normalize=False):
    """
    flip each bit with probability `flip_prob`

    Parameters
    ==========
    flip_prob : real, default=0.01
        mutation rate (probability for each bit to be flipped)
    normalize : bool, default=False
        normalize `flip_prob` with respect to chromosome size

    Returns
    =======
    eoMonOp<BinarySolution>
    """
    if nbits==1:
        return OneBitFlip()
    elif allow_duplicates:
        return DetBitFlip(nbits)
    else:
        return DetSingleBitFlip(nbits)


def bit_flip_n(nbits=1,allow_duplicates=True):
    """
    flip a fixed number `nbits` of randomly chosen bits

    Parameters
    ==========
    nbits : int, default=1
        number of bits to flip
    allow_duplicates : bool, default=True
        allow flipping the same bit multiple times (if `nbits`>1)

    Returns
    =======
    eoMonOp<BinarySolution>
    """
    if nbits==1:
        return OneBitFlip()
    elif allow_duplicates:
        return DetBitFlip(nbits)
    else:
        return DetSingleBitFlip(nbits)


def bit_inversion():
    """
    invert bits in a randomly chosen range
    """
    return BitInversion()


def bit_next():
    """
    next bitstring in lexicographic order
    """
    return BitNext()


def bit_previous():
    """
    previous bitstring in lexicographic order
    """
    return BitPrevious()


################# Bit Crossovers

def one_point_bit_cx():
    """
    one point crossover for binary solutions

    choose crossover-site randomly and swap ranges [begin,site)
    """
    return OnePtBitCrossover()


def n_point_bit_cx(n_points=2):
    """
    n-point crossover for binary solutions

    Parameters
    ==========
    n_points : int, default=2
    """
    return NPtsBitXover(n_points)


def uniform_bit_cx(preference=0.5):
    """
    uniform crossover for binary solutions

    for each component, swap bits with probability `preference` (default=0.5)

    Parameters
    ==========
    preference : real, default=0.5
    """
    return UBitXover(preference)


### ==================================================
### REAL
### ==================================================
def uniform_real_mutation(epsilon, bounds=None, p_change=1.0):
    """
    uniform crossover for real solutions

    Parameters
    ===========
    epsilon : real
        range for uniform mutation, scaled to range if bounds are provided
    bounds : eoRealVectorBounds, optional
        bounds of the domain
    p_change : real, default=1.0
        probability of changing a solution component
    """
    if bounds == None:
        return UniformMutation(epsilon,p_change)
    else:
        return UniformMutation(bounds,epsilon,p_change)


def segment_cx(bounds=None,alpha=0.0):
    """
    Segment Crossover
    """
    if bounds == None:
        return SegmentCrossover(alpha)
    else:
        return SegmentCrossover(bounds,alpha)


def hypercube_cx(bounds=None,alpha=0.0):
    """
    Hypercube Crossover
    """
    if bounds == None:
        return HypercubeCrossover(alpha)
    else:
        return HypercubeCrossover(bounds,alpha)

def uniform_real_cx(preference=0.5):
    """
    Uniform crossover for real solutions

    For each component exchange values with probability `preference`
    """
    return RealXover(preference)


def sbx(bounds=None,eta=1.0):
    """
    SBX
    """
    if bounds == None:
        return SBXCrossover(eta)
    else:
        return SBXCrossover(bounds,eta)
