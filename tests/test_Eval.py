from pyparadiseo import Solution,Pop

from pyparadiseo import evaluator
from pyparadiseo import initializer

import time
import functools

import numpy as np
import numba as nb

ind = Solution()
init = initializer.BinaryInit(12)
init(ind)

print(ind)

solu = Solution([1,True,0.234,'abc'])
print(solu)





################
# HOW TO DEFINE FITNESS EVALUATOR ?
################
@nb.njit
def baz(sol,data):
    return 1.0
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

class myPyEval(evaluator.FitnessEval):
    def __init__(self,val,fun):
        evaluator.FitnessEval.__init__(self)
        self.foo = fun
        self.data = val
    def __call__(self,sol):
        sol.fitness = self.foo(sol.encoding,self.data)

def get_foo(val):
    return functools.partial(baz,data=val)

def time_eval(eval,init,N):
    ind = Solution()
    pop = Pop(N,init)

    t1 = time.time()
    for i in pop:
        eval(i)
    print(time.time()-t1)

off=42

D=100
N=10000

time_eval(myEval(off),initializer.BinaryInit(D),N)
time_eval(myEval(off),initializer.BinaryInit(D),N)

eval_count = evaluator.EvalFuncCounter(myEval(off))
time_eval(eval_count,initializer.BinaryInit(D),N)

f = get_foo(off)
time_eval(evaluator.FitnessEval(f),initializer.BinaryInit(D),N)

time_eval(myPyEval(off,baz),initializer.BinaryInit(D),N)



loopeval = evaluator.PopLoopEval(evaluator.FitnessEval(f))
pop = Pop(N,initializer.BinaryInit(D))

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
