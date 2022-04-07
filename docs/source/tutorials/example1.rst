Hello World : simpleGA for OneMax
=================================

The following example shows how to build a simple genetic algorithm (SGA) for the OneMax optimization problem.

.. code-block:: python

    from pyparadiseo import Pop
    from pyparadiseo.evaluator import FitnessEval,PopLoopEval
    # for OneMax problem (binary encoding)
    from pyparadiseo.initializer import BinaryInit
    from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
    # problem-independent algorithm components
    from pyparadiseo.eo import algo,selector,continuator

    # make pyparadiseo evaluator from python function
    count_ones = FitnessEval(lambda sol: sum(sol))
    # generate population of size 25 and problem size 20
    pop = Pop(25, BinaryInit(20))
    # evaluate population with eval
    PopLoopEval(count_ones)(pop,pop)

    # assemble simple GA
    sga = algo.SGA(
        selector.DetTournamentSelect(4),    # deterministic tournament with size 4
        OnePtBitCrossover(),.1,             # 1-Point crossover with probability 0.1
        DetBitFlip(),.7,                    # BitFlip mutation with probability 0.7
        count_ones,                         # the evaluator
        continuator.GenContinue(50)         # continue for 50 generations
    )
    # run algo on pop
    sga(pop)
    # and print best individual
    print(pop.best())
