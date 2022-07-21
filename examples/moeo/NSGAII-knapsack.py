import sys
sys.path.append('..')

from pyparadiseo import config

from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import operator
from pyparadiseo import population

from pyparadiseo.eo import transform
from pyparadiseo.eo import continuator
from pyparadiseo.moeo import algo

from pyparadiseo import decorators

from problems import knapsack


import time

#2 objectives minimize first (weight)/maximize second (value)
config.setup_objectives(2,[True,False])

#wraps a solution generator (in problems.knapsack) into an eoInit
myInit = decorators.initializer(knapsack.make_solutionGenerator(20))
myEval = decorators.objectives(knapsack.evalKnapsack)
myXover = decorators.crossover(knapsack.cxSet)
myMutate = decorators.mutation(knapsack.mutSet)


myEvalCount = decorators.counting_eval(myEval)#cannot nest directly...?
# myEvalCount = evaluator.EvalFuncCounter(myEval,"What's this string for?")#cannot nest directly...?

#basic moeoNSGA
#[problem+xover+mutation]
def nsgaII_test_1():
    alg = algo.nsgaII(myEval,(myXover,0.1,myMutate,0.5),100)
    pop = population.from_init(50,myInit)

    # #run algorithm
    t1=time.time()
    alg(pop)
    print(time.time()-t1)
    # plot evolved pop
    knapsack.do_plot(pop)


#using custom continuator...
def nsgaII_test_2():
    myContinue=continuator.eval_calls(myEvalCount,5000)
    myGenOp = operator.SGAGenOp(myXover,0.1,myMutate,0.5)
    alg = algo.nsgaII(myEvalCount,myGenOp,myContinue)

    pop = population.from_init(50,myInit)

    # #run algorithm
    t1=time.time()
    alg(pop)
    print(time.time()-t1)
    # plot evolved pop
    knapsack.do_plot(pop)


#using a population evaluator...
def nsgaII_test_3():
    myPopEval = evaluator.pop_eval_from_fitness(myEval)
    myGenOp = operator.SGAGenOp(myXover,0.1,myMutate,0.5)
    alg = algo.nsgaII(myPopEval,myGenOp,100)

    pop = population.from_init(50,myInit)
    # #run algorithm
    t1=time.time()
    alg(pop)
    print(time.time()-t1)
    # #plot evolved pop
    knapsack.do_plot(pop)


#using a eoTransform and standard Eval...
def nsgaII_test_4():
    myTransform = transform.SGA(myXover,0.5,myMutate,0.5)
    alg = algo.nsgaII(myEval,myTransform,100)

    pop = population.from_init(50,myInit)

    # #run algorithm
    t1=time.time()
    alg(pop)
    print(time.time()-t1)
    # #plot evolved pop
    knapsack.do_plot(pop)

#using a eoTransform and population evaluator...
def nsgaII_test_5():
    myPopEval = evaluator.pop_eval_from_fitness(myEval)
    myTransform = transform.SGA(myXover,0.5,myMutate,0.5)
    alg = algo.nsgaII(myPopEval,myTransform,100)

    pop = population.from_init(50,myInit)

    # #run algorithm
    t1=time.time()
    alg(pop)
    print(time.time()-t1)
    # #plot evolved pop
    knapsack.do_plot(pop)


if __name__ == '__main__':
    nsgaII_test_1()
    nsgaII_test_2()
    nsgaII_test_3()
    nsgaII_test_4()
    nsgaII_test_5()
