#!/usr/bin/env python
from mpi4py import MPI
import numpy as np

from pyparadiseo import solution

import ext_eval


comm = MPI.Comm.Get_parent()
size = comm.Get_size()
rank = comm.Get_rank()

print('hello',rank,' ',size)
print('ho ', comm.Get_rank(), ' of  ',comm.Get_size(),' ', MPI.COMM_WORLD.Get_rank(), ' of  ',MPI.COMM_WORLD.Get_size())

a = MPI.COMM_WORLD.bcast(MPI.COMM_WORLD.Get_rank(), root=0)
print("value from other child", a)

print("comm.Is_inter", comm.Is_inter())
b = comm.bcast(comm.Get_rank(), root=0)
print("value from parent", b)


common_comm=comm.Merge(True)
print("common_comm.Is_inter", common_comm.Is_inter())
print('common_comm ', common_comm.Get_rank(), ' of  ',common_comm.Get_size())

nb_sim_per_proc = None
nb_sim_per_proc = common_comm.scatter(comm.Get_rank(), root=0)


inds=common_comm.scatter(comm.Get_rank(), root=0)

print(nb_sim_per_proc,inds)

pop = common_comm.scatter(comm.Get_rank(),root=0)

print(pop)

fit=list()
for ind in pop:
    fit.append(ext_eval.fitness(ind))

print(common_comm.Get_rank(),fit)

common_comm.gather(fit,root=0)
# nb_sim_per_proc = comm.scatter(comm.Get_rank(), root=0)
# np.zeros((size,N_VAR))
#
# whatever =


# N = np.array(0, dtype='i')
# comm.Bcast([N, MPI.INT], root=0)
# h = 1.0 / N; s = 0.0
# for i in range(rank, N, size):
#     x = h * (i + 0.5)
#     s += 4.0 / (1.0 + x**2)
# PI = np.array(s * h, dtype='d')
# comm.Reduce([PI, MPI.DOUBLE], None,
#             op=MPI.SUM, root=0)
#
#
# nb_sim_per_proc = np.zeros((0,10))
#
# comm.Bcast([nb_sim_per_proc, MPI.INT], root=0)
#
# print(rank,"\t",nb_sim_per_proc)



# sendpop = None
# recvpop = [solution.zeros(3,stype='real') for _ in range(2)]

# nsim = comm.recv(source=0, tag=10)
# while nsim!=-1:
#     candidates = np.empty((nsim, p.n_dvar))
#     candidates = comm.recv(source=0, tag=11)
#     obj_vals = fitness(candidates)
#     comm.send([obj_vals], dest=0, tag=12)
#     nsim = comm.recv(source=0, tag=10)

# listpop=None
# recvpop = comm.scatter(listpop,root=0)
# # comm.Scatter(sendpop,recvpop,root=0)#MPI.ROOT)
# #
# print(recvpop)

# N = numpy.array(0, dtype='i')
# comm.Bcast([N, MPI.INT], root=0)
# h = 1.0 / N; s = 0.0
# for i in range(rank, N, size):
#     x = h * (i + 0.5)
#     s += 4.0 / (1.0 + x**2)
# PI = numpy.array(s * h, dtype='d')
# comm.Reduce([PI, MPI.DOUBLE], None,
#             op=MPI.SUM, root=0)

comm.Disconnect()
