import pyparadiseo as pp

from pyparadiseo import config
from pyparadiseo import evaluator
from pyparadiseo import population
from pyparadiseo import operator

from pyparadiseo import eo

import time
import copy
import numpy as np

def norm2(ind):
    return np.linalg.norm(ind,2)


VEC_SIZE = 500 # Number of object variables in genotypes
POP_SIZE = 20 # Size of population
T_SIZE = 3 # size for tournament selection
MAX_GEN = 500 # Maximum number of generation before STOP
CROSS_RATE = 0.8 # Crossover rate
EPSILON = 0.1 # range for real uniform mutation
MUT_RATE = 0.5 # mutation rate

DOMAIN_BOUND = 5

def realGA_generic_sol():
    config.set_solution_type('gen')
    config.set_minimize_fitness()

    #only the internal paradiseo RNG
    pp.rng().reseed(42)


    myeval = evaluator.fitness(lambda sol: norm2(sol))

    mybounds=pp.bounds.bound_box(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)

    rand_arr = lambda : DOMAIN_BOUND*(2*np.random.random(VEC_SIZE)-1)
    myinit = pp.initializer.initializer(rand_arr)

    p = population.from_init(POP_SIZE,myinit)

    for ind in p:
        myeval(ind)

    select = eo.select_one.det_tournament(3)

    # both work on 'real' solutions only...
    _xover = pp.operator.segment_cx(mybounds,0.0)
    _mutat = pp.operator.uniform_real_mutation(EPSILON,mybounds,1.0)

    # wrapping to apply to generic sol type
    def gen_xvoer(ind1,ind2):
        sol1 = pp.solution.solution(ind1,'real')
        sol2 = pp.solution.solution(ind2,'real')

        _xover(sol1,sol2)

        ind1 = sol1.encoding
        ind2 = sol2.encoding

        return True


    # wrapping to apply to generic sol type
    def gen_mut(ind1):
        sol1 = pp.solution.solution(ind1,'real')

        _mutat(sol1)

        ind1 = sol1.encoding
        return True

    xover = pp.operator.make_crossover(gen_xvoer)
    mutat = pp.operator.make_mutation(gen_mut)
    conti = eo.continuator.max_generations(MAX_GEN)

    sga = eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

    t1=time.time()
    sga(p)
    t=time.time()-t1

    # print(p)
    print(p.best().fitness)
    print("Time: ",t)


def realGA_real_sol():
    pp.set_solution_type('real')
    pp.set_minimize_fitness()

    #only the internal paradiseo RNG
    pp.rng().reseed(42)

    from pyparadiseo import _mod
    myeval=_mod.Sphere()

    mybounds = pp.bounds.bound_box(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)

    myinit = pp.initializer.random(bounds=mybounds)
    p = population.from_init(POP_SIZE,myinit,stype='real')

    for ind in p:
        myeval(ind)

    select = eo.select_one.det_tournament(3)
    xover = pp.operator.segment_cx(mybounds,0.0)
    mutat = pp.operator.uniform_real_mutation(EPSILON,mybounds,1.0)
    conti = eo.continuator.max_generations(MAX_GEN)

    sga = eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

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

    mybounds = pp.bounds.bound_box(VEC_SIZE,(-1.0)*DOMAIN_BOUND,DOMAIN_BOUND)
    myinit = pp.initializer.random(stype='real',bounds=mybounds)

    p = population.from_init(POP_SIZE,myinit,stype='real')

    for ind in p:
        myeval(ind)

    import copy

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
    conti = eo.continuator.max_generations(MAX_GEN)

    sga = eo.algo.simpleGA(select,xover,CROSS_RATE,mutat,MUT_RATE,myeval,conti)

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

    print("With 'generic' python solution...")
    realGA_generic_sol()
    print("#"*20)
