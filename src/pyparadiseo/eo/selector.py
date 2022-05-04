from pyparadiseo import config,utils

"""
Test docstring for eo.selector module

The eo.selector module contains selection operators.
"""

from .._core import eoSelect as Select
Select.__doc__= """Abstract base class for selectors.

    Extended description of function.

    Methods:
        __call__

    Args:
        arg1 (int): Description of arg1
        arg2 (str): Description of arg2

    Returns:
        bool: Description of return value

    """

Select.__init__.__doc__="""This is documentation for __init__

    blablabla

    Args:
        truc (int): letruc
"""


# from selectors.cpp
from .._core import eoDetSelect as DetSelect
from .._core import eoSelectMany as SelectMany
from .._core import eoSelectNumber as SelectNumber
from .._core import eoSelectPerc as SelectPerc
from .._core import eoTruncSelect as TruncSelect
from .._core import eoTruncatedSelectMany as TruncatedSelectMany

# from selectOne.cpp
from .._core import eoSelectOne as SelectOne
SelectOne.__doc__="this is the doc of SelectOne"


from .._core import eoDetTournamentSelect as DetTournamentSelect
from .._core import eoStochTournamentSelect as StochTournamentSelect
from .._core import eoProportionalSelect as ProportionalSelect
from .._core import eoTruncatedSelectOne as TruncatedSelectOne
from .._core import eoRandomSelect as RandomSelect
from .._core import eoNoSelect as NoSelect
from .._core import eoSequentialSelect as SequentialSelect
from .._core import eoEliteSequentialSelect as EliteSequentialSelect


class _DetTournamentSelect():
    """Deterministic Tournament Selection

    Parameters
    ----------
    _tSize tournament size
    """
    def __new__(cls,t_size=2,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoDetTournamentSelect"+config.TYPES[type])
        return class_(t_size)
