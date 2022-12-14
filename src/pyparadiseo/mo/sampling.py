from pyparadiseo import config
from pyparadiseo import initializer
from pyparadiseo import solution

from pyparadiseo import mo
# from pyparadiseo.initializer import eoInit
# from pyparadiseo.mo.algo import moLocalSearch

class moSampling():
    def __init__(self,_init,_ls,stype='gen'):
        self.init = _init
        self.ls = _ls
        self.stype = stype
        self.cont = _ls.get_continuator()
        self.check = mo.continuator.checkpoint(self.cont)

    def add(self,stat):
        self.check.add(stat)

    def __call__(self):
        sol = solution.empty(stype=self.stype)

        self.ls.set_continuator(self.check)
        self.init(sol)
        self.ls(sol)
        self.ls.set_continuator(self.cont)
