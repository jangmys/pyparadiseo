#unconstrained binary quadratic programming
import os
import numpy as np

#
class UBQP():
    def __init__(self,instance_name : str = None):
        if instance_name is None:
            instance_name="bqp50"

        self.nb_instances,self.nb_vars,self.Qs = self.read_orlib_instance(instance_name)
        self.n_var = self.nb_vars[0]
        self.Q = self.Qs[0]


    def read_orlib_instance(self,instance_name):
        filename=os.path.dirname(__file__)+"/problemData/ubqp/"+instance_name+".txt"

        with open(filename,'r') as f:
            content = f.read()
            lines = content.split('\n')

            print(lines[0])

            n_inst = int(lines[0])
            n_vars = []
            n_nzs = []

            Qs = []

            cnt=0
            for k,line in enumerate(lines[1:]):
                if len(line.split())==2:
                    nvar = int(line.split()[0])
                    nnz = int(line.split()[1])

                    Qs.append(np.zeros([nvar,nvar]))
                    n_vars.append(nvar)
                    n_nzs.append(nnz)
                elif len(line.split())==3:
                    print(line)

                    i = int(line.split()[0])
                    j = int(line.split()[1])
                    v = int(line.split()[2])

                    Qs[-1][i-1,j-1] = v
                else:
                    break

        return n_inst,n_vars,np.array(Qs)


    def full_eval(self,sol):
        fit = 0
        for i,e in sol:
            if e:
                for j in sol[:i]:
                    if j:
                        fit += self.Q[i][j]
        return fit


    def simple_incr_eval(sol,nbor):
        i=nbor.index
        d=self.Q[i][i]

        for j in range(i):
            if sol[j]:
                d+=self.Q[i,j]

        for j in range(i+1,self.n_var):
            if sol[j]:
                d+=self.Q[j,i]

        if not sol[i]:
            nbor.fitness = sol.fitness + d
        else:
            nbor.fitness = sol.fitness - d


    def double_incr_eval(sol):
        pass
