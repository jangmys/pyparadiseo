#abstract base classes
from .core import eoEvalFunc
from .core import eoPopEvalFunc

from .core import FitnessEval
from .core import ObjectiveEval
from .core import eoPopLoopEval as PopLoopEval

### counting eval function (for eoContinuator)
from .core import eoEvalFuncCounter as EvalFuncCounter
