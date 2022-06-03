#ABC
from ._core import RealBounds as RealBoundsBase

#use? bound for one variable ...

from ._core import RealNoBounds # unbounded
from ._core import RealBelowBound # [a,inf]
from ._core import RealAboveBound # [inf,a]
from ._core import RealInterval #[a,b]

from ._core import RealVectorBounds #a vector of bounds


#class_ BoundsReal():
#   pass

#Bounds (vector)

# () default
# (dim, bound) --> same for all (allows half-intevals)
# (dim, lb, ub) --> same for all
# (real_vec,real_vec) --> lb,ub as vectors

# (bound,bound) --> only 2D (why the particular case?)

def bound_below():
    """
    half-open bounds [a,inf[ (1D)
    """
    pass


def bound_above():
    """
    half-open bounds ]inf,a] (1D)
    """
    pass

def bound_interval():
    """
    1D
    """

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
            #either we got two vectors or (int,bounds)
            #trust python to figure it out
            ret = eoRealVectorBounds(args[0],args[1])
            return ret
        except ValueError:
            print("failed to construct bound_box")

    if len(args) == 3:
        #should be one int and two floats
        return eoRealVectorBounds(args[0],args[1],args[2])
