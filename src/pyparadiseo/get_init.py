# from ._core import eoInit
# from ._core import pyeoInit
#
# # import numpy as np
#
# def get_init(solution_encoding,len=None):
#     """
#     factory function for solution initializer
#     """
#     if solution_encoding == 'binary':
#         #expect len !
#         return pyeoInit(lambda : np.random.randint(0,2,len))
#     elif solution_encoding == 'integer':
#         #expect upper,lower
#         pass
#     elif solution_encoding == 'permutation':
#         #expect len !
#         pass
#     else:
#         pass
#
#
#
#
#
# def rand_bits(len_):
#     x=np.random.randint(0,2,len_)
#     return x
#
# class BinarySolutionGenerator(eoInit):
#     def __init__(self,len_):
#         eoInit.__init__(self)
#         self._len = len_
#     def __call__(self,eo):
#         eo.encoding = rand_bits(self._len)
#         # eo.encoding = np.random.randint(0,2,self._len)
#         eo.invalidate()
#
# def get_binary_solution_generator(len_):
#     return pyeoInit(lambda : rand_bits(len_))
#     # return pyeoInit(lambda : np.random.randint(0,2,len_))
#
# #############################################
#
#
# #############################################
# def rand_ints(lb,ub):
#     x=np.zeros(len(lb))
#     for i,_ in enumerate(x):
#         x[i]=np.random.randint(lb[i],ub[i])
#     return x
#
#
# class IntegerSolutionGenerator(eoInit):
#     def __init__(self,len_,lb,ub):
#         eoInit.__init__(self)
#         self._len = len_
#         self.lb = lb
#         self.ub = ub
#     def __call__(self,eo):
#         # eo.encoding = np.random.randint(self.lb,self.ub,len(self.lb))
#         eo.encoding = rand_ints(self.lb,self.ub)
#         eo.invalidate()
#
# def get_integer_solution_generator(len_,lb,ub):
#     #assert lb < ub ...
#     return pyeoInit(lambda : rand_ints(lb,ub))
#
# ##########################################
#
#
#
#
#
# ##########################################
#
#
# class CustomSolutionClass():
#     """
#     will be set as `encoding` of the pyEOT
#
#     should specify __getitem__ and __setitem__
#     """
#     def __init__(self,len,lb,ub):
#         self.x = rand_ints(lb,ub)
#     def __str__(self):
#         return str(self.x)
#     def __getitem__(self,key):
#         return self.x[key]
#     def __setitem__(self,key,value):
#         self.x[key]=value
