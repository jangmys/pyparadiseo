import os
import math
import numpy as np

class TravelingSalesman():
    def __init__(self, instance : str = None):
        self.ncities,self.xcoord,self.ycoord = self.read_input(instance)

        self.moves = []
        for i in range(1,self.ncities):
            for j in range(i+1,self.ncities):
                self.moves.append((i,j))

    def read_input(self,instance_name : str):
        """Read TSP instance from ./problemData/tsp/{instance_name}.tsp

        Parameters
        ----------
        instance_name : str
            name of the file to read, without .tsp extension

        Returns
        -------

        """
        if instance_name is None:
            raise FileNotFoundError('no file to read from')

        filename=os.path.dirname(__file__)+"/problemData/tsp/"+instance_name+".tsp"
        print("[] Reading instance data from ",filename)

        with open(filename, 'r') as f:
            content = f.read()
            lines = content.split('\n')

            ncities = int(lines[3].split()[-1])

            xcoord = np.zeros(ncities,dtype=float)
            ycoord = np.zeros(ncities,dtype=float)

            for i,line in enumerate(lines[6:]):
                if line.startswith('EOF'):
                    break
                else:
                    X, Y = line.split()[1:]
                    xcoord[i]=X
                    ycoord[i]=Y

        return ncities,xcoord,ycoord

    def get_edge_length(self, a, b):
        dx = self.xcoord[a] - self.xcoord[b]
        dy = self.ycoord[a] - self.ycoord[b]
        return math.sqrt(dx * dx + dy * dy)

    ### need this
    def eval(self,perm):
        cost = self.get_edge_length(0, perm[1])
        for i in range(1,len(perm)-1):
            cost += self.get_edge_length(perm[i], perm[i + 1])
        cost += self.get_edge_length(perm[len(perm) - 1], 0)
        return cost

    ### need that (not == Permutation because tour[0] = 0)
    def make_random_tour(self):
        tour = np.arange(self.ncities)
        np.random.shuffle(tour[1:])
        return tour

    def eval_incremental(self,sol,sol_fitness,index):
        a,b=self.moves[index]
        tourlen = sol_fitness
        tourlen -= self.get_edge_length(sol[a-1],sol[a])
        tourlen += self.get_edge_length(sol[a-1],sol[b])
        if b == self.ncities-1:
            tourlen -= self.get_edge_length(sol[b],sol[0])
            tourlen += self.get_edge_length(sol[b],sol[0])
        else:
            tourlen -= self.get_edge_length(sol[b],sol[b+1])
            tourlen += self.get_edge_length(sol[a],sol[b+1])

        # neigh.fitness = tourlen
        return tourlen

    def move(self,nbor,sol):
        """assuming i<j"""
        i,j=self.moves[nbor.index()]
        while i<j:
            sol.array[i],sol.array[j]=sol.array[j],sol.array[i]
            i += 1
            j -= 1
        return sol
