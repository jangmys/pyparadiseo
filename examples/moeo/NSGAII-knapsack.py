import sys
sys.path.append('..')

from pyparadiseo import config

from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import operator
from pyparadiseo import population

from pyparadiseo.eo import continuator
from pyparadiseo.moeo import algo




from problems import knapsack

#basic moeoNSGA
#[problem+xover+mutation]
def nsgaII_test_1():
    #2 objectives minimize first (weight)/maximize second (value)
    config.setup_objectives(2,[True,False])

    #wraps a solution generator (in problems.knapsack) into an eoInit
    myInit = initializer.make_initializer(knapsack.make_solutionGenerator(20))
    #an eval fct (compatible with solution generator!) wrapped into an eoEvalFunc
    myEval = evaluator.objectives(knapsack.evalKnapsack)
    #a crossover (eoQuadOp)
    myXover = operator.make_crossover(knapsack.cxSet)
    #a mutation (eoMonOp)
    myMutate = operator.make_mutation(knapsack.mutSet)

    alg = algo.moeoNSGAII(100,myEval,myXover,0.1,myMutate,0.5)

    pop = population.from_init(50,myInit)

    # #run algorithm
    alg(pop)
    # plot evolved pop
    knapsack.do_plot(pop)


# #using custom continuator...
def nsgaII_test_2():
    #2 objectives minimize first (weight)/maximize second (value)
    config.setup_objectives(2,[True,False])

    #wraps a solution generator (in problems.knapsack) into an eoInit
    myInit = initializer.make_initializer(knapsack.make_solutionGenerator(20))
    #an eval fct (compatible with solution generator!) wrapped into an eoEvalFunc
    myEval = evaluator.objectives(knapsack.evalKnapsack)
    myEvalCount = evaluator.EvalFuncCounter(myEval,"What's this string for?")#cannot nest directly...?
    myContinue=continuator.eval_calls(myEvalCount,5000)
    #a crossover (eoQuadOp)
    myXover = operator.make_crossover(knapsack.cxSet)
    #a mutation (eoMonOp)
    myMutate = operator.make_mutation(knapsack.mutSet)

    myGenOp = operator.SGAGenOp(myXover,0.1,myMutate,0.5)

    # alg = algo.moeoNSGAII(100,myEval,myXover,0.1,myMutate,0.5)
    alg = algo.moeoNSGAII(myContinue,myEvalCount,myGenOp)

    pop = population.from_init(50,myInit)

    # #run algorithm
    alg(pop)
    # plot evolved pop
    knapsack.do_plot(pop)
#
#
# #using a population evaluator...
# def nsgaII_test_3():
#     #wraps a solution generator (in problems.knapsack) into an eoInit
#     myInit = eo_wrap.setInit(knapsack.make_solutionGenerator(20))
#     #2 objectives minimize first (weight)/maximize second (value)
#     moeoObjectiveVectorTraits.setup(2,[True,False])
#
#     #an eval fct (compatible with solution generator!) wrapped into an eoEvalFunc
#     myEval = eo_wrap.setEvalFunc(knapsack.evalKnapsack)
#     myPopEval = eoPopLoopEval(myEval)
#
#     # myEvalCount = eoEvalFuncCounter(eo_wrap.setEvalFunc(knapsack.evalKnapsack),"What's this string for?")
#     # myContinue = eoEvalContinue(myEvalCount,1000)
#     myContinue = eoGenContinue(100)
#
#     #a crossover (eoQuadOp)
#     myXover = eo_wrap.setCrossover(knapsack.cxSet)
#     #a mutation (eoMonOp)
#     myMutate = eo_wrap.setMutate(knapsack.mutSet)
#     myGenOp = eoSGAGenOp(myXover,0.5,myMutate,0.5)
#
#     # alg = moeoNSGA(500,myEval,myXover,0.1,myMutate,0.5,0.5)
#     alg = moeoNSGAII(myContinue,myPopEval,myGenOp)
#
#     pop = moeoPop(50,myInit)
#
#     # #run algorithm
#     alg(pop)
#     # #plot evolved pop
#     knapsack.do_plot(pop)
#
# #using a eoTransform and standard Eval...
# def nsgaII_test_4():
#     #wraps a solution generator (in problems.knapsack) into an eoInit
#     myInit = eo_wrap.setInit(knapsack.make_solutionGenerator(20))
#     #2 objectives minimize first (weight)/maximize second (value)
#     moeoObjectiveVectorTraits.setup(2,[True,False])
#
#     #an eval fct (compatible with solution generator!) wrapped into an eoEvalFunc
#     myEval = eo_wrap.setEvalFunc(knapsack.evalKnapsack)
#     myContinue = eoGenContinue(100)
#
#     myXover = eo_wrap.setCrossover(knapsack.cxSet)
#     myMutate = eo_wrap.setMutate(knapsack.mutSet)
#     myTransform = eoSGATransform(myXover,0.5,myMutate,0.5)
#
#     alg = moeoNSGAII(myContinue,myEval,myTransform)
#
#     pop = moeoPop(50,myInit)
#
#     # #run algorithm
#     alg(pop)
#     # #plot evolved pop
#     knapsack.do_plot(pop)
#
# #using a eoTransform and population evaluator...
# def nsgaII_test_5():
#     #wraps a solution generator (in problems.knapsack) into an eoInit
#     myInit = eo_wrap.setInit(knapsack.make_solutionGenerator(20))
#     #2 objectives minimize first (weight)/maximize second (value)
#     moeoObjectiveVectorTraits.setup(2,[True,False])
#
#     #an eval fct (compatible with solution generator!) wrapped into an eoEvalFunc
#     myEval = eo_wrap.setEvalFunc(knapsack.evalKnapsack)
#     myPopEval = eoPopLoopEval(myEval)
#     myContinue = eoGenContinue(100)
#
#     myXover = eo_wrap.setCrossover(knapsack.cxSet)
#     myMutate = eo_wrap.setMutate(knapsack.mutSet)
#     myTransform = eoSGATransform(myXover,0.5,myMutate,0.5)
#
#     alg = moeoNSGAII(myContinue,myPopEval,myTransform)
#
#     pop = moeoPop(50,myInit)
#
#     # #run algorithm
#     alg(pop)
#     # #plot evolved pop
#     knapsack.do_plot(pop)

if __name__ == '__main__':
    nsgaII_test_1()
    nsgaII_test_2()
    # nsgaII_test_3()
    # nsgaII_test_4()
    # nsgaII_test_5()
