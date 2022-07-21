from pyparadiseo import config

from pyparadiseo import population

import pyparadiseo as pp

from pyparadiseo import eo
from pyparadiseo import initializer,evaluator,operator
from pyparadiseo.eo import selector,select_one,replacement,continuator,algo
from pyparadiseo import rng

from pyparadiseo import initializer,evaluator

import time
import copy
import numpy as np
import numba as nb

@nb.njit
def sum_bits(sol):
    return np.sum(sol)

if __name__ == "__main__":
    config.set_solution_type('bin')

    POP_SIZE = 10
    DIM = 500

    myinit = initializer.random(DIM) #Init(lambda : np.random.choice([True,False],DIM))
    myeval = evaluator.fitness(sum_bits)

    #crossovers 5
    crossovers = [pp.operator.OnePtBitCrossover(),pp.operator.UBitCrossover(0.5)]
    for i in range(1,11,4):
        crossovers.append(pp.operator.NPtsBitCrossover(i))

    #mutations 4
    mutations = [pp.operator.BitMutation(0.01)]
    for i in range(1,11,4):
        mutations.append(pp.operator.DetBitFlip(i))

    #selectors 4 (+2)
    #add StochTournamentSelect
    #add ProportionalSelect
    selectors = [select_one.random(),select_one.sequential(),select_one.stoch_tournament(0.5)]
    for i in range(2,11,4):
        selectors.append(select_one.det_tournament(i))

    #replacement
    replacements = [replacement.comma(),replacement.plus(),replacement.ssga_worse(),replacement.ssga_stoch_tournament(0.51)]
    for i in range(2,10,4):
        replacements.append(replacement.ssga_det_tournament(i))


    continuators = [continuator.steady_fitness(10,10),continuator.steady_fitness(10,20)]
    # continuators = [continuator.GenContinue(100)]

    xover_rates = [r*0.1 for r in range(10)]
    mut_rate = [r*0.1 for r in range(10)]

    pop_eval = pp.evaluator.pop_eval_from_fitness(myeval)

    i=0

    t1 = time.time()

    print("selectors:\t",len(selectors))
    print("crossovers:\t",len(crossovers))
    print("mutations:\t",len(mutations))
    print("replacement:\t",len(replacements))
    print("continue:\t",len(continuators))

    print("algo instances:\t",len(selectors)*len(selectors)*len(selectors)*len(crossovers)*len(mutations)*len(replacements)*len(continuators))


    bestGA = None
    bestFit = 0

    for crossselect in selectors: # 6
        for xover in crossovers: # 5
            for aftercrossselect in selectors: # 6
                for mutselect in selectors: # 6
                    for mut in mutations: # 4
                        for repl in replacements: # 6
                            for cont_ind in range(1,2): # 2
                                rng().reseed(42)

                                pop = population.from_init(10,myinit)
                                for ind in pop:
                                    myeval(ind)

                                i = i+1

                                cnt = continuator.max_generations(10)
                                # cnt = continuator.SteadyFitContinue(10,cont_ind*10)
                                # print(crossselect,xover,aftercrossselect,mutselect,mut,repl,cnt)

                                ga = eo.algo.fastGA(
                                    0.5,
                                    crossselect,
                                    xover,
                                    aftercrossselect,
                                    0.5,
                                    mutselect,
                                    mut,
                                    pop_eval,
                                    repl,
                                    cnt
                                )

                                tstart = time.time()
                                ga(pop)

                                print(i,pop.best().fitness,time.time()-tstart)

                                if pop.best().fitness > bestFit:
                                    bestFit = pop.best().fitness

                                    bestGA = eo.algo.fastGA(
                                        0.5,
                                        crossselect,
                                        xover,
                                        aftercrossselect,
                                        0.5,
                                        mutselect,
                                        mut,
                                        pop_eval,
                                        repl,
                                        cnt,
                                        0
                                    )
                                break

    print(time.time()-t1)

    print(bestFit)
