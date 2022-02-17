from pyparadiseo import Solution

from pyparadiseo import eo
from pyparadiseo.eo import continuator
from pyparadiseo import evaluator

from pyparadiseo.initializer import Init
from pyparadiseo.evaluator import FitnessEval
from pyparadiseo import Pop


import unittest
import inspect
import numpy as np


class test_eocontinue(unittest.TestCase):
    def setUp(self):
        self.init = Init(lambda : np.random.randint(0,2,10))
        self.eval = FitnessEval(lambda x: np.sum(x))

        self.pop = Pop(10,self.init)

        self.ind1 = Solution()
        self.init(self.ind1)

        self.ind2 = Solution()
        self.init(self.ind2)

    def tearDown(self):
        pass

    def test_bases(self):
        for name, obj in inspect.getmembers(continuator):
            if inspect.isclass(obj):
                if name == "EvalFuncCounter":
                    self.assertEqual(obj.__bases__[0].__name__,"eoEvalFunc")
                elif name != "Continue":
                    self.assertEqual(obj.__bases__[0].__name__,"eoContinue")

    def test_eoGenContinue(self):
        myGenContinue = continuator.GenContinue(42)

        c=0
        while True:
            c = c + 1
            if not myGenContinue(self.pop):
                break
        self.assertEqual(c,42)

    def test_eoEvalFuncCounter(self):
        #make counting_eval_function object from FitnessEval
        myEvalFuncCounter = evaluator.EvalFuncCounter(self.eval,"test-string")
        #make eval_continuator : 100 evaluations
        myEvalContinue = continuator.EvalContinue(myEvalFuncCounter,100)

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
