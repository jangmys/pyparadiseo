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
from ._core import eoMonOp
from ._core import eoBinOp
from ._core import eoQuadOp

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
from ._core import PolynomialMutation

from ._core import TwoPtCrossover
from ._core import SegmentCrossover
from ._core import HypercubeCrossover
from ._core import SBXCrossover
from ._core import RealUXover

__all__=['eoMonOp','eoBinOp','eoQuadOp','make_mutation',
'make_crossover',
'standard_bit_mutation',
'uniform_bit_mutation',
'conditional_bit_mutation',
'shifted_bit_mutation',
'normal_bit_mutation',
'fast_bit_mutation',
'bit_flip_n',
'bit_inversion',
'bit_next',
'bit_previous',
'one_point_bit_cx',
'n_point_bit_cx',
'uniform_bit_cx',
'uniform_real_mutation',
'polynomial_mutation',
'two_point_crossover',
'segment_cx',
'hypercube_cx',
'uniform_real_cx',
'sbx'
]




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

    class_ = utils.get_class("pyMonOp"+config.TYPES[stype])

    return class_(mutate)


def make_crossover(xover,stype=None):
    """
    make crossover from callable
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("pyQuadOp"+config.TYPES[stype])

    return class_(xover)


### ==================================================
### ----------------------BINARY----------------------
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
    Uniform bit mutation with mutation rate p

    choose k from the uniform distribution U(0,n) and apply flip_k(x).

    Returns
    =======
    eoMonOp<BinarySolution>
    """
    return UniformBitMutation(rate)


def conditional_bit_mutation(rate=0.5):
    """
    Conditional standard bit mutation with mutation rate p

    choose k from the binomial distribution Bin(n,p) until k >0
    and apply flip_k(x).
    This is identical to sampling k from the conditional binomial
    distribution Bin>0(n,p) which re-assigns the probability to sample a 0 proportionally to all values i ∈ [1..n].
    """
    return ConditionalBitMutation(rate)


def shifted_bit_mutation(rate=0.5):
    """
    Shifted standard bit mutation with mutation rate p.

    choose k from the binomial distribution Bin(n,p).
    When k= 0, set k= 1. Apply flip_k(x).

    This is identical to sampling k from the conditional binomial
    distribution Bin0→1(n,p) which re-assigns the probability to
    sample a 0 to sampling k= 1.
    """
    return ShiftedBitMutation(rate)


def normal_bit_mutation(rate=0.5,variance=1):
    """
    Mutation which size is sample in a gaussian.

    sample k from the normal distribution N(pn,σ^2) and apply flip_k(x).

    From:
    Furong Ye, Carola Doerr, and Thomas Back.
    Interpolating local and global search by controllingthe variance of standard bit mutation.
    In 2019 IEEE Congress on Evolutionary Computation(CEC), pages 2292–2299.

    In contrast to standard bit mutation, this operators allows to scale the variance of the mutation strength independently of the mean.
        """
    return NormalBitMutation(rate,variance)


def fast_bit_mutation(rate=0.5,beta=1.5):
    """
    Fast mutation which size is sampled from an adaptive power law.

    From:
    Benjamin Doerr, Huu Phuoc Le, Régis Makhmara, and Ta Duy Nguyen.
    Fast genetic algorithms.
    In Proc. of Genetic and Evolutionary Computation Conference (GECCO’17), pages 777–784.ACM, 2017.
    """
    return FastBitMutation(rate,beta)






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
### -----------------------REAL-----------------------
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
        return UniformMutation(epsilon=epsilon,p_change=p_change)
    else:
        return UniformMutation(bounds=bounds,epsilon=epsilon,p_change=p_change)


def polynomial_mutation(proba=0.2,eta=50,bounds=None):
    """Polynomial Mutation as in https://www.egr.msu.edu/~kdeb/papers/k2012016.pdf"""
    if bounds == None:
        return PolynomialMutation(proba,eta)
    else:
        return PolynomialMutation(bounds,proba,eta)


def two_point_crossover():
    """Two point (real) crossover

    - pick two random indices i1,i2 in [0,min_size(a,b)] (i1<i2)
    - swap ranges [i1,i2[

    Example
    =======
    cross([A,B,C,D,E],[1,2,3,4,5]) with i1=1,i2=3 ==>

    ([A,2,3,D,E],[1,B,C,4,5])
    """
    return TwoPtCrossover()


def segment_cx(bounds=None,alpha=0.0,stype='real'):
    """
    Segment Crossover
    """
    klass = None
    if stype == 'real':
        klass = SegmentCrossover

    if bounds == None:
        return klass(alpha)
    else:
        return klass(bounds,alpha)


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
