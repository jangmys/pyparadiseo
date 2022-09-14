import pyparadiseo as pp


from pyparadiseo import config
from pyparadiseo import evaluator
from pyparadiseo import population
from pyparadiseo import operator

from pyparadiseo import eo,initializer,bounds
from pyparadiseo.eo import select_one,continuator,algo
from pyparadiseo.operator import MonOp,QuadOp

import time
import copy
import numpy as np

def norm2(ind):
    return np.linalg.norm(ind,2)


VEC_SIZE = 500 # Number of object variables in genotypes
POP_SIZE = 20 # Size of population
T_SIZE = 3 # size for tournament selection
MAX_GEN = 5000 # Maximum number of generation before STOP
CROSS_RATE = 0.8 # Crossover rate
EPSILON = 0.1 # range for real uniform mutation
MUT_RATE = 0.5 # mutation rate

DOMAIN_BOUND = 5

def realGA_generic_sol():
    config.set_solution_type('gen')

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)

    config.set_minimize_fitness()

    # myeval = pp.evaluator.EvalFuncProxy(lambda sol: norm2(sol))
    myeval = evaluator.fitness(lambda sol: norm2(sol))

    mybounds=bounds.RealVectorBounds(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)
    myinit = pp.initializer.RealBoundedInit(mybounds)

    # print(type(myinit))

    p = population.from_init(POP_SIZE,myinit)
    # pop = pp.Pop(POP_SIZE,myinit)

    for ind in p:
        myeval(ind)

    # print(p)

    select = select_one.det_tournament(3)

    # print(select(p))
    # print(select(p))
    # print(select(p))
    # print('-'*10)

    # select = selector.DetTournamentSelect(3)
    xover = pp.operator.SegmentCrossover(0.0)
    # mutat = pp.operator.UniformMutation(mybounds,EPSILON,1.0)
    mutat = pp.operator.UniformMutation(bounds=mybounds,epsilon=EPSILON,p_change=1.0)
    conti = pp.eo.continuator.max_generations(MAX_GEN)

    sga = pp.eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    t1=time.time()
    sga(p)
    t=time.time()-t1

    # print(p)
    print(p.best().fitness)
    print("Time: ",t)


def realGA_real_sol():
    config.set_solution_type('real')
    config.set_minimize_fitness()

    #only the internal paradiseo RNG
    pp.rng().reseed(42)
    #seed
    nprng = np.random.default_rng(42)

    # myeval = evaluator.fitness(lambda sol: norm2(sol),"real")
    from pyparadiseo import _mod
    myeval=_mod.Sphere()
    #
    # import ioh
    # f = ioh.get_problem("Sphere", dimension=VEC_SIZE, instance=1)
    #
    # myeval = evaluator.fitness(f)


    mybounds = bounds.bound_box(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)

    myinit = initializer.random(stype='real',bounds=mybounds)
    p = population.from_init(POP_SIZE,myinit,stype='real')

    for ind in p:
        myeval(ind)

    select = select_one.det_tournament(3)
    xover = operator.SegmentCrossover(0.0)
    mutat = operator.UniformMutation(bounds=mybounds,epsilon=EPSILON,p_change=1.0)

    conti = pp.eo.continuator.max_generations(MAX_GEN)
    #
    sga = pp.eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    t1=time.time()
    sga(p)
    t=time.time()-t1

    print(p.best().fitness)
    print("Time: ",t)



def realGA_pyops():
    config.set_solution_type('real')
    config.set_minimize_fitness()

    #only the internal paradiseo RNG
    pp.rng().reseed(42)

    # myeval = evaluator.fitness(lambda sol: norm2(sol),"real")
    from pyparadiseo import _mod
    myeval=_mod.Sphere()

    mybounds = bounds.bound_box(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)

    myinit = initializer.random(stype='real',bounds=mybounds)

    p = population.from_init(POP_SIZE,myinit,stype='real')

    for ind in p:
        myeval(ind)

    import copy

    # @select_one.select_one
    class tournament():
        def __init__(self,t_size,rng):
            # eoSelectOneReal.__init__(self)
            self.t_size = t_size
            self.rng = rng
        def __call__(self,pop):
            r1 = self.rng.random(len(pop))
            for i in range(self.t_size-1):
                r2 = self.rng.random(len(pop))
                if pop[r2].fitness < pop[r1].fitness:
                    # print(pop[r2].fitness,pop[r1].fitness)
                    r1 = r2

            return pop[r1]

        def setup(self,pop):
            pass


    from pyparadiseo._core import pySelectOneReal
    from pyparadiseo._core import eoSelectOneReal

    tourn = tournament(3,pp.rng())

    def setup(pop):
        pass

    select = pySelectOneReal(tourn,setup)

    xover = operator.SegmentCrossover(0.0)
    mutat = operator.UniformMutation(bounds=mybounds,epsilon=EPSILON,p_change=1.0)

    conti = pp.eo.continuator.max_generations(MAX_GEN)
    #
    sga = pp.eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    t1=time.time()
    sga(p)
    t=time.time()-t1

    print(p.best().fitness)
    print("Time: ",t)


# mimic eo/tutorial/Lesson1/FirstRealGA.cpp
if __name__ == "__main__":
    print("Simple GA\n","="*20)
    print("Minimizing Sphere function with DIM: ",VEC_SIZE)
    print("Domain: [-",DOMAIN_BOUND,',',DOMAIN_BOUND,"]")
    print("#Generations: ", MAX_GEN)

    print("With 'real' solution...")
    realGA_real_sol()
    print("#"*20)

    print("With python operators...")
    realGA_pyops()
    print("#"*20)
    # realGA_generic_sol()
