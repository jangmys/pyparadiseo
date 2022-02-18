import unittest
import numpy as np

# problem dependent
from pyparadiseo import Pop
from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# independent
from pyparadiseo.eo import algo,selector,continuator


class test_simpleHC_onemax(unittest.TestCase):
    def setUp(self):
        self.POPSIZE = 25
        self.DIM = 20
        self.NGENS = 100
        # core.FitnessTraits.set_minimizing(False)

    def test_onemax(self):
        """
        run simple GA on OneMax. expect to find at least 18/20...unlikely to happen by chance and almost sure if it works
        """
        #make pyparadiseo evaluator from python function
        eval = FitnessEval(lambda sol: np.count_nonzero(sol))
        #generate and evaluate population
        pop = Pop(self.POPSIZE, BinaryInit(self.DIM))
        PopLoopEval(eval)(pop,pop)

        #assemble simple GA
        sga = algo.SGA(
            selector.DetTournamentSelect(4),
            OnePtBitCrossover(),.1,
            DetBitFlip(),.7,
            eval,
            continuator.GenContinue(self.NGENS)
        )
        #run algo on pop and print best individual
        sga(pop)
        self.assertAlmostEqual(pop.best().fitness, 19, None, "this is very unlikely",delta=1)
        # print(pop.best())


if __name__ == "__main__":
    unittest.main()
