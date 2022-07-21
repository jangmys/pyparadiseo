import unittest
import numpy as np

# problem dependent
from pyparadiseo import population
from pyparadiseo import config
from pyparadiseo import evaluator

# encoding dependent
from pyparadiseo.operator import OnePtBitCrossover
# independent
from pyparadiseo.eo import algo,select_one,continuator


class test_simpleHC_onemax(unittest.TestCase):
    def setUp(self):
        self.POPSIZE = 25
        self.DIM = 20
        self.NGENS = 100 #100

    def test_onemax_bin(self):
        """
        run simple GA on OneMax. expect to find at least 18/20...unlikely to happen by chance and almost sure if it works
        """
        from pyparadiseo import config

        from pyparadiseo import operator
        from pyparadiseo import evaluator
        from pyparadiseo import population
        from pyparadiseo import initializer

        config.set_solution_type('bin')

        #make pyparadiseo evaluator from python function
        eval = evaluator.fitness(lambda sol: np.count_nonzero(sol))
        init = initializer.random(size=self.DIM)
        # #generate and evaluate population
        pop = population.from_init(self.POPSIZE, init)

        p_eval=evaluator.pop_eval_from_fitness(eval)
        p_eval(pop,pop)

        # #assemble simple GA
        sga = algo.simpleGA(
            select_one.det_tournament(4),
            operator.OnePtBitCrossover(),.1,
            operator.DetBitFlip(),.7,
            eval,
            continuator.max_generations(self.NGENS)
        )
        #run algo on pop and print best individual

        # print(pop)
        sga(pop)

        # print("#"*20)
        # print(pop)
        #
        # print("-"*20)
        # for ind in pop:
        #     eval(ind)
        #
        # print("-"*20)
        # for ind in pop:
        #     print(f_eval(ind))
        #
        # print("-"*20)
        # for ind in pop:
        #     print(f_eval(ind.array))

        # print(pop.best())

        self.assertAlmostEqual(pop.best().fitness, self.DIM-1, None, "this is very unlikely",delta=1)

        # core.FitnessTraits.set_minimizing(False)
        #
    def test_onemax_gen(self):
        """
        run simple GA on OneMax. expect to find at least 18/20...unlikely to happen by chance and almost sure if it works
        """
        config.set_solution_type('gen')
        #
        #     #make pyparadiseo evaluator from python function
        #     eval = evaluator.fitness(lambda sol: np.count_nonzero(sol))
        #
        #     #generate and evaluate population
        #     pop = population.from_init(self.POPSIZE, Init(lambda : np.random.choice([True,False],self.DIM)))
        #     evaluator.pop_eval_from_fitness(eval)(pop,pop)
        #
        #     #assemble simple GA
        #     sga = algo.simpleGA(
        #         select_one.det_tournament(4),
        #         OnePtBitCrossover(),.1,
        #         _DetBitFlip(),.7,
        #         eval,
        #         continuator.max_generations(self.NGENS)
        #     )
        #     #run algo on pop and print best individual
        #     sga(pop)
        #     self.assertAlmostEqual(pop.best().fitness, self.DIM-1, None, "this is very unlikely",delta=1)

if __name__ == "__main__":
    unittest.main()
