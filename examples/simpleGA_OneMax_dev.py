# problem dependent
from pyparadiseo import BinaryPop
from pyparadiseo import evaluator
# encoding dependent
from pyparadiseo.initializer import BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# independent
from pyparadiseo.eo import algo,selector,continuator

if __name__ == "__main__":
    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: sum(sol)) #np.count_nonzero(sol))
    #generate and evaluate population
    pop = BinaryPop(25, BinaryInit(20))
    evaluator.pop_eval_from_fitness(eval)(pop,pop)

    #assemble simple GA
    sga = algo.SGA(
        selector.DetTournamentSelect(4),
        OnePtBitCrossover(),.1,
        DetBitFlip(),.7,
        eval,
        continuator.GenContinue(50)
    )
    #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
