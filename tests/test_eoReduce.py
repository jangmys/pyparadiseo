from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import population

from pyparadiseo.eo import reduce

import unittest
import inspect

POP_SIZE=5

class test_eoReduce(unittest.TestCase):
    def setUp(self):
        config.set_maximize_fitness()
        config.set_solution_type('bin')

        self.init = initializer.random(10)
        self.pop1 = population.from_init(POP_SIZE,self.init)
        self.pop2 = population.from_init(POP_SIZE,self.init)

        self.pop_larger = population.from_init(2*POP_SIZE,self.init)

        for i in range(len(self.pop1)):
            self.pop1[i].fitness = i
            self.pop2[i].fitness = i*100

        for i in range(len(self.pop_larger)):
            self.pop_larger[i].fitness = i*10000


    def tearDown(self):
        config.set_maximize_fitness()
        config.set_solution_type('gen')


    def test_truncate(self):
        #keep best
        kill = reduce.truncate()

        kill(self.pop1,1)
        self.assertEqual(len(self.pop1),1)
        self.assertEqual(int(self.pop1[0].fitness),4)


    def test_keeprandom(self):
        #keep random
        kill = reduce.random()

        kill(self.pop1,3)
        self.assertEqual(len(self.pop1),3)


    def test_linreduce(self):
        #keep random
        kill = reduce.linear()

        c=1
        while len(self.pop1)>0:
            kill(self.pop1,len(self.pop1)-1)
            self.assertEqual(len(self.pop1),POP_SIZE-c)

            if len(self.pop1)>0:
                self.assertEqual(int(self.pop1.worst().fitness),c)
                c += 1




if __name__ == "__main__":
    unittest.main()
