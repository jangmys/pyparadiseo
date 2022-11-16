from pyparadiseo import config

from pyparadiseo import operator
from pyparadiseo.eo import transform

import unittest
# import inspect

#TODO : for now, just testing imports and base class name ...
class test_eoTransform(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')

    def tearDown(self):
        config.set_solution_type('gen')

    def test_gen(self):
        def mutate(ind1):
            for i in range(len(ind1)):
                ind1[i]=0
            return True

        def xover(ind1,ind2):
            for i in range(len(ind1)):
                ind1[i]=0
            for i in range(len(ind2)):
                ind2[i]=1
            return True

        mymut = operator.make_mutation(mutate)
        mycross = operator.make_crossover(xover)

        SGATransform = transform.SGA(mycross,0.5,mymut,0.5)

        self.assertTrue(callable(SGATransform))


    def test_real(self):
        config.set_solution_type('real')

        def mutate(ind1):
            for i in range(len(ind1)):
                ind1[i]=0
            return True

        def xover(ind1,ind2):
            for i in range(len(ind1)):
                ind1[i]=0
            for i in range(len(ind2)):
                ind2[i]=1
            return True

        mymut = operator.make_mutation(mutate)
        mycross = operator.make_crossover(xover)



        SGATransform = transform.SGA(mycross,0.5,mymut,0.5)


    #
    #     self.assertTrue(callable(SGATransform))


if __name__ == '__main__':
    unittest.main()
