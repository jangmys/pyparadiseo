from pyparadiseo import RealSolution,Solution,Pop

from pyparadiseo import solution
from pyparadiseo import population
from pyparadiseo import evaluator
from pyparadiseo import initializer,bounds
from pyparadiseo import _core

import time
import functools

import numpy as np
import numba as nb

################
# a fitness function with solution and data input
################
# @nb.njit
def baz(sol,data):
    return np.sum(sol) + data

# @nb.njit
def sum(x,a):
    return np.sum(x) + a


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

def time_eval(eval,init,N,stype=None):
    # ind = RealSolution()
    # ind = Solution()
    pop = population.from_init(N,init,stype)

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
init = initializer.make_initializer(lambda : 2*np.random.random(D)-1)

print("="*20)
f=evaluator.fitness(functools.partial(baz,data=off))
time_eval(f,init,N)
time_eval(f,init,N)
print("="*20)



ind = solution.empty(stype='real')
init = initializer.random(D,stype='real',bounds=bounds.bound_box(D,-1,1))
# # init = initializer.make_initializer(lambda : 2*np.random.random(D)-1,stype='real')
#
# print("="*20)
# f=evaluator.fitness(functools.partial(baz,data=off),stype='real')
# pop = population.from_init(N,init,stype='real')
#
# t1 = time.time()
# for i in pop:
#     f(i)
# print(time.time()-t1)
# print("="*20)
#
#
#
#
#
# lpop=[solution.empty() for _ in range(N)]
# # lpop=[np.random.random(D) for _ in range(N)]
#
#
# for ind in lpop:
#     ind.encoding = np.random.random(D)
#     # print(ind)
#     # for i in range(len(ind)):
#     #     ind[i]=np.random.random()
#
#
# t1 = time.time()
# for ind in lpop:
#     ind.fitness = sum(ind.encoding,off)
# print(time.time()-t1)





sol = _core.RealSolution(D)

print("#"*30)


# print(sol)
# print(sol.array)

print(type(sol))
print(type(sol.array))
print("#"*10)

sol.array[2] = 1.1111

print(sol)
print(sol.array)
print("#"*10)
# ===================================================

sol.carray[4] = 1.1111

print(sol.array)
print(sol)
print("#"*10)

sol2 = _core.RealSolution(sol)

print(sol2)
print(sol2.array)


# print(sum(sol.array,off))
#
# lpop=[_core.RealSolution(D) for _ in range(N)]
#
# for ind in lpop:
#     for i in range(len(ind)):
#         ind[i]=np.random.random()
#
# t1 = time.time()
# for ind in lpop:
#     ind.fitness=np.sum(ind)
# print(time.time()-t1)
#
# t1 = time.time()
# for ind in lpop:
#     ind.fitness=np.sum(ind.array)
# print(time.time()-t1)
#
#
#
# b= bounds.bound_box(D,-1.0,1.0)
# init=initializer.random(stype='real',bounds=b)
#
# s = solution.empty(stype='real')
#
# init(s)
#
# s.fitness = np.sum(s.array)
#
# print(s)
# print(s.array)
#
# s2=solution.from_object(s)
# print(s2)
