import importlib

# =====================================
# GENERIC
# =====================================
def get_class(modul,kls):
    mod = importlib.import_module(modul)
    class_=getattr(mod,kls)
    return class_


TYPES={
    'gen' : '',
    'bin' : 'Bin',
    'real' : 'Real'
}

# =====================================
# SOLUTION
def get_solution(name,encoding=None,type='gen'):
    clazz = get_class("pyparadiseo._core",name+TYPES[type])

    if encoding is not None:
        return clazz(encoding)
    else:
        return clazz()


# =====================================
# POPULATION
def get_population(pop_size=None,f_init=None,type="gen"):
    """
    create and initialize a Population class object

    Parameters
    ----------
    stype : Solution type
        "generic" (default)
        "real"
        "perm"
        "particle"

    pop_size, optional : int
    f_init : initialization function

    Returns
    -------
    A Population
    """
    clazz = get_class("pyparadiseo._core","Pop"+TYPES[type])

    if pop_size is None:
        return clazz()
    else:
        if f_init is None:
            print("need init function")
        else:
            return clazz(pop_size,f_init)



# =====================================
# EVAL (directly from ._core) ... only factory?
# =====================================
def get_evaluator(name,fun=None,type='gen'):
    clazz = get_class("pyparadiseo._core",name+TYPES[type])

    if fun is not None:
        return clazz(fun)
    else:
        return clazz()









# =====================================
# EVAL (through evaluator.py) .. additional python wrapper (doc:!)
# =====================================

# =======================================================================
# Generic (from pymoo)
# =========

def get_from_list(l, name, args, kwargs):
    i = None

    for k, e in enumerate(l):
        if e[0] == name:
            i = k
            break

    if i is None:
        for k, e in enumerate(l):
            if re.match(e[0], name):
                i = k
                break

    if i is not None:

        if len(l[i]) == 2:
            name, clazz = l[i]

        elif len(l[i]) == 3:
            name, clazz, default_kwargs = l[i]

            # overwrite the default if provided
            for key, val in kwargs.items():
                default_kwargs[key] = val
            kwargs = default_kwargs

        return clazz(*args, **kwargs)
    else:
        raise Exception("Object '%s' for not found in %s" % (name, [e[0] for e in l]))





def get_eval_options():
    from pyparadiseo.evaluator import _FitnessEval
    from pyparadiseo.evaluator import _ObjectiveEval

    EVAL = [
        ("fitness",_FitnessEval),
        ("objective",_ObjectiveEval)
    ]

    return EVAL


def get_eval(name,*args,d={},**kwargs):
    return get_from_list(get_eval_options(),name,args,{**d, **kwargs})



# =====================================
# CONTINUATORS
# =====================================
def get_continue_options():
    from pyparadiseo.eo.continuator import _GenContinue
    from pyparadiseo.eo.continuator import _CombinedContinue
    from pyparadiseo.eo.continuator import _EvalContinue
    from pyparadiseo.eo.continuator import _SteadyFitContinue
    from pyparadiseo.eo.continuator import _SecondsElapsedContinue

    CONTINUATORS=[
        ("gen_continue",_GenContinue),
        ("eval_continue",_EvalContinue),
        ("combined_continue",_CombinedContinue)
        ("steady_fit_continue",_SteadyFitContinue)
        ("seconds_elapsed_continue",_SecondsElapsedContinue)

    ]

    return CONTINUATORS


def get_continue(name,*args,d={},**kwargs):
    return get_from_list(get_continue_options(),name,args,{**d, **kwargs})


# =====================================
# MUATATION
# =====================================




REAL_MUTATION = {
    'uniform' : 'UniformMutation',
    'det_uniform' : 'DetUniformMutation'
}


BIN_MUTATION = {
    'flip_one' : 'OneBitFlip',
    'flip_det' : 'DetBitFlip',
    'flip_det_single' : 'DetSingleBitFlip'
}


def get_mutation(name,type='gen'):
    if type is 'real':
        clazz = get_class("pyparadiseo._core",REAL_MUTATION[name])
    if type is 'bin':
        clazz = get_class("pyparadiseo._core",BIN_MUTATION[name])

    return clazz


# =====================================
# CROSSOVER
# =====================================


REAL_CROSSOVER = {
    'segment' : 'SegmentCrossover',
    'hypercube :' : 'HypercubeCrossover',
    'sbx' : 'SBXCrossover'
}
