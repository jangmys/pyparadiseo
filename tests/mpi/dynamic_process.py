# import multiprocessing
# from mpi4py.futures import MPIPoolExecutor
from mpi4py import MPI
from mpi4py.futures import MPIPoolExecutor
from mpi4py.futures import MPICommExecutor




from pyparadiseo import population,initializer,evaluator,solution

import numpy as np
import time
import ext_eval

def sleep_and_hello(sec,arg):
    time.sleep(sec)
    print("hello ",arg)
    return arg+1

def func(x):
    # time.sleep(1)
    return np.sum(x)

# from pyparadiseo._core import eoPopEvalFuncBin

# class popevaal():
#     global _fitness_eval
#
#     def __init__(self,fitness_eval,max_workers):
#         self._fitness_eval = fitness_eval
#         self.executor = MPIPoolExecutor(max_workers=max_workers)
#
#     def __call__(self,pop,pop2):
#         future = list()
#         for ind in pop:
#             future.append(self.executor.submit(self._fitness_eval, ind))
#
#         for i,f in enumerate(future):
#             pop[i].fitness = f.result()
#
#
#
# class popeval(eoPopEvalFuncBin):
#     global _fitness_eval
#
#     def __init__(self,fitness_eval,max_workers):
#         eoPopEvalFuncBin.__init__(self)
#         self._fitness_eval = fitness_eval
#         self.executor = MPIPoolExecutor(max_workers=max_workers)
#
#     def __call__(self,pop,pop2):
#         future = list()
#         for ind in pop:
#             future.append(self.executor.submit(self._fitness_eval, ind))
#
#         for i,f in enumerate(future):
#             pop[i].fitness = f.result()
#
#
# class popeval_sync(eoPopEvalFuncBin):
#     global _fitness_eval
#
#     def __init__(self,fitness_eval,max_workers):
#         eoPopEvalFuncBin.__init__(self)
#         self._fitness_eval = fitness_eval
#         self.executor = MPIPoolExecutor(max_workers=max_workers)
#
#     def __call__(self,pop,pop2):
#         for i,result in enumerate(self.executor.map(self._fitness_eval, pop)):
#             pop[i].fitness = result
#
#

eval = evaluator.fitness(func,stype='bin')

import sys

if __name__ == '__main__':
    sub_comm = MPI.COMM_SELF.Spawn(sys.executable,args=['wrkr.py'],maxprocs=5)

    val=42
    sub_comm.bcast(val, MPI.ROOT)

    common_comm=sub_comm.Merge(False)
    print('parent in common_comm ', common_comm.Get_rank(), ' of',common_comm.Get_size())

    #=================================================
    # N = np.array(100, 'i')
    # sub_comm.Bcast([N, MPI.INT], root=MPI.ROOT)
    # PI = np.array(0.0, 'd')
    # sub_comm.Reduce(None, [PI, MPI.DOUBLE],
    #             op=MPI.SUM, root=MPI.ROOT)
    # print(PI)
    #
    # #=================================================
    POP_SIZE = 12
    N_VAR =10
    #
    size = common_comm.Get_size()
    rank = common_comm.Get_rank()
    #
    # print("size ",size)
    pop_dvec = np.zeros((POP_SIZE,N_VAR))

    for i in range(size):
        pop_dvec[i][0]=i


    def split(a, n):
        k, m = divmod(len(a), n)
        return list(a[i * k + min(i, m):(i + 1) * k + min(i + 1, m)] for i in range(n))

    pop_dvec = split(pop_dvec,size)

    #

    nb_sim_per_proc = [len(pop_dvec[i]) for i in range(size)]#np.zeros((size+1,N_VAR))
    #
    nb_sim=common_comm.scatter(nb_sim_per_proc, root=0)


    inds=common_comm.scatter(pop_dvec, root=0)

    print("thiz",nb_sim,inds)


    pop = [solution.random(10,stype='bin') for _ in range(POP_SIZE)]

    pop = split(pop,size)

    recvpop = common_comm.scatter(pop,root=0)

    print("ROOT POP:",recvpop)

    fit = list()

    for ind in recvpop:
        fit.append(ext_eval.fitness(ind))

    print("0",fit)

    fit = common_comm.gather(fit,root=0)

    print("#"*20)
    print(pop)
    print(fit)


    # sub_comm.scatter(nb_sim_per_proc, root=MPI.ROOT)
    # sub_comm.scatter([nb_sim_per_proc, MPI.INT], root=MPI.ROOT)


    #
    # for i in range(1,nprocs): # sending to workers
    #     comm.send(nb_sim_per_proc[i], dest=i, tag=10)
    #     comm.send(pop_dvec[np.sum(nb_sim_per_proc[:i]):np.sum(nb_sim_per_proc[:i+1])], dest=i, tag=11)


    sub_comm.Disconnect()
