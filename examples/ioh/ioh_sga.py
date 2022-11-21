# https://iohprofiler.github.io/IOHexp/python/
import ioh

from ioh import get_problem

import pyparadiseo
from pyparadiseo import config
from pyparadiseo import solution
from pyparadiseo import population
from pyparadiseo import bounds

from pyparadiseo import evaluator
from pyparadiseo import initializer
from pyparadiseo import operator

from pyparadiseo.eo import select_one
from pyparadiseo.eo import algo
from pyparadiseo.eo import continuator

import numpy as np


VEC_SIZE = 50 # Number of object variables in genotypes
POP_SIZE = 20 # Size of population
T_SIZE = 3 # size for tournament selection
MAX_GEN = 5000 # Maximum number of generation before STOP
CROSS_RATE = 0.8 # Crossover rate
EPSILON = 0.1 # range for real uniform mutation
MUT_RATE = 0.5 # mutation rate

DOMAIN_BOUND = 10

### IOH
f = get_problem("Sphere", dimension=VEC_SIZE, instance=1)

#Print some properties of the problem
print("IOH Problem : ", f.meta_data)

### PYPARADISEO
config.set_solution_type('real')
config.set_minimize_fitness()

#fix the internal paradiseo RNG
pyparadiseo.rng().reseed(42)

f_eval = evaluator.fitness(f)

mybounds = bounds.box(f.constraint.lb,f.constraint.ub)

# POP
myinit = initializer.random(stype='real',bounds=mybounds)
pop = population.from_init(POP_SIZE,myinit,stype='real')

for ind in pop:
    f_eval(ind)

select = select_one.det_tournament(3)
xover = operator.SegmentCrossover(0.0)
mutat = operator.UniformMutation(bounds=mybounds,epsilon=EPSILON,p_change=1.0)

conti = continuator.max_generations(MAX_GEN)

sga = algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,f_eval,conti)

import time

from ioh import logger

l = logger.Analyzer(folder_name="temp")

f.attach_logger(l)


t1=time.time()
sga(pop)
t=time.time()-t1

print(pop.best().fitness)
print("Time: ",t)

print("#"*10+"\n",f.state)
