import pyparadiseo

from pyparadiseo import rng
from pyparadiseo import config

from pyparadiseo import population
from pyparadiseo import initializer
from pyparadiseo.eo import selector

from pyparadiseo.eo import select_one

import numpy as np
import unittest
import inspect


class test_eoSelector(unittest.TestCase):
    def setUp(self):
        config.set_maximize_fitness()
        POP_SIZE=20

        rng().reseed(42)

        self.init = initializer.initializer(lambda : np.random.randint(0,2,10))
        self.pop = population.from_init(POP_SIZE,self.init)
        for i in range(len(self.pop)):
            self.pop[i].fitness = i

        #mock random
        self.det_randmock=[]
        for i in range(5*POP_SIZE):
            self.det_randmock.append(rng().random(POP_SIZE))

        rng().reseed(42)
        self.stoch_randmock55=[]
        for i in range(100):
            self.stoch_randmock55.append(rng().random(POP_SIZE))
            self.stoch_randmock55.append(rng().random(POP_SIZE))
            self.stoch_randmock55.append(rng().flip(.55))

        rng().reseed(42)
        self.stoch_randmock85=[]
        for i in range(100):
            self.stoch_randmock85.append(rng().random(POP_SIZE))
            self.stoch_randmock85.append(rng().random(POP_SIZE))
            self.stoch_randmock85.append(rng().flip(.85))

        rng().reseed(42)
        self.unif_mock=[]
        for i in range(100):
            self.unif_mock.append(rng().uniform())

        rng().reseed(42)


    def test_decorator(self):
        @select_one.select_one
        class select_x():
            def __init__(self,num):
                self.num = num
            def __call__(self,pop):
                return pop[self.num]

        for x in [2,3,5,7,11]:
            sel = select_x(x)
            self.assertTrue(isinstance(sel,select_one.SelectOne))

            a=int(sel(self.pop).fitness)
            self.assertEqual(a,x)


    def test_random(self):
        """select random individuals"""
        rng().reseed(42)

        sel = select_one.random()

        for i in range(10):
            a=int(sel(self.pop).fitness)
            self.assertEqual(self.det_randmock[i],a)


    def test_det_tournament_max(self):
        #default tournament size is 2
        sel = select_one.det_tournament()

        for i in range(0,20,2):
            a=int(sel(self.pop).fitness)
            expect = max(self.det_randmock[i:i+2])
            self.assertEqual(a,expect)

        def run_test(t_size):
            rng().reseed(42)
            sel = select_one.det_tournament(t_size)

            for i in range(0,20,t_size):
                a=int(sel(self.pop).fitness)
                expect = max(self.det_randmock[i:i+t_size])
                self.assertEqual(a,expect)

        run_test(2)
        run_test(4)
        run_test(5)
        run_test(10)
        run_test(20)


    def test_det_tournament_min(self):
        config.set_minimize_fitness()

        #default tournament size is 2
        sel = select_one.det_tournament()

        for i in range(0,20,2):
            a=int(sel(self.pop).fitness)
            expect = min(self.det_randmock[i:i+2])
            self.assertEqual(a,expect)

        def run_test(t_size):
            rng().reseed(42)
            sel = select_one.det_tournament(t_size)

            for i in range(0,20,t_size):
                a=int(sel(self.pop).fitness)
                expect = min(self.det_randmock[i:i+t_size])
                self.assertEqual(a,expect)

        run_test(2)
        run_test(4)
        run_test(5)
        run_test(10)
        run_test(20)


    def test_stoch_tournament_max(self):
        #test for maximization
        sel = select_one.stoch_tournament(0.55)

        for i in range(20):
            a=int(sel(self.pop).fitness)

            i1 = self.stoch_randmock55[3*i]
            i2 = self.stoch_randmock55[3*i+1]
            flip = self.stoch_randmock55[3*i+2]

            if flip:
                expect = max(i1,i2)
            else:
                expect = min(i1,i2)

            self.assertEqual(a,expect)

        #test for minimization with different t_rate
        rng().reseed(42)
        config.set_minimize_fitness()

        sel = select_one.stoch_tournament(0.85)

        for i in range(20):
            a=int(sel(self.pop).fitness)

            i1 = self.stoch_randmock85[3*i]
            i2 = self.stoch_randmock85[3*i+1]
            flip = self.stoch_randmock85[3*i+2]

            if flip:
                expect = min(i1,i2)
            else:
                expect = max(i1,i2)

            self.assertEqual(a,expect)


    def test_truncated(self):
        rng().reseed(42)

        #wrap
        rand_select = select_one.random()
        trunc_rand_select = select_one.truncated(rand_select,1.0)

        #don't forget this
        trunc_rand_select.setup(self.pop)

        #with 1.0 trunc uses full pop
        for i in range(10):
            a=int(trunc_rand_select(self.pop).fitness)
            self.assertEqual(a,self.det_randmock[i])

        #retry with 0.5
        rng().reseed(42)

        #take only 3 best
        trunc_rand_select = select_one.truncated(rand_select,1,False)
        #don't forget this
        trunc_rand_select.setup(self.pop)

        for i in range(10):
            a=int(trunc_rand_select(self.pop).fitness)
            self.assertGreater(a,16)


        rng().reseed(42)
        #take only among best half
        trunc_rand_select = select_one.truncated(rand_select,0.5)
        #don't forget this
        trunc_rand_select.setup(self.pop)

        for i in range(20):
            a=int(trunc_rand_select(self.pop).fitness)
            self.assertGreater(a,9)


    def test_best(self):
        sel = select_one.best()

        for i in range(10):
            a=int(sel(self.pop).fitness)
            self.assertEqual(a,19)

    def test_proportional(self):
        """roulette wheel"""
        # self.pop.shuffle()

        cumul=[0.0]
        for i in range(1,len(self.pop)):
            cumul.append(cumul[-1]+i)

        for i in range(len(cumul)):
            cumul[i] = cumul[i]/cumul[-1]

        import bisect

        sel = select_one.proportional()

        sel.setup(self.pop)
        for i in range(100):
            a=int(sel(self.pop).fitness)
            self.assertEqual(bisect.bisect_right(cumul,self.unif_mock[i]),a)


    def test_round_robin(self):
        sel = select_one.round_robin()

        for i in range(30):
            a=int(sel(self.pop).fitness)
            self.assertEqual(a,i%20)


    def test_sequential(self):
        self.pop.shuffle()

        sel = select_one.sequential()

        for i in range(10):
            a=int(sel(self.pop).fitness)
            # print(a)

            # self.assertEqual(a,i%20)


    def test_ctor(self):
        #abstract : should not be able to construct this?!
        sel = select_one.SelectOne()
        self.assertTrue(callable(sel))

        sel = select_one.det_tournament()
        self.assertTrue(callable(sel))

        sel = select_one.det_tournament(42)
        self.assertTrue(callable(sel))

        sel = select_one.truncated(sel,0.7)
        self.assertTrue(callable(sel))

        sel = select_one.random()
        self.assertTrue(callable(sel))

        sel = select_one.round_robin()
        self.assertTrue(callable(sel))

        sel = select_one.sequential()
        self.assertTrue(callable(sel))

        sel = select_one.sequential(True)
        self.assertTrue(callable(sel))

        sel = select_one.elite_sequential()
        self.assertTrue(callable(sel))

    def do_test(self,selector):
        # rng = pyparadiseo.rng()
        pyparadiseo.rng().reseed(42)
        nTries = 5000

        print('*'*20, "Testing", str(selector.__class__), '*'*20)
        print('ind#\t', "/",nTries,"\t", "%\t")
        selection = [0. for i in range(len(self.pop))]

        for i in range(nTries):
            selection[ int(selector(self.pop).fitness) ] += 1

        for i in range(len(self.pop)):
            print(i,"\t", selection[i],"\t", selection[i] / nTries)

        return selection, nTries

    # def test_DetTournament(self):
    #     sel = select_one.det_tournament()
    #     self.do_test(sel)
    #     sel = select_one.det_tournament(1)
    #     self.do_test(sel)
    #     sel = select_one.det_tournament(2)
    #     self.do_test(sel)
    #     sel = select_one.det_tournament(3)
    #     self.do_test(sel)
    #     sel = select_one.det_tournament(10)
    #     self.do_test(sel)



if __name__ == "__main__":
    unittest.main()
