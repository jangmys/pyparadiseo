from pyparadiseo import RealSolution,Solution,Pop

from pyparadiseo import solution
from pyparadiseo import evaluator
from pyparadiseo import initializer,bounds
from pyparadiseo import _core

import time
import functools

import numpy as np
import numba as nb






################
# HOW TO DEFINE FITNESS EVALUATOR ?
################
@nb.njit
def baz(sol,data):
    return np.sum(sol) + data
    # return 1.0
    # return np.count_nonzero(sol) + data

#### /1/ inherit from base class and specialize __call__
class myEval(evaluator.eoEvalFunc):
    def __init__(self,val):
        #not needed? super() doesn't work
        evaluator.eoEvalFunc.__init__(self)
        self.data = val
    def __call__(self, sol):
        # no other arguments allowed
        # set .fitness from .encoding ...
        sol.fitness = baz(sol.encoding,self.data)

class myPyEval(_core.FitnessEval):
    def __init__(self,val,fun):
        _core.FitnessEval.__init__(self)
        self.foo = fun
        self.data = val
    def __call__(self,sol):
        sol.fitness = self.foo(sol.encoding,self.data)

def get_foo(val):
    return functools.partial(baz,data=val)

def time_eval(eval,init,N):
    # ind = RealSolution()
    # ind = Solution()
    pop = Pop(N,init)

    t1 = time.time()
    for i in pop:
        eval(i)
    print(time.time()-t1)


off=42

D=100
N=10000

print("Evaluate np.sum(x) + a ",N," times for len(x)=",D)
print("Solution type 'gen' with encoding=np.random.random(",D,")")
print("="*20)
print("first call : Eval twice (numba.njit!)")
print("="*20)


ind = solution.empty()
init = initializer.initializer(lambda : 2*np.random.random(D)-1)

# ind = RealSolution(D)
# init(ind)

# print(ind)



print("="*20)
time_eval(myEval(off),init,N)
print("callable class inherited from eoEvalFunc")
time_eval(myEval(off),init,N)
print("="*20)

eval_count = evaluator.EvalFuncCounter(myEval(off))
print("counting")
time_eval(eval_count,init,N)
print("="*20)

f = get_foo(off)
print("functools partial")
time_eval(evaluator.fitness(f),init,N)
print("="*20)

print("myPyEval")
time_eval(myPyEval(off,baz),init,N)
print("="*20)

print("POP-LOOP","="*20)

loopeval = evaluator.pop_eval_from_fitness(evaluator.fitness(f))
pop = Pop(N,init)

t1 = time.time()
loopeval(pop,pop)
print(time.time()-t1)


p = [np.random.choice(2,D) for _ in range(N)]
v = np.zeros(N)

baz(p[0],off)

t1 = time.time()
for i,ind in enumerate(p):
    v[i]=baz(ind,off)
print(time.time()-t1)

print("#"*22)

eval = evaluator.fitness(None)

def f2(sol):
    sol.fitness = baz(sol.encoding,42)

eval.set_eval_func(f2)
time_eval(eval,init,N)
