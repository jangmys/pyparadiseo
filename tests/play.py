import pyparadiseo

from pyparadiseo import evaluator

from pyparadiseo.evaluator import *

from pyparadiseo._core import eoVector
from pyparadiseo._core import moeoVector
from pyparadiseo._core import realSol

import numpy as np


eoVec=eoVector(10,0)
print(eoVec)

print("#"*20)

moeoVec=moeoVector(10,0)
print(moeoVec)

for i in moeoVec:
    print(i)

print("size: ",len(moeoVec))

moeoVec[1]=1.0

print(np.sum(moeoVec))

print(moeoVec)


print("#"*20)

realVec = realSol(8)

print(dir(realSol))

realVec.append(4)

print(realVec)
