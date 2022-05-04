import unittest
import numpy as np

# problem dependent
from pyparadiseo import Pop
from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import Init,BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,_DetBitFlip
# independent
from pyparadiseo.eo import algo,selector,continuator


class test_simpleHC_onemax(unittest.TestCase):
    def setUp(self):
        self.POPSIZE = 25
        self.DIM = 20
        self.NGENS = 100
        # core.FitnessTraits.set_minimizing(False)

    def test_onemax_gen(self):
        """
        run simple GA on OneMax. expect to find at least 18/20...unlikely to happen by chance and almost sure if it works
        """
        #make pyparadiseo evaluator from python function
        eval = FitnessEval(lambda sol: np.count_nonzero(sol))

        #generate and evaluate population
        pop = Pop(self.POPSIZE, Init(lambda : np.random.choice([True,False],self.DIM)))
        PopLoopEval(eval)(pop,pop)

        #assemble simple GA
        sga = algo.SGA(
            selector.DetTournamentSelect(4),
            OnePtBitCrossover(),.1,
            _DetBitFlip(),.7,
            eval,
            continuator.GenContinue(self.NGENS)
        )
        #run algo on pop and print best individual
        sga(pop)
        self.assertAlmostEqual(pop.best().fitness, 19, None, "this is very unlikely",delta=1)

    def test_onemax_bin(self):
        """
        run simple GA on OneMax. expect to find at least 18/20...unlikely to happen by chance and almost sure if it works
        """
        import pyparadiseo
        from pyparadiseo import factory
        from pyparadiseo import config

        from pyparadiseo.initializer import _Init
        from pyparadiseo.evaluator import _FitnessEval,_PopEval
        from pyparadiseo.pop import _Pop

        from pyparadiseo.operator import OnePtBitCrossoverBin,DetBitFlipBin

        config.set_solution_type('bin')

        #make pyparadiseo evaluator from python function
        # eval = factory.get_eval('fitness',lambda sol: np.count_nonzero(sol))
        eval = _FitnessEval(lambda sol: np.count_nonzero(sol))
        # print(type(eval))

        init = _Init(size=self.DIM)
        print(type(init))
        # FitnessEval(lambda sol: np.count_nonzero(sol))

        # #generate and evaluate population
        pop = _Pop(self.POPSIZE, init)
        print(type(pop))

        p_eval=_PopEval(eval)
        p_eval(pop,pop)

        print(pop)

        # print(type(p_eval))

        # p_eval=_PopEval(lambda sol: np.count_nonzero(sol))
        # print(type(p_eval))


        # PopLoopEval(eval)(pop,pop)
        #
        # #assemble simple GA
        sga = algo._SGA(
            selector._DetTournamentSelect(4),
            OnePtBitCrossoverBin(),.1,
            DetBitFlipBin(),.7,
            eval,
            continuator._GenContinue(self.NGENS)
        )
        #run algo on pop and print best individual
        sga(pop)
        self.assertAlmostEqual(pop.best().fitness, 19, None, "this is very unlikely",delta=1)


if __name__ == "__main__":
    unittest.main()
