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

def foo(sol):
    return 420


eval = get_evaluator("FitnessEval",foo,type="bin")
print(eval)

eval = get_evaluator("ObjectiveEval",foo,type="bin")
print(eval)

eval = get_evaluator("ObjectiveEval",foo)
print(eval)

from pyparadiseo import utils,config

class ProxyClass(eoEvalFunc):
    def __init__(self, *args, type='gen', **kwargs):
        eoEvalFunc.__init__(self)

        # condition = self.do_some_condition_checks(*args, **kwargs)
        if type=='bin':
            print("init bin")

        klass=utils.get_class("FitnessEval"+config.TYPES[type])
        print("CLASS",utils.get_class("FitnessEval"+config.TYPES[type]))

        # self._obj = klass(*args, **kwargs)
        object.__setattr__(
            self,
            "_obj",klass(*args, **kwargs)
            # FitnessEval(*args, **kwargs) if type=='gen' else FitnessEvalBin(*args, **kwargs)
        )

    # proxying (special cases)
    #
    def __setattr__(self, name, value):
        setattr(object.__getattribute__(self, "_obj"), name, value)

    # def __getattribute__(self, name):
    #     return getattr(object.__getattribute__(self, "_obj"), name)

    def __delattr__(self, name):
        delattr(object.__getattribute__(self, "_obj"), name)

    def __nonzero__(self):
        return bool(object.__getattribute__(self, "_obj"))

    def __str__(self):
        return str(object.__getattribute__(self, "_obj"))

    def __repr__(self):
        return repr(object.__getattribute__(self, "_obj"))

    def __hash__(self):
        return hash(object.__getattribute__(self, "_obj"))

    def __call__(self,sol):
        object.__getattribute__(self, "_obj").__call__(sol)



class ProxyEval(eoEvalFunc):
    def __init__(self, *args, type='gen', **kwargs):
        eoEvalFunc.__init__(self)

        klass=utils.get_class("FitnessEval"+config.TYPES[type])
        print("CLASS",utils.get_class("FitnessEval"+config.TYPES[type]))

        self._eval = klass(*args, **kwargs)


        # self._obj = klass(*args, **kwargs)
        # object.__setattr__(
        #     self,
        #     "_obj",klass(*args, **kwargs)
        #     # FitnessEval(*args, **kwargs) if type=='gen' else FitnessEvalBin(*args, **kwargs)
        # )

    def __call__(self,sol):
        self._eval.__call__(sol)
        # object.__getattribute__(self, "_obj").__call__(sol)

    def __getattr__(self, name):
        return getattr(self._eval, name)

    def __str__(self):
        return self._eval.__str__()
        # str(object.__getattribute__(self, "_eval"))
        # return str(object.__getattribute__(self, "_eval"))
        # return str(object.__getattribute__(self, "_obj"))

    def __repr__(self):
        return self._eval.__repr__()
        # return repr(object.__getattribute__(self, "_obj"))


    # proxying (special cases)
    #
    # def __setattr__(self, name, value):
    #     setattr(object.__getattribute__(self, "_obj"), name, value)
    #
    # # def __getattribute__(self, name):
    # #     return getattr(object.__getattribute__(self, "_obj"), name)
    #
    # def __delattr__(self, name):
    #     delattr(object.__getattribute__(self, "_obj"), name)
    #
    # def __nonzero__(self):
    #     return bool(object.__getattribute__(self, "_obj"))
    #
    # def __str__(self):
    #     return str(object.__getattribute__(self, "_obj"))
    #
    # def __repr__(self):
    #     return repr(object.__getattribute__(self, "_obj"))
    #
    # def __hash__(self):
    #     return hash(object.__getattribute__(self, "_obj"))



print("#"*20)

class evaluate(eoEvalFunc):
    def __init__(self):
        eoEvalFunc.__init__(self)
        # pass
    def __call__(self, sol):
        # take solution as argument, set fitness
        sol.fitness = 42

print(isinstance(evaluate(),eoEvalFunc))


ev = evaluate()
import inspect
print(inspect.getclasstree(inspect.getmro(evaluate)))

print("#"*20)
for t in "bin","gen":
    proxy1 = ProxyClass(foo,type=t)
    print("proxy ",proxy1)

    proxyeval1 = ProxyEval(foo,type=t)
    print("proxy2 ",proxyeval1)


print("#"*20)
import numpy as np

sol = pyparadiseo.Solution(np.arange(5))

print(sol)
proxy1(sol)
print("eval proxy ",sol)
#
# print(sol)
# proxyeval1(sol)
# print("eval proxy2 ",sol)
#
# peval=PopLoopEval(ev)
#
# peval=PopLoopEval(proxy1)
#
# peval=PopLoopEval(proxyeval1)
#
# proxy2 = ProxyClass(foo,type="bin")
# print(proxy2)
# print(dir(proxy1))
#
# sol = pyparadiseo.BinarySolution(5)
# print(sol)
# proxy2(sol)
# print(sol)
# #
# #
# # peval=PopLoopEval(ev)
# #
# peval=PopLoopEval(proxy2)
# #
