from pyparadiseo import config

import pyparadiseo as pp
# problem dependent
from pyparadiseo import population
from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import operator

# from pyparadiseo import Pop
# from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import BinaryInit,BinarySolInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# from pyparadiseo.pop import _Pop
# independent
from pyparadiseo.eo import algo,select_one,continuator


if __name__ == "__main__":
    #set global solution type 'bin'
    config.set_solution_type('bin')

    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: sum(sol))

    #generate and evaluate population
    pop=population.from_init(25,initializer.random(20))
    evaluator.pop_eval_from_fitness(eval)(pop,pop)

    #assemble simple GA
    sga = algo.simpleGA(
        select_one.det_tournament(4),
        operator.OnePtBitCrossoverBin(),.1,
        operator.DetBitFlipBin(),.7,
        eval,
        continuator.max_generations(50)
    )
    # #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
