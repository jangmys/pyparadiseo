#abstract base classes
from ._core import eoEvalFunc
from ._core import eoPopEvalFunc

from ._core import FitnessEval
from ._core import ObjectiveEval
from ._core import eoPopLoopEval as PopLoopEval

### counting eval function (for eoContinuator)
from ._core import eoEvalFuncCounter as EvalFuncCounter



def pop_eval(pop,eval):
    for ind in pop:
        eval(ind)
