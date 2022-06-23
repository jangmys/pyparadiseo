from pyparadiseo import solution
from pyparadiseo import operator
from pyparadiseo import initializer

import numpy as np
import time


N = 1000000
D = 5

py_sol = solution.from_object(np.random.choice([True,False],D))

bin_sol = solution.empty(stype='bin')
bin_init = initializer.random(D,stype='bin')
bin_init(bin_sol)

# print(py_sol)
# print(bin_sol)

flip = operator._OneBitFlip()
bin_flip = operator.OneBitFlip()


flip(py_sol)
bin_flip(bin_sol)

# print(py_sol)
# print(bin_sol)


t1 = time.time()
for i in range(N):
    flip(py_sol)
print(time.time()-t1)

print(bin_sol)
print(bin_sol.array)


t1 = time.time()
for i in range(N):
    bin_flip(bin_sol)
print(time.time()-t1)

print(bin_sol)
print(bin_sol.array)



bin_sol2 = solution.empty(stype='bin')
bin_init(bin_sol2)

xover = operator.OnePtBitCrossoverBin()

print(bin_sol)
print(bin_sol2)


xover(bin_sol,bin_sol2)

print(bin_sol)
print(bin_sol2)

from pyparadiseo import _core

solu = _core.DoubleSolution(10)

print(solu)
