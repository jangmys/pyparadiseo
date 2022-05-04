import pyparadiseo as pp
# problem dependent
from pyparadiseo import Pop
from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import BinaryInit,BinarySolInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# independent
from pyparadiseo.eo import algo,selector,continuator

if __name__ == "__main__":
    pop=pp.create_population(pop_size=25,f_init=BinarySolInit(20),stype="binary")

    print(pop)

    #make pyparadiseo evaluator from python function
    eval = FitnessEval(lambda sol: sum(sol))
    eval(pop[0])
    print(pop)

    print(pop[0])

    PopLoopEval(eval)(pop,pop)


     #np.count_nonzero(sol))
    #generate and evaluate population
    pop=pp.create_population(25,BinaryInit(20))
    # pop = Pop(25, BinaryInit(20))
    PopLoopEval(eval)(pop,pop)

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
