from pyparadiseo import config

# problem dependent
from pyparadiseo import population
from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import operator

from pyparadiseo.eo import algo,select_one,continuator

DIM=20
POP_SIZE=25
MAX_GEN=100

if __name__ == "__main__":
    #set global solution type 'bin'
    config.set_solution_type('bin')

    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: sum(sol))

    #generate and evaluate population
    pop=population.from_init(POP_SIZE,initializer.random(DIM))
    evaluator.pop_eval_from_fitness(eval)(pop,pop)

    #assemble simple GA
    sga = algo.simpleGA(
        select_one.det_tournament(4),
        operator.OnePtBitCrossover(),.1,
        operator.DetBitFlip(),.7,
        eval,
        continuator.max_generations(MAX_GEN)
    )
    # #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
