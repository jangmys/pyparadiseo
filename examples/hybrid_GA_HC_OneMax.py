#general
from pyparadiseo import config

# problem dependent
from pyparadiseo import population,solution
from pyparadiseo import evaluator
from pyparadiseo import initializer
from pyparadiseo import operator
# eo
from pyparadiseo.eo import algo,selector,select_one,continuator
# mo
from pyparadiseo import mo

from problems import onemax




if __name__ == "__main__":
    config.set_solution_type("bin")

    DIM = 200
    onemax=onemax.OneMax(DIM)

    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: sum(sol)) #np.count_nonzero(sol))

    # standard initializer object
    myinit = initializer.random(DIM)
    # nbor evaluation
    nborEval = mo.eval.neighbor_eval(onemax.eval_incremental)
    # neighborhood
    nhood = mo.neighborhood.ordered(DIM)

    # algo
    hc = mo.algo.simple_hill_climber(nhood,eval,nborEval,onemax.move)

    ########################################
    # define sol / init / eval
    sol = solution.empty()
    myinit(sol)
    eval(sol)
    print("random solution: ",sol)

    hc(sol)
    print("after hill climb: ",sol)

    #generate and evaluate population
    pop = population.from_init(25, initializer.random(DIM))
    evaluator.pop_eval_from_fitness(eval)(pop,pop)

    sel = select_one.det_tournament(4)

    print("best of pop(25): ",pop.best())

    #assemble simple GA
    sga = algo.simpleGA(
        select_one.det_tournament(4),
        operator.OnePtBitCrossover(),.1,
        hc,.7,
        eval,
        continuator.max_generations(50)
    )
    #run algo on pop and print best individual
    sga(pop)
    print("after hybrid GA-HC: ",pop.best())
