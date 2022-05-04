import pyparadiseo

from pyparadiseo import evaluator

from pyparadiseo.evaluator import *

EVALUATORS={
    'fitness' : 'FitnessEval',
    'objective' : 'ObjectiveEval',
    # 'pop' : 'PopLoopEval'
}

TYPES={
    'gen' : '',
    'bin' : 'Bin'
}

FITNESSEVALS={
    'gen' : FitnessEval,
    'bin' : FitnessEvalBin
}

class FitEval:
    def __new__(cls,type,f_eval=None):
        return FITNESSEVALS[type](f_eval)

def foo():
    return 42

f=FitEval('gen')
print(f)
print(type(f))

f=FitEval('gen',foo)
print(f)
print(type(f))

f=FitEval('bin')
print(f)
print(type(f))

f=FitEval('bin',foo)
print(f)
print(type(f))

import importlib

def get_class(modul,kls):
    mod = importlib.import_module(modul)
    class_=getattr(mod,kls)
    return class_

for e in EVALUATORS.values():
    for type in TYPES.values():
        print('-'*10)
        c=get_class('pyparadiseo.evaluator',e+type)
        print(e+type)
        print(c)
        print(c())
        print('-'*10)



def get_evaluator(klass,fun=None,type='gen'):
    clazz = get_class("pyparadiseo._core",klass+TYPES[type])

    if fun is not None:
        return clazz(fun)
    else:
        return clazz()

eval = get_evaluator("FitnessEval")
print(eval)

eval = get_evaluator("FitnessEval","bin")
print(eval)

def foo():
    return 420


eval = get_evaluator("FitnessEval",foo,type="bin")
print(eval)

eval = get_evaluator("ObjectiveEval",foo,type="bin")
print(eval)

eval = get_evaluator("ObjectiveEval",foo)
print(eval)

#
#
#
# def get_from_list(l, name, args, kwargs):
#     i = None
#
#     for k, e in enumerate(l):
#         if e[0] == name:
#             i = k
#             break
#
#     if i is None:
#         for k, e in enumerate(l):
#             if re.match(e[0], name):
#                 i = k
#                 break
#
#     if i is not None:
#
#         if len(l[i]) == 2:
#             name, clazz = l[i]
#
#         elif len(l[i]) == 3:
#             name, clazz, default_kwargs = l[i]
#
#             # overwrite the default if provided
#             for key, val in kwargs.items():
#                 default_kwargs[key] = val
#             kwargs = default_kwargs
#
#         return clazz(*args, **kwargs)
#     else:
#         raise Exception("Object '%s' for not found in %s" % (name, [e[0] for e in l]))
#
#
#
#
#
#
#
# def get_evaluator_options():
#     from pyparadiseo.evaluator import FitnessEval
#
#
#     EVALUATORS = [
#         ("fitness", FitnessEval),
#         ("objective", ObjectiveEval),
#         ("pop", ObjectiveEval)
#     ]
#
#     return SELECTION
#
#
# def get_selection(name, *args, d={}, **kwargs):
#     return get_from_list(get_selection_options(), name, args, {**d, **kwargs})
#



print("#"*20)
