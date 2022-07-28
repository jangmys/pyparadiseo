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
    #not really testing anything...just that the calls don't fail
    def test_particle(self):
        particle = _core.RealParticle()
        self.assertEqual(len(particle.array),0)
        self.assertEqual(len(particle),0)

        particle = _core.RealParticle(10)
        self.assertEqual(len(particle.array),10)
        self.assertEqual(len(particle),10)

        print(particle)

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

        pop = swarm.empty(stype='real')
        # pop = population.empty(stype='real')
        # pop = _core.RealParticlePop()
        for i in range(7):
            pop.push_back(_core.RealParticle(10,1.0,1.0,1.0))

        init = initializer.random(10,stype='real',bounds=bounds.bound_box(10,-1,1))
        eval = evaluator.fitness(lambda x : sum(x),stype='real')
        for ind in pop:
            init(ind)
            eval(ind)
            # print(ind)

            # ind.fitness = 4.2
        topo.setup(pop)


        print(topo.best(0))
        print(topo.globalBest())

    def test_stdvelo(self):
        VEC_SIZE = 2
        POP_SIZE = 20
        NEIGHBORHOOD_SIZE = 5

        topo = _core.eoLinearTopology(NEIGHBORHOOD_SIZE)

        # print(_core.eoLinearTopology)

        import inspect
        print(inspect.getmro(_core.eoLinearTopology))
        print(inspect.getmro(_core.eoTopology))

        print("is: ",isinstance(topo,_core.eoTopology))

        bnds = bounds.bound_box(VEC_SIZE,-1.5,1.5)

        velo = _core.eoStandardVelocity(topo)

        pass

    def test_algo(self):
        config.set_solution_type('real-pso')

        VEC_SIZE = 2
        POP_SIZE = 20
        NEIGHBORHOOD_SIZE = 5

        pop = swarm.empty()
        eval = evaluator.fitness(lambda x : np.linalg.norm(x,2))
        # init = initializer.random(VEC_SIZE,bounds=bounds.bound_box(VEC_SIZE,-3,3))
        # bnds = bounds.bound_box(VEC_SIZE,-1.5,1.5)
        #
        # topo = _core.eoLinearTopology(NEIGHBORHOOD_SIZE)
        #
        # velo = _core.eoStandardVelocity(topo,1,1.6,2,bnds)
        # flight = _core.eoStandardFlight()
        #
        # cont = continuator.max_generations(50)

        # alg = _core.eoEasyPSO(cont,eval,velo)

        config.set_solution_type('gen')


if __name__ == '__main__':
    unittest.main()
