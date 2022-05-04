#ABC
from ._core import RealBounds as RealBoundsBase

#use? bound for one variable ...

from ._core import RealNoBounds # unbounded
from ._core import RealBelowBound # [a,inf]
from ._core import RealAboveBound # [inf,a]
from ._core import RealInterval #[a,b]

from ._core import RealVectorBounds #a vector of bounds

globals()["RealInterval"] = RealInterval

#class_ BoundsReal():
#   pass

#Bounds (vector)

# () default
# (dim, bound) --> same for all (allows half-intevals)
# (dim, lb, ub) --> same for all
# (real_vec,real_vec) --> lb,ub as vectors

# (bound,bound) --> only 2D (why the particular case?)
