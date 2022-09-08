from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import population

from pyparadiseo.eo import merge

import unittest
import inspect

POP_SIZE=5

#BF:(const oldpop&,newpop&)->void
# (like select_many...)
#merging old with new

class test_eoMerge(unittest.TestCase):
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


    def test_elitist1(self):
        #merge 1
        elitemerge = merge.elitist(1,False)

        elitemerge(self.pop1,self.pop2)

        self.assertEqual(len(self.pop1),POP_SIZE)
        self.assertEqual(len(self.pop2),POP_SIZE+1)
        self.assertEqual(int(self.pop2[POP_SIZE].fitness),4)


    def test_elitist2(self):
        #copy 40% best (=2)
        elitemerge = merge.elitist(0.4)

        elitemerge(self.pop1,self.pop2)

        self.assertEqual(len(self.pop1),POP_SIZE)
        self.assertEqual(len(self.pop2),1.4*POP_SIZE)

        self.assertEqual(int(self.pop2[POP_SIZE].fitness)+int(self.pop2[POP_SIZE+1].fitness),7)


    def test_plus(self):
        plusmerge = merge.plus()

        plusmerge(self.pop1,self.pop2)

        self.assertEqual(len(self.pop1),POP_SIZE)
        self.assertEqual(len(self.pop2),2*POP_SIZE)

        # for i,ind in enumerate(self.pop1):
        #     self.assertEqual(int(ind.fitness),i*100)
        #
        # print(self.pop1)
        #
        # print('-'*50)
        #
        # print(self.pop2)
        # for i,ind in enumerate(self.pop1):
        #     self.assertEqual(int(ind.fitness),i*100)
        #
        # #replace with larger pop
        # replace(self.pop1,self.pop_larger)
        # self.assertEqual(len(self.pop1),2*POP_SIZE)
        # for i,ind in enumerate(self.pop1):
        #     self.assertEqual(int(ind.fitness),i*10000)


    # def test_bases(self):
    #     for name, obj in inspect.getmembers(replacement):
    #         if inspect.isclass(obj):
    #             if name[0] == "_":
    #                 continue
    #             if name != "Replacement":
    #                 self.assertEqual(obj.__bases__[0].__name__,"eoReplacement")


if __name__ == "__main__":
    unittest.main()
