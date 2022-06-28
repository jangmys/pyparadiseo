# problem dependent
from pyparadiseo import Pop,Solution
from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# eo
from pyparadiseo.eo import algo,selector,continuator
# mo
from pyparadiseo import mo

from problems import onemax




if __name__ == "__main__":
    DIM = 200
    onemax=onemax.OneMax(DIM)

    #make pyparadiseo evaluator from python function
    eval = FitnessEval(lambda sol: sum(sol)) #np.count_nonzero(sol))


    # max_one = onemax.OneMax(DIM)

    # standard initializer object
    myinit = BinaryInit(DIM)
    # full evaluation
    # myeval = pp.FitnessEval(max_one.sum_bits)
    # nbor evaluation
    nborEval = mo.eval.NeighborEval(onemax.eval_incremental)
    # neighborhood
    nhood = mo.neighborhood.OrderNeighborhood(DIM)

    # algo
    hc = mo.algo.SimpleHC(nhood,eval,nborEval)
    # set move
    hc.setMove(onemax.move)

    ########################################
    # define sol / init / eval
    sol = Solution()
    myinit(sol)
    eval(sol)
    print(sol)


    hc(sol)

    print(sol)
#

    #generate and evaluate population
    pop = Pop(25, BinaryInit(DIM))
    PopLoopEval(eval)(pop,pop)

    #assemble simple GA
    sga = algo.SGA(
        selector.DetTournamentSelect(4),
        OnePtBitCrossover(),.1,
        hc,.7,
        eval,
        continuator.GenContinue(50)
    )
    #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
