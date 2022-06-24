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

# init = initializer.make_initializer(lambda:np.arange(10))

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
