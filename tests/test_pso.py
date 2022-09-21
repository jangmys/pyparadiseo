from pyparadiseo import _core

# from pyparadiseo import Solution
from pyparadiseo import population
from pyparadiseo import evaluator
from pyparadiseo import bounds

from pyparadiseo import swarm
from pyparadiseo import config

from pyparadiseo import operator,initializer,bounds
from pyparadiseo.eo import continuator


import numpy as np

import unittest
import copy


class TestPSO(unittest.TestCase):
    def setUp(self):
        self.particle = _core.RealParticle(10)
        # print("this")
    def tearDown(self):
        config.set_solution_type('gen')

    #create particle
    def test_particle(self):
        particle = _core.RealParticle()
        self.assertEqual(len(particle.array),0)
        self.assertEqual(len(particle),0)

        particle = _core.RealParticle(10)
        self.assertEqual(len(particle.array),10)
        self.assertEqual(len(particle),10)

    def test_stdflight(self):
        flight = _core.eoStandardFlight(10,-2,2)
        self.assertTrue(callable(flight))

        particle = _core.RealParticle(10,1.0,1.0,1.0)

        self.assertEqual(particle.array[0],0.0)
        flight(particle)
        self.assertEqual(particle.array[0],1.0)
        flight(particle)
        self.assertEqual(particle.array[0],2.0)
        flight(particle)
        self.assertEqual(particle.array[0],2.0)

    def test_lineartopo(self):
        topo = _core.eoLinearTopology(3)
    #
    #     pop = swarm.empty(stype='real')
    #     # pop = population.empty(stype='real')
    #     # pop = _core.RealParticlePop()
    #     for i in range(7):
    #         pop.push_back(_core.RealParticle(10,1.0,1.0,1.0))
    #
    #     init = initializer.random(10,stype='real',bounds=bounds.bound_box(10,-1,1))
    #     eval = evaluator.fitness(lambda x : sum(x),stype='real')
    #     for ind in pop:
    #         init(ind)
    #         eval(ind)
    #         # print(ind)
    #
    #         # ind.fitness = 4.2
    #     topo.setup(pop)
    #
    #
    #     print(topo.best(0))
    #     print(topo.globalBest())
    #
    def test_veloinit(self):
        rndgen = _core.RealUniformGenerator(-2,2)

        print(self.particle)

        veloinit=_core.eoVelocityInitReal(10,rndgen)

        veloinit(self.particle)

        print(self.particle)


    def test_stdvelo(self):
        VEC_SIZE = 2
        POP_SIZE = 20
        NEIGHBORHOOD_SIZE = 5

        topo = _core.eoLinearTopology(NEIGHBORHOOD_SIZE)
    #
    #     # print(_core.eoLinearTopology)
    #
    #     import inspect
    #     print(inspect.getmro(_core.eoLinearTopology))
    #     print(inspect.getmro(_core.eoTopology))
    #
    #     print("is: ",isinstance(topo,_core.eoTopology))
    #
    #     bnds = bounds.bound_box(VEC_SIZE,-1.5,1.5)
    #
    #     velo = _core.eoStandardVelocity(topo,1.0,1.0,1.0,bnds)
    #
    #     config.set_solution_type('real-pso')
    #     init = initializer.random(VEC_SIZE,bounds=bounds.bound_box(VEC_SIZE,-3,3))
    #     pop = swarm.from_init(POP_SIZE,init)
    #
    #     # print(pop[0])
    #     #
    #     # velo(pop[0],0)


    def test_bestinit(self):
        config.set_solution_type('real-pso')

        VEC_SIZE = 2
        # particle = _core.RealParticle(VEC_SIZE)

        feval = evaluator.fitness(lambda x : np.linalg.norm(x,2))

        init = initializer.random(bounds=([-3]*VEC_SIZE,[3]*VEC_SIZE))
        bestinit=_core.eoFirstIsBestInit()

        pop = swarm.from_init(4,init)

        for particle in pop:
            init(particle)
            feval(particle)
            bestinit(particle)


    def test_initializer(self):
        config.set_solution_type('real-pso')
        config.set_minimize_fitness()

        VEC_SIZE = 20
        NEIGHBORHOOD_SIZE = 5
        POP_SIZE = 20

        cont = continuator.max_generations(10000)

        feval = evaluator.fitness(lambda x : np.linalg.norm(x,2))
        veloinit=_core.eoVelocityInitReal(VEC_SIZE,_core.RealUniformGenerator(-2,2))
        bestinit=_core.eoFirstIsBestInit()
        topo = _core.eoLinearTopology(NEIGHBORHOOD_SIZE)

        flight = _core.eoStandardFlight()

        bnds = bounds.bound_box(VEC_SIZE,-0.5,0.5)
        velo = _core.eoStandardVelocity(topo,1,1.6,2,bnds)

        init = initializer.random(bounds=([-3]*VEC_SIZE,[3]*VEC_SIZE))
        pop = swarm.from_init(POP_SIZE,init)

        psoinit=_core.PSOInitializer(
            feval,
            veloinit,
            bestinit,
            topo,
            pop
        )

        # print(pop)
        psoinit()

        alg = _core.eoEasyPSO(cont,feval,velo)
        alg(pop)
        print(pop.best())
        print("#"*20)

        alg = _core.eoEasyPSO(cont,feval,velo,flight)
        alg(pop)
        print(pop.best())
        print("#"*20)

        alg = _core.eoEasyPSO(psoinit,cont,feval,velo)
        alg(pop)
        print(pop.best())
        print("#"*20)

        alg = _core.eoEasyPSO(psoinit,cont,feval,velo,flight)
        alg(pop)
        print(pop.best())
        print("#"*20)


    def test_algo(self):
        config.set_solution_type('real-pso')

        VEC_SIZE = 2
        POP_SIZE = 20
        NEIGHBORHOOD_SIZE = 5

    #     eval = evaluator.fitness(lambda x : np.linalg.norm(x,2))
    #     bnds = bounds.bound_box(VEC_SIZE,-1.5,1.5)
    #
    #     init = initializer.random(VEC_SIZE,bounds=bounds.bound_box(VEC_SIZE,-3,3))
    #     pop = swarm.from_init(POP_SIZE,init)
    #
    #     # print("POP\t\t",pop)
    #     # pop = swarm.empty()
    #
    #     topo = _core.eoLinearTopology(NEIGHBORHOOD_SIZE)
    #     velo = _core.eoStandardVelocity(topo,1,1.6,2,bnds)
    #
    #     # # flight = _core.eoStandardFlight()
    #     # #
    #     cont = continuator.max_generations(50)
    #     #
    #     alg = _core.eoEasyPSO(cont,eval,velo)
    #     #
    #     # alg(pop)
    #     #
    #     # print(pop)
    #     #
        config.set_solution_type('gen')


if __name__ == '__main__':
    unittest.main()
