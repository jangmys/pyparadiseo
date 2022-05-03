#abstract base classes
from ._core import eoEvalFunc
from ._core import eoEvalFuncBin

# __import__("._core.eoEvalFuncBin")

from ._core import eoPopEvalFunc

from ._core import FitnessEval
from ._core import ObjectiveEval
from ._core import eoPopLoopEval as PopLoopEval

from ._core import FitnessEvalBin
from ._core import ObjectiveEvalBin
from ._core import eoPopLoopEvalBin as PopLoopEvalBin



### counting eval function (for eoContinuator)
from ._core import eoEvalFuncCounter as EvalFuncCounter

import importlib

from pyparadiseo import _core


## put this to "utils" (or similar)...
def get_class(modul,kls):
    mod = importlib.import_module(modul)
    class_=getattr(mod,kls)
    return class_

TYPES={
    'gen' : '',
    'bin' : 'Bin',
    'real' : 'Real'
}
# ===================================
# ===================================
# ===================================

class _FitnessEval():
    def __new__(cls,fun=None,type='gen'):
        clazz = get_class("pyparadiseo._core","FitnessEval"+TYPES[type])
        if fun is not None:
            return clazz(fun)
        else:
            return clazz()

class _ObjectiveEval():
    def __new__(cls,fun=None,type='gen'):
        clazz = get_class("pyparadiseo._core","ObjectiveEval"+TYPES[type])
        if fun is not None:
            return clazz(fun)
        else:
            return clazz()
