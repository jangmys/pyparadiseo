from pyparadiseo import _core

# from pyparadiseo import Solution
from pyparadiseo import population
from pyparadiseo import operator,initializer,bounds

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
        topo = _core.eoLinearTopology(10)

        pop = _core.RealParticlePop()
        for i in range(100):
            pop.push_back(_core.RealParticle(10,1.0,1.0,1.0))

        for ind in pop:
            ind.fitness = 4.2
        topo.setup(pop)

        print(topo.best(0))
        print(topo.globalBest())

    def test_stdvelo(self):
        # velo = _core.eoStandardVelocity

        pass

if __name__ == '__main__':
    unittest.main()
