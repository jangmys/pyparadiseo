import unittest
import numpy as np

# problem dependent
from pyparadiseo import population
from pyparadiseo import config
from pyparadiseo import evaluator

# encoding dependent
from pyparadiseo.initializer import Init,BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,_DetBitFlip
# independent
from pyparadiseo.eo import algo,select_one,continuator


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
        config.set_solution_type('gen')

        #make pyparadiseo evaluator from python function
        eval = evaluator.fitness(lambda sol: np.count_nonzero(sol))

        #generate and evaluate population
        pop = population.from_init(self.POPSIZE, Init(lambda : np.random.choice([True,False],self.DIM)))
        evaluator.pop_eval_from_fitness(eval)(pop,pop)

        #assemble simple GA
        sga = algo.simpleGA(
            select_one.det_tournament(4),
            OnePtBitCrossover(),.1,
            _DetBitFlip(),.7,
            eval,
            continuator.max_generations(self.NGENS)
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

        from pyparadiseo import evaluator
        from pyparadiseo import pop
        from pyparadiseo import initializer

        from pyparadiseo.operator import OnePtBitCrossoverBin,DetBitFlipBin

        config.set_solution_type('bin')

        #make pyparadiseo evaluator from python function
        # eval = factory.get_eval('fitness',lambda sol: np.count_nonzero(sol))
        eval = evaluator.fitness(lambda sol: np.count_nonzero(sol))
        # print(type(eval))

        init = initializer.random(size=self.DIM)
        print(type(init))

        # #generate and evaluate population
        pop = pop.from_init(self.POPSIZE, init, type='bin')
        print(type(pop))

        p_eval=evaluator.pop_eval_from_fitness(eval)
        p_eval(pop,pop)

        print(pop)

        # print(type(p_eval))

        # p_eval=_PopEval(lambda sol: np.count_nonzero(sol))
        # print(type(p_eval))

        # #assemble simple GA
        sga = algo.simpleGA(
            select_one.det_tournament(4),
            OnePtBitCrossoverBin(),.1,
            DetBitFlipBin(),.7,
            eval,
            continuator.max_generations(self.NGENS)
        )
        #run algo on pop and print best individual
        sga(pop)
        self.assertAlmostEqual(pop.best().fitness, 19, None, "this is very unlikely",delta=1)


if __name__ == "__main__":
    unittest.main()
