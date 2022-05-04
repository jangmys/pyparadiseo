from pyparadiseo import config,utils

from .._core import eoSGA as SGA
from .._core import eoEasyEA as EasyEA
from .._core import eoFastGA as FastGA



class _SGA():
    """Simple GA

    (selectOne,QuadOp,float,MonOp,float,EvalFunc,Continue)
    """
    def __new__(cls,selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSGA"+config.TYPES[type])

        #check arguments
        return class_(selector,crossover,p_cross,mutate,p_mutate,f_eval,continuator)



class _EasyEA():
    """Easy EA

    (continue,EvalFunc,Breed,Replace)  #order inverse of SGA? (mandatory left, optional right)
    (continue,Evalfunc,Breed,Replace,unsigned)
    (continue,PopEval,Breed,Replace)
    (continue,EvalFunc,Breed,Merge,Reduce)
    (continue,EvalFunc,Select,Transform,Replace)
    (continue,EvalFunc,Select,Transform,Merge,Reduce)
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoEasyEA"+config.TYPES[type])


class _FastGA():
    """Fast GA

        double,
        eoSelectOne<SolutionType>&,
        eoQuadOp<SolutionType>&,
        eoSelectOne<SolutionType>&,
        double,
        eoSelectOne<SolutionType>&,
        eoMonOp<SolutionType>&,
        eoPopEvalFunc<SolutionType>&,
        eoReplacement<SolutionType>&,
        eoContinue<SolutionType>&,
        double
    """
    def __new__(cls,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoFastGA"+config.TYPES[type])


__all__ = [SGA,EasyEA,FastGA]
