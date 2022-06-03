import pyparadiseo as pp

# from pyparadiseo import RealParticle
from pyparadiseo import Solution,Pop

from pyparadiseo import evaluator,bounds
from pyparadiseo import operator,initializer
# from pyparadiseo import testSolution


import unittest


class myInit(initializer.eoInit):
    def __init__(self,n):
        initializer.eoInit.__init__(self)
        self.size = n
    def __call__(self,sol):
        sol = Solution(self.size)



if __name__ == '__main__':
    # sol = RealParticle()
    # sol = RealParticle(1)
    # sol = RealParticle(1,1.0)
    # sol = RealParticle(1,1.0,2.0)
    # sol = RealParticle(1,1.0,2.0,3.0)

    sol = Solution()
    # mutate = operator.UniformMutation(0.1)
    mutate = operator.UniformMutation(0.1)

    # print("Before :  ",sol)
    # mutate(sol)
    # print("After :   ",sol)

    init = initializer.RealBoundedInit(bounds.RealVectorBounds(10,3,4))

    init(sol)
    print("Init :   ",sol)

    mutate(sol)
    print("After :   ",sol)


    print("#"*20)
    popul = Pop(4,init)
    print(popul)

    for ind in popul:
        mutate(ind)
        print("Mutated :   ",ind)


    # sol[0] = 0
    # sol[1] = 1
    # sol[2] = 2

    # for i in sol:
    #     print(i)
    #
    # print(len(sol))
    #
    testSolution(sol)
    print("AddOne :  ", sol)

    #
    # for i in sol:
    #     print(i)
    #
    #
    # mutate(sol)
    # for i in sol:
    #     print(i)
    #
    import numpy as np

    def eval(sol):
        return np.sum(sol)
        #
        # sum = 0
        # for i in sol:
        #     sum += i
        # return sum
    #
    ev = evaluator.fitness(eval)
    #
    ev(sol)
    # # sol.fitness = eval(sol)
    print("Evaluate :  ",sol)

    from pyparadiseo import initializer,bounds

    init = initializer.RealBoundedInit(bounds.RealVectorBounds(10,-1,1))

    init(sol)
    print("ReINIT :  ",sol)


    # import numpy as np
    #
    # sol = Solution()
    # sol = np.zeros(5)
    #
    # print(sol)

    # print(sol[:])
        # mutate = operator.UniformMutation(0.8,0.2)
        # print(self.sol)
        # mutate(self.sol)
        # print(self.sol)
