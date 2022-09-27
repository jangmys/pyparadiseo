# import multiprocessing
# from mpi4py.futures import MPIPoolExecutor
from mpi4py import MPI
from mpi4py.futures import MPIPoolExecutor
from mpi4py.futures import MPICommExecutor


from pyparadiseo import population,initializer,evaluator,solution

import numpy as np
import time

def sleep_and_hello(sec,arg):
    time.sleep(sec)
    print("hello ",arg)
    return arg+1

def func(x):
    # time.sleep(1)
    return np.sum(x)

from pyparadiseo._core import eoPopEvalFuncBin

class popevaal():
    global _fitness_eval

    def __init__(self,fitness_eval,max_workers):
        self._fitness_eval = fitness_eval
        self.executor = MPIPoolExecutor(max_workers=max_workers)

    def __call__(self,pop,pop2):
        future = list()
        for ind in pop:
            future.append(self.executor.submit(self._fitness_eval, ind))

        for i,f in enumerate(future):
            pop[i].fitness = f.result()



class popeval(eoPopEvalFuncBin):
    global _fitness_eval

    def __init__(self,fitness_eval,max_workers):
        eoPopEvalFuncBin.__init__(self)
        self._fitness_eval = fitness_eval
        self.executor = MPIPoolExecutor(max_workers=max_workers)

    def __call__(self,pop,pop2):
        future = list()
        for ind in pop:
            future.append(self.executor.submit(self._fitness_eval, ind))

        for i,f in enumerate(future):
            pop[i].fitness = f.result()


class popeval_sync(eoPopEvalFuncBin):
    global _fitness_eval

    def __init__(self,fitness_eval,max_workers):
        eoPopEvalFuncBin.__init__(self)
        self._fitness_eval = fitness_eval
        self.executor = MPIPoolExecutor(max_workers=max_workers)

    def __call__(self,pop,pop2):
        for i,result in enumerate(self.executor.map(self._fitness_eval, pop)):
            pop[i].fitness = result



eval = evaluator.fitness(func,stype='bin')

if __name__ == '__main__':
    pop = population.from_list(
        [solution.random(5,stype='bin') for _ in range(10)],stype='bin'
        )

    for i in np.arange(1,9):
        peval = popeval(func,i)

        t1 = time.time()
        peval(pop,pop)
        print(time.time()-t1)

        peval = popeval_sync(func,i)

        t1 = time.time()
        peval(pop,pop)
        print("\t",time.time()-t1)

        t1 = time.time()
        peval = evaluator.pop_eval(popevaal(func,i),stype='bin')
        peval(pop,pop)
        print("\t",time.time()-t1)

    # print(pop)


    print('#'*20)
    executor = MPIPoolExecutor(max_workers=2)
    future = list()
    future.append(executor.submit(sleep_and_hello, 2, 22))
    future.append(executor.submit(sleep_and_hello, 3, 33))
    future.append(executor.submit(sleep_and_hello, 1, 11))
    future.append(executor.submit(func, pop[0]))
    future.append(executor.submit(eval, pop[0]))

    print("continue")
    print("receiving in order of submission...")

    for f in future:
        print(f.result())



    # from pyparadiseo._core import _SolutionVectorBinary

    # comm = MPI.COMM_WORLD
    # size = comm.Get_size()
    # rank = comm.Get_rank()
    #
    # sendbuf = None
    # if rank == 0:
    #     sendbuf = np.arange(100, dtype='i')        # sendbuf.T[:,:] = range(size)
    #     # print(sendbuf)
    #
    # recvbuf = np.empty(50, dtype='i')
    # comm.Scatter(sendbuf, recvbuf, root=0)
    # # print('Rank ',rank,'has',recvbuf)
    #
    # # -------------------------------------------------------
    # def split(a, n):
    #     k, m = divmod(len(a), n)
    #     print(k,m)
    #     return list(a[i * k + min(i, m):(i + 1) * k + min(i + 1, m)] for i in range(n))
    #
    #
    # listpop = None
    # if rank == 0:
    #     pop = [solution.random(10,stype='bin') for _ in range(10)]
    #     listpop = split(pop,size)
    #     print(listpop)
    #
    # recvpop = comm.scatter(listpop,root=0)
    #
    # for ind in recvpop:
    #     eval(ind)
    #
    # # recvpop = population.from_list(recvpop,stype='bin')
    # # print('Rank ',rank,'has',recvpop)
    #
    # listpop = comm.gather(recvpop,root=0)
    #
    # if rank == 0:
    #     print("RANK 0")
    #     pop = population.from_list(listpop[0],stype='bin')
    #     pop.merge(population.from_list(listpop[1],stype='bin'))
    #
    #     print(pop)

    # ===========================================================
