from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import population

from pyparadiseo.eo import replacement

import unittest
import inspect

POP_SIZE=5

#TODO : for now, just testing imports and base class name ...
class test_eoReplacement(unittest.TestCase):
    def setUp(self):
        config.set_maximize_fitness()
        config.set_solution_type('bin')

        self.init = initializer.random(10)
        self.pop1 = population.from_init(POP_SIZE,self.init)
        self.pop2 = population.from_init(POP_SIZE,self.init)

        self.pop_larger = population.from_init(2*POP_SIZE,self.init)

        self.pop_one = population.from_init(1,self.init)

        for i in range(len(self.pop1)):
            self.pop1[i].fitness = i
            self.pop2[i].fitness = i*100

        for i in range(len(self.pop_larger)):
            self.pop_larger[i].fitness = i*10000

        self.pop_one[0].fitness = 777

    def tearDown(self):
        config.set_maximize_fitness()
        config.set_solution_type('gen')

    def test_generational(self):
        #swapping populations
        replace = replacement.generational()

        #replace with same size pop
        replace(self.pop1,self.pop2)
        for i,ind in enumerate(self.pop1):
            self.assertEqual(int(ind.fitness),i*100)

        #replace with larger pop
        replace(self.pop1,self.pop_larger)
        self.assertEqual(len(self.pop1),2*POP_SIZE)
        for i,ind in enumerate(self.pop1):
            self.assertEqual(int(ind.fitness),i*10000)


    def test_plus(self):
        replace = replacement.plus()

        #result is pop1
        replace(self.pop1,self.pop2)

        self.assertEqual(int(self.pop1[0].fitness),400)
        self.assertEqual(int(self.pop1[1].fitness),300)
        self.assertEqual(int(self.pop1[2].fitness),200)
        self.assertEqual(int(self.pop1[3].fitness),100)
        self.assertEqual(int(self.pop1[4].fitness),4)


    def test_comma(self):
        replace = replacement.comma()

        replace(self.pop1,self.pop2)

        #equal size. pop2 replaces pop1 (no sort)
        for i in range(POP_SIZE):
            self.assertEqual(int(self.pop1[i].fitness),i*100)


    def test_ssga_worse(self):
        replace = replacement.ssga_worse()

        #|offspring|=1 : replace 1
        replace(self.pop1,self.pop_one)

        self.assertEqual(int(self.pop1[POP_SIZE-1].fitness),777)

        #|offspring|=|parents| : replace all
        replace(self.pop1,self.pop2)

        for i in range(POP_SIZE):
            self.assertEqual(int(self.pop1[i].fitness),i*100)

    # def test_bases(self):
    #     for name, obj in inspect.getmembers(replacement):
    #         if inspect.isclass(obj):
    #             if name[0] == "_":
    #                 continue
    #             if name != "Replacement":
    #                 self.assertEqual(obj.__bases__[0].__name__,"eoReplacement")


if __name__ == "__main__":
    unittest.main()
