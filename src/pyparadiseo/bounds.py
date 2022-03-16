#ABC
from ._core import RealBounds as RealBoundsBase

from ._core import RealNoBounds
from ._core import RealBelowBound
from ._core import RealAboveBound
from ._core import RealInterval

from ._core import RealVectorBounds

globals()["RealInterval"] = RealInterval
