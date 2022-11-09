"""
Evaluators
----------

evaluate solutions

evaluators assign fitness or objective values to solutions
"""
from pyparadiseo import config,utils
from mpi4py.futures import MPIPoolExecutor


def fitness(fun=None,counting=False,stype=None):
    """
    make fitness evaluation

    Parameters
    ----------
    fun : callable
        function must take a solution encoding as input and return a scalar fitness value
    counting : bool
        if True evaluator object counts fitness evaluations

    Example
    -------

    Notes
    -----
    see :func:`pyparadiseo.evaluator.counting_fitness`
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("FitnessEval"+config.TYPES[stype])

    ret = None

    if fun is not None:
        ret=class_(fun)
    else: # to use as decorator
        ret=class_()

    if counting:
        print("counting")
        return  utils.get_class("eoEvalFuncCounter"+config.TYPES[stype])(ret)
    else:
        return ret


def pop_eval(pop_eval,fit_obj='fitness',stype=None):
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("pyPopEval"+config.TYPES[stype])

    return class_(pop_eval)


def counting_fitness(fun=None,stype=None):
    """
    make counting fitness function
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoEvalFuncCounter"+config.TYPES[stype])

    ### TODO : check if we get python function or eoEvalFunc ...

    if fun is not None:
        return class_(fun)
    else: # to use as decorator
        return class_()


def objectives(fun=None,stype=None):
    """
    make fitness evaluation

    Parameters
    ----------
    fun : callable
        function must take a solution encoding as input and return a scalar fitness value

    Example
    -------

    Notes
    -----
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("ObjectiveEval"+config.TYPES[stype])

    if fun is not None:
        return class_(fun)
    else:
        return class_()


def pop_eval_from_fitness(f_eval,stype=None):
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoPopLoopEval"+config.TYPES[stype])

    if isinstance(f_eval,utils.get_class("eoEvalFunc"+config.TYPES[stype])):
        return class_(f_eval)
    else:
        return class_(fitness(fun=f_eval,stype=stype))

# ===================================
# ===================================
# ===================================

class __popeval():
    def __init__(self,fitness_eval,max_workers):
        self._fitness_eval = fitness_eval
        self._max_workers=max_workers

    def __call__(self,pop,pop2):
        with MPIPoolExecutor(max_workers=self._max_workers) as executor:
            future = executor.map(self._fitness_eval, pop2)
            for i,f in enumerate(future):
                pop2[i].fitness = f

        # for ind in pop2:
        #     print(ind.fitness)

def pool_exec_pop_eval(f_eval,nb_workers,stype=None):
    if stype is None:
        stype = config._SOLUTION_TYPE

    return pop_eval(__popeval(f_eval,nb_workers),stype=stype)

#
# class _FitnessEval():
#     def __new__(cls,fun=None,type=None):
#         if type is None:
#             type = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("FitnessEval"+config.TYPES[type])
#
#         if fun is not None:
#             return class_(fun)
#         else:
#             return class_()
#
#
# class _ObjectiveEval():
#     def __new__(cls,fun=None,type=None):
#         if type is None:
#             type = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("ObjectiveEval"+config.TYPES[type])
#
#         if fun is not None:
#             return class_(fun)
#         else:
#             return class_()
#
#
# class _PopEval():
#     def __new__(cls,f_eval,stype=None):
#         if stype is None:
#             stype = config._SOLUTION_TYPE
#
#         class_ = utils.get_class("eoPopLoopEval"+config.TYPES[stype])
#
#         if isinstance(f_eval,utils.get_class("eoEvalFunc"+config.TYPES[stype])):
#             return class_(f_eval)
#         else:
#             return class_(_FitnessEval(fun=f_eval,type=stype))
