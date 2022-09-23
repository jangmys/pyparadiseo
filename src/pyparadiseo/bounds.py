"""
Domain bounds
-------------

bounds

eoBounds.h

domain bounds
"""
#ABC
from ._core import RealBounds as RealBoundsBase
# from ._core import RealVectorBoundsBase

#use? bound for one variable ...

from ._core import RealNoBounds # unbounded
from ._core import RealBelowBound # [a,inf]
from ._core import RealAboveBound # [inf,a]
from ._core import RealInterval #[a,b]

from ._core import RealVectorBounds #a vector of bounds



from ._core import IntBounds as IntBoundsBase
from ._core import IntBaseVectorBounds as IntVectorBoundsBase

from ._core import IntNoBounds # unbounded
from ._core import IntBelowBound # [a,inf]
from ._core import IntAboveBound # [inf,a]
from ._core import IntInterval #[a,b]

import numpy as np

#class_ BoundsReal():
#   pass

#Bounds (vector)

# () default
# (dim, bound) --> same for all (allows half-intevals)
# (dim, lb, ub) --> same for all
# (real_vec,real_vec) --> lb,ub as vectors


def real_interval(lb,ub):
    """
    1D Real Interval Bounds

    Parameters
    ----------
    lb : real
    ub : real

    np.inf is allowed
    """
    if not np.isinf(lb) and not np.isinf(ub):
        return RealInterval(lb,ub)
    if np.isinf(lb) and np.isinf(ub):
        return RealNoBounds()
    if np.isinf(lb):
        return RealAboveBound(ub)
    if np.isinf(ub):
        return RealBelowBound(lb)


def bound_box(*args):
    r"""Box-bounds for real-valued solutions

    Parameters
    ==========
    args : iterable
        - (dim : int, lb : float, ub : float)
            same bounds in each dimension
        - (lb : float[], ub : float[])
            different bounds for each variable
        - (dim : int, bound)
            duplicate bound (see bound_{below,above,interval} dim times
    """
    if len(args) == 2:
        ret=None
        try:
            if len(args[0]) != len(args[1]):
                print("lb and ub should be of same length")

            ret = RealVectorBounds()
            for idx in range(len(args[0])):
                a,b = args[0][idx],args[1][idx]
                ret.append(real_interval(a,b))

            return ret
        except ValueError:
            print("failed to construct bound_box")

    if len(args) == 3:
        #should be one int and two floats
        return RealVectorBounds(args[0],
            RealInterval(args[1],args[2])
        )
