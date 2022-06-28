import numpy as np

class OneMax():
    def __init__(self,_dim):
        self.dim = _dim
        pass

    def sum_bits(self,sol) -> float:
        return float(np.count_nonzero(sol))

    def eval_incremental(self,sol_enc,sol_fit,index) -> float:
        if sol_enc[index]:
            return sol_fit - 1
        else:
            return sol_fit + 1

    def move(self,nbor,sol):
        ind = nbor.index()
        if sol.array[ind]:
            sol.array[ind] = False
        else:
            sol.array[ind] = True


# def sum_bits(sol):
#     return np.count_nonzero(sol)

# def move(self,nbor,sol):
#     ind = nbor.index()
#     if sol[ind]:
#         sol[ind] = False
#     else:
#         sol[ind] = True
#
# def evalIncremental(sol_enc,sol_fit,index) -> float:
#     if sol_enc[index]:
#         return sol_fit - 1
#     else:
#         return sol_fit + 1
