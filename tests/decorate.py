import numpy as np

from pyparadiseo import initializer
from pyparadiseo import solution

from pyparadiseo import decorators

# @decorators.fitness
@decorators.myDecorator
def aFunction():
    print("inside aFunction()")

@decorators.fitness()
def compute_sum(ind):
    return np.sum(ind)



aFunction()


@decorators.decoratorFunctionWithArguments("a","arg2","arg3")
def bFunction():
    print("inside aFunction()")

bFunction()







s = solution.empty()

@decorators.initializer
def init():
    return np.arange(10)

@decorators.initializer()
def init2():
    return np.arange(11)

init3 = decorators.initializer(lambda : np.arange(12))

@decorators.initializer(stype='gen')
def init4():
    return np.arange(13)

@decorators.initializer('gen')
def init5():
    return np.arange(14)

init(s)
compute_sum(s)
print(s)

init2(s)
compute_sum(s)
print(s)

init3(s)
compute_sum(s)
print(s)

init4(s)
compute_sum(s)
print(s)

init5(s)
compute_sum(s)
print(s)

solution.from_init(init)


####################################################
### FITNESS
from pyparadiseo import initializer
from pyparadiseo import bounds


def return_fitness(arr):
    return np.sum(arr)

def apply_to_sol(sol):
    sol.fitness = np.sum(sol.array)

sol = solution.random(10,stype='real',bounds=bounds.box(10,-1,1))

print(sol)

fit1 = decorators.fitness(return_fitness,stype='real')

fit1(sol)
print(sol)



print("x"*20)

###################################################
@decorators.MyDecoratorClass("un arg")
def foo():
    print("gello")

@decorators.MyDecoratorClass
def bar():
    print("hello")

foo()
bar()


# class bar():
#     def __init__(self,value):
#         self.value = value
#
#     @decorators.MyDecorator("another arg")
#     def foo(self,sol):
#         return np.sum(sol)
#
# b=bar(42)
# print(b.foo(sol.array))
