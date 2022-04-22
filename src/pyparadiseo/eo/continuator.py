from .._core import eoContinue as Continue

# move to eval?
# from ..core import eoEvalFuncCounter as EvalFuncCounter

from .._core import eoGenContinue as GenContinue
from .._core import eoCombinedContinue as CombinedContinue
from .._core import eoEvalContinue as EvalContinue
from .._core import eoSteadyFitContinue as SteadyFitContinue
from .._core import eoSecondsElapsedContinue as SecondsElapsedContinue

from .._core import eoFitContinue as FitContinue


def get_continue(name,*args,**kwargs):
    if name == "GenContinue":
        return GenContinue(kwargs["nb_gens"])
    if name == "EvalContinue":
        return EvalContinue(kwargs["count_eval_f"],kwargs["nb_evals"])
