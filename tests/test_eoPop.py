import pyparadiseo as pp

from pyparadiseo import config
from pyparadiseo import population
from pyparadiseo import solution

from pyparadiseo import initializer
from pyparadiseo import bounds

import unittest
import numpy as np

def isNonIncreasing(pop):
    for i in range(1,len(pop)):
        if pop[i-1].fitness < pop[i].fitness:
            return False
    return True


class Test_Pop(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')
        config.set_maximize_fitness()

        self.init = initializer.initializer(lambda : np.arange(10)) #just something
        self.bininit = initializer.random(10,stype='bin')
        self.realinit = initializer.random(bounds=([-1]*10,[1]*10),stype='real')
        self.perminit = initializer.random(10,stype='perm')

        self.pop = population.empty()
        self.binpop = population.empty(stype='bin')
        self.realpop = population.empty(stype='real')
        self.permpop = population.empty(stype='perm')



    def tearDown(self):
        self.pop.resize(0)
        self.binpop.resize(0)
        self.realpop.resize(0)

    def test_from_init(self):
        pop = population.from_init(10,self.init)
        self.assertEqual(len(pop),10)

        pop = population.from_init(10,self.realinit)
        self.assertEqual(len(pop),10)

    #
    def test_resize(self):
        def t_resize(pop):
            self.assertEqual(len(pop),0)
            pop.resize(100)
            self.assertEqual(len(pop),100)

            for i,ind in enumerate(pop):
                ind.fitness = i

            pop.resize(10)
            self.assertEqual(len(pop),10)
            self.assertEqual(pop[9].fitness,9)

        t_resize(self.pop)
        t_resize(self.binpop)
        t_resize(self.realpop)


    def test_indexing(self):
        self.pop = population.from_init(5, self.init)
        self.assertEqual(len(self.pop),5)

        # --------------replace all content using slice [:]--------------
        N=3
        self.pop[:] = [solution.solution([_]) for _ in range(N)]
        self.assertEqual(len(self.pop),N)

        print("pop",self.pop)
        # --------------indexing +/- --------------
        self.assertEqual(self.pop[0][0],0)
        self.assertEqual(self.pop[1][0],1)
        self.assertEqual(self.pop[2][0],2)
        self.assertEqual(self.pop[-1][0],2)
        self.assertEqual(self.pop[-2][0],1)

        # --------------deleting--------------
        del self.pop[1]
        self.assertEqual(len(self.pop),N-1)
        self.assertEqual(self.pop[1][0],2)

        # --------------mutate a container element--------------
        self.pop[0].fitness = 42
        self.pop[1].fitness = 43
        self.assertEqual(self.pop[0].fitness,42)
        self.assertEqual(self.pop[1].fitness,43)
        self.pop[0].invalidate()
        self.assertTrue(self.pop[0].invalid())

        # --------------copy a container element--------------
        from copy import copy
        s = copy(self.pop[1])
        self.assertEqual(s.fitness,43)
        s.invalidate()
        self.assertTrue(s.invalid())
        #shouldn't change!
        self.assertEqual(self.pop[1].fitness,43)

        #----------------reference a container element---------------
        s = self.pop[1]
        s.invalidate()
        #should change!
        self.assertTrue(self.pop[1].invalid())

        #-----------------slicing-------------------
        print("-"*10,"slice","-"*10)
        N=5
        self.pop[:] = [solution.solution([_]) for _ in range(N)]
        for i,ind in enumerate(self.pop):
            ind.fitness = i

        sl = self.pop[0:2]
        for i,ind in enumerate(sl):
            self.assertEqual(ind[0],i)

        sl[0].invalidate()
        self.assertTrue(sl[0].invalid())
        self.assertFalse(self.pop[0].invalid())

        #----------------slicing : replace----------------
        # replaces pop[1] and pop[2] by solution(['z'])
        self.pop[1:3] = solution.solution(['z'])
        self.assertEqual(len(self.pop),4)
        self.assertEqual(self.pop[1][0],'z')

        # self.pop[0:2] = [[1.0],[2.0],[3.0]] # perform implicit conversion ??
        self.pop[0:2] = [solution.solution([2*_]) for _ in range(5)]
        self.assertEqual(len(self.pop),7)
        self.assertEqual(self.pop[3][0],6)

        #----------------slicing : delete----------------
        del self.pop[3:5]
        self.assertEqual(len(self.pop),5)

        #----------------slicing : replace trailing----------------
        self.pop[2:] = [solution.solution([100*_]) for _ in range(4)]
        self.assertEqual(len(self.pop),6)
        self.assertEqual(self.pop[3][0],100)

        #----------------slicing : del leading----------------
        del self.pop[:2]
        self.assertEqual(len(self.pop),4)
        self.assertEqual(self.pop[1][0],100)

        #append tuple front
        self.pop[0:0] = (solution.solution([123]),solution.solution([456]))
        self.assertEqual(len(self.pop),6)

        # -------------- references ---------------------
        # reset...
        N=4
        self.pop[:] = [solution.solution([_]) for _ in range(N)]
        i0 = self.pop[0]
        i1 = self.pop[1]
        i2 = self.pop[2]
        i3 = self.pop[3]

        del self.pop[2]

        self.assertEqual(i0[0],0)
        self.assertEqual(i1[0],1)
        self.assertEqual(i2[0],2) #ref still valid
        self.assertEqual(i3[0],3) #ref still valid !

        del self.pop[:] #del all

        self.assertEqual(i0[0],0) #ref still valid
        self.assertEqual(i1[0],1) #ref still valid
        self.assertEqual(i2[0],2) #ref still valid
        self.assertEqual(i3[0],3) #ref still valid !


    def test_pickle(self):
        import pickle

        pop = population.from_init(10,self.realinit)
        for i,ind in enumerate(pop):
            ind.fitness = i
            ind.diversity = i**2
            ind.objectives = [10*i,11*i]

        d=pickle.dumps(pop)
        pop2=pickle.loads(d)

        for i,ind in enumerate(pop):
            np.testing.assert_allclose(ind.encoding,pop2[i].encoding)
            np.testing.assert_allclose(ind.carray,pop2[i].carray)
            np.testing.assert_allclose(ind.objectives,pop2[i].objectives)
            np.testing.assert_allclose(ind.fitness,pop2[i].fitness)
            np.testing.assert_allclose(ind.diversity,pop2[i].diversity)


    def test_append(self):
        def t_append(pop,init):
            pop.append(10,init)
            self.assertEqual(len(pop),10)
            pop.append(20,init)
            self.assertEqual(len(pop),20)
            with self.assertRaises(RuntimeError):
                pop.append(10,init)
        t_append(self.pop,self.init)
        t_append(self.binpop,self.bininit)
    #     t_append(self.realpop,self.realinit)

    def test_sort(self):
        def t_sort(pop):
            pop.resize(20)
            # sort fails if fitness not set
            with self.assertRaises(RuntimeError):
                pop.sort()

            for i in pop:
                i.fitness = np.random.randint(100)

            pop.sort()
            self.assertTrue(isNonIncreasing(pop),"maximize : sorted pop should be non-increasing")
        t_sort(self.pop)
        t_sort(self.binpop)
        t_sort(self.realpop)

    def test_shuffle(self):
        def t_shuffle(pop):
            pop.resize(100)
            for i,ind in enumerate(pop):
                ind.fitness = i
            pop.shuffle()
            self.assertFalse(isNonIncreasing(pop),"shuffled pop is sorted. this is highly unlikely.")

        t_shuffle(self.pop)
        t_shuffle(self.binpop)


    def test_best_worst(self):
        def t_best_worst(pop):
            pop.resize(100)
            for i,ind in enumerate(pop):
                ind.fitness = i
            pop.shuffle()

            pp.config.set_maximize_fitness()
            self.assertEqual(pop.best().fitness,99,"maximize : 99.0 is best")
            self.assertEqual(pop.worst().fitness,0,"maximize : 0.0 is worst")
            pp.config.set_minimize_fitness()
            self.assertEqual(pop.best().fitness,0,"minimize : 0.0 is best")
            self.assertEqual(pop.worst().fitness,99,"minimize : 99.0 is worst")


        t_best_worst(self.pop)
        t_best_worst(self.binpop)


    def test_push_back(self):
        def t_push_back(pop,init,ind):
            pop.append(10,init)
            # ind = solution.empty()
            init(ind)
            ind.fitness = 42
            pop.push_back(ind)
            self.assertEqual(len(pop), 11)
            self.assertEqual(pop[10].fitness, 42)

        ind = solution.empty()
        t_push_back(self.pop,self.init,ind)
        ind = solution.empty(stype='bin')
        t_push_back(self.binpop,self.bininit,ind)


    def test_swap(self):
        def t_swap(pop,init):
            pop2 = population.from_init(10,init)
            self.assertEqual(len(pop2),10)
            for i,ind in enumerate(pop2):
                ind.fitness = i

            pop2.swap(pop)
            self.assertEqual(len(pop),10)
            for i,ind in enumerate(pop):
                self.assertEqual(ind.fitness,i)

            self.assertEqual(len(pop2),0)
        t_swap(self.pop,self.init)
        t_swap(self.binpop,self.bininit)
        t_swap(self.realpop,self.realinit)

if __name__ == '__main__':
    unittest.main()
