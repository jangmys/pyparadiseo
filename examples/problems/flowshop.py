import os
import numpy as np
# import numba

class fsp:
    def __init__(self,instance_name):
        self.read_input(instance_name)
        self.head = np.zeros(shape=(self.nbMach,self.nbJob))
        self.tail = np.zeros(shape=(self.nbMach,self.nbJob))
        self.inser = np.zeros(shape=(self.nbMach,self.nbJob))
        self.sumPT = np.array([np.sum(self.PTM[:,i]) for i in range(self.nbJob)])

    # read from input file
    def read_input(self,instance_name):
        """Read PFSP instance from ./problemData/fsp/{instance_name}.dat

        Parameters
        ----------
        instance_name : str
            name of the file to read, without .dat extension

        Returns
        -------
            PTM : 2D numpy array (dtype=int)
                matrix of processing times
            nbJob : int
                number of jobs
            nbMach : int
                number of machines
        """

        filename=os.path.dirname(__file__)+"/problemData/fsp/"+instance_name+".dat"
        print("[] Reading instance data from ",filename)
        with open(filename, 'r') as f:
            content = f.read()
            lines = content.split('\n')

            self.nbJob = int(lines[0].split()[0])
            self.nbMach = int(lines[0].split()[1])

            self.PTM = np.zeros([self.nbMach,self.nbJob],dtype=int)

            i = 0
            for l in lines[1:]:
                if not l.strip():
                    continue
                if l.startswith('EOF'):
                    break
                else:
                    self.PTM[i] = [int(x) for x in l.split()]
                    i += 1

        return self.PTM,self.nbJob,self.nbMach

    def make_randomSolution(self):
        return np.random.permutation(self.nbJob)

    def eval(self,perm):
        tmp = np.zeros(self.nbMach,dtype=np.int64)
        for i in range(self.nbJob):
            jb = perm[i]
            tmp[0] += self.PTM[0,jb]
            for j in range(1,self.nbMach):
                tmp[j] = max(tmp[j],tmp[j-1]) + self.PTM[j,jb]
        return tmp[self.nbMach-1]


    def computeHeads(self,perm):
        '''heads : termination dates of all operations in perm[0,1,...,lim1-1]'''

        # print(perm," === ",len(perm))
        self.head[0,0] = self.PTM[0,perm[0]]
        for i in range(1,len(perm)):
            self.head[0,i]=self.head[0,i-1]+self.PTM[0,perm[i]]

        for i in range(1,self.nbMach):
            self.head[i,0]=self.head[i-1,0]+self.PTM[i,perm[0]]

        for j in range(1,self.nbMach):
            for i in range(1,len(perm)):
                self.head[j,i]=max(self.head[j-1,i],self.head[j,i-1])+self.PTM[j,perm[i]]

        # print("[]",perm)
        # print(self.head)
        # print("head[]",self.head[-1,len(perm)-1])




    def computeTails(self,perm,lim1):
        '''tails : reverse scheduling of perm[0,1,...,lim1-1]'''
        self.tail[-1,lim1-1] = self.PTM[self.nbMach-1,perm[lim1-1]]
        #last machine
        for i in range(lim1-2,-1,-1):
            self.tail[-1,i] = self.tail[-1,i+1] + self.PTM[-1,perm[i]]
        #last job
        for i in range(self.nbMach-2,-1,-1):
            self.tail[i,lim1-1] = self.tail[i+1,lim1-1]+self.PTM[i,perm[lim1-1]]

        for i in range(lim1-2,-1,-1):
            for j in range(self.nbMach-2,-1,-1):
                self.tail[j,i]=max(self.tail[j+1,i],self.tail[j,i+1])+self.PTM[j,perm[i]]


    def computeInsert(self,perm,len,job):
        '''termination dates of 'job' when inserted at all possible positions in perm'''
        self.inser[0,0] = self.PTM[0,job]
        for i in range(1,self.nbMach):
            self.inser[i,0]=self.inser[i-1,0]+self.PTM[i,job]
        for i in range(1,len):
            self.inser[0,i]=self.head[0,i-1]+self.PTM[0,job]
            for j in range(1,self.nbMach):
                self.inser[j,i]=max(self.inser[j-1,i],self.head[j,i-1])+self.PTM[j,job]


    def insertMakespans(self,perm,lim1,job):
        '''get makespans of job-insert moves'''
        self.computeHeads(perm)
        self.computeTails(perm, lim1)
        self.computeInsert(perm, lim1, job)

        makespans = np.zeros(lim1)
        for i in range(lim1):
            makespans[i] = max([ self.inser[j,i]+self.tail[j,i] for j in range(self.nbMach)])

        return makespans


    def removeMakespans(self,perm,lim1):
        self.computeHeads(perm,lim1)
        self.computeTails(perm,lim1)

        makespans = np.zeros(lim1+1)

        makespans[0] = max( [self.tail[j,1] for j in range(self.nbMach)] )
        for i in range(1,lim1-1):
            makespans[i] = max( [self.head[j,i-1] + self.tail[j,i+1] for j in range(self.nbMach)] )
        makespans[len-1] = max( [self.head[j,len-2] for j in range(self.nbMach)] )

        return makespans


    #insert in position which gives best cmax
    def bestInsert(self,perm,job):
        # makespans = np.zeros(len(perm)+1)
        makespans = self.insertMakespans(perm,len(perm),job)

        minpos = np.argmin(makespans)
        mini = makespans[minpos]

        perm = np.insert(perm,minpos,job)

        return perm,mini


    #//remove least well inserted job from perm
    #//return removed job in remjob
    #//return position of removed job
    def bestRemove(self,perm):
        # makespans = np.zeros(len(perm))
        makespans = self.removeMakespans(perm,len(perm))
