# import multiprocessing
# from mpi4py.futures import MPIPoolExecutor
from mpi4py import MPI
from mpi4py.futures import MPIPoolExecutor
from mpi4py.futures import MPICommExecutor


from pyparadiseo import population,initializer,evaluator,solution

import numpy as np

if __name__ == '__main__':
    def func(x):
        return np.sum(x)

    eval = evaluator.fitness(func,stype='bin')

    # from pyparadiseo._core import _SolutionVectorBinary

    comm = MPI.COMM_WORLD
    size = comm.Get_size()
    rank = comm.Get_rank()

    sendbuf = None
    if rank == 0:
        sendbuf = np.arange(100, dtype='i')        # sendbuf.T[:,:] = range(size)
        # print(sendbuf)

    recvbuf = np.empty(50, dtype='i')
    comm.Scatter(sendbuf, recvbuf, root=0)
    # print('Rank ',rank,'has',recvbuf)

    # -------------------------------------------------------
    def split(a, n):
        k, m = divmod(len(a), n)
        print(k,m)
        return list(a[i * k + min(i, m):(i + 1) * k + min(i + 1, m)] for i in range(n))


    listpop = None
    if rank == 0:
        pop = [solution.random(10,stype='bin') for _ in range(10)]
        listpop = split(pop,size)
        print(listpop)

    recvpop = comm.scatter(listpop,root=0)

    for ind in recvpop:
        eval(ind)

    # recvpop = population.from_list(recvpop,stype='bin')
    # print('Rank ',rank,'has',recvpop)

    listpop = comm.gather(recvpop,root=0)

    if rank == 0:
        print("RANK 0")
        pop = population.from_list(listpop[0],stype='bin')
        pop.merge(population.from_list(listpop[1],stype='bin'))

        print(pop)

    # ===========================================================
