"""
PyParadisEO
"""
from pyparadiseo import eo, mo, moeo

from pyparadiseo import evaluator
from pyparadiseo import initializer
from pyparadiseo import operator
from pyparadiseo import population
from . import utils
from . import config

#  EO RNG
from ._core import eoRng
from ._core import rng

#import submodules
from . import solution
from . import bounds
from . import population as pop #pop
from . import initializer as init #init
from . import evaluator as eval #eval

from .config import *
