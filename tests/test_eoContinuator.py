from pyparadiseo import config

from pyparadiseo import solution

from pyparadiseo import eo
from pyparadiseo.eo import continuator
from pyparadiseo import evaluator
from pyparadiseo import initializer

# from pyparadiseo.initializer import Init
from pyparadiseo import population


import unittest
import inspect
import numpy as np


class test_eocontinue(unittest.TestCase):
    def setUp(self):
        config.set_solution_type('gen')

        self.init = initializer.initializer(lambda : np.random.randint(0,2,10))
        self.eval_raw = lambda x: np.sum(x)
        self.eval = evaluator.fitness(self.eval_raw)
        self.pop = population.from_init(10,self.init)

        self.ind1 = solution.empty()
        self.init(self.ind1)

        self.ind2 = solution.empty()
        self.init(self.ind2)

    def tearDown(self):
        pass

    def test_inherit(self):
        from pyparadiseo._core import eoContinue

        class myCont(eoContinue):
            def __init__(self):
                eoContinue.__init__(self)
            def __call__(self,pop):
                return True

        truecont = myCont()
        # print(inspect.getmro(type(truecont)))


    def test_pycont(self):
        class max_gens():
            def __init__(self,_num):
                self.lastGen = _num
                self.currentGen = 0
            def __call__(self,pop):
                self.currentGen = self.currentGen + 1
                if self.currentGen == self.lastGen:
                    return False
                else:
                    return True

        cont = continuator.continuator(max_gens(12))

        c=0
        while True:
            c = c + 1
            if not cont(self.pop):
                break
        self.assertEqual(c,12)
        # self.assertEqual(cont.currentGen,12)

    def test_eoGenContinue(self):
        myGenContinue = continuator.max_generations(42)

        c=0
        while True:
            c = c + 1
            if not myGenContinue(self.pop):
                break
        self.assertEqual(c,42)

    def test_eoEvalFuncCounter(self):
        #make counting_eval_function object from FitnessEval
        myEvalFuncCounter = evaluator.counting_fitness(self.eval)
        #make eval_continuator : 100 evaluations
        myEvalContinue = continuator.eval_calls(myEvalFuncCounter,100)

        c=0
        # stop after 100 evaluations
        while myEvalContinue(self.pop):
            #if we don't invalidate, eval won't re-evaluate
            self.ind1.invalidate()
            myEvalFuncCounter(self.ind1)
            c = c + 1
        self.assertEqual(c,100)


        #make counting_eval_function object from FitnessEval
        myEvalFuncCounter = evaluator.fitness(self.eval_raw,True)
        #make eval_continuator : 100 evaluations
        myEvalContinue = continuator.eval_calls(myEvalFuncCounter,100)

        c=0
        # stop after 100 evaluations
        while myEvalContinue(self.pop):
            #if we don't invalidate, eval won't re-evaluate
            self.ind1.invalidate()
            myEvalFuncCounter(self.ind1)
            c = c + 1
        self.assertEqual(c,100)




if __name__ == '__main__':
    unittest.main()
