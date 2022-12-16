from pyparadiseo import config,utils

from .._core import eoStatBase
from .._core import eoValueParam

class eo_average_stat(eoStatBase,eoValueParam):
    def __init__(self):
        eoStatBase.__init__(self)
        eoValueParam.__init__(self)
    def __call__(self,pop):
        sum = 0.0;
        for indy in pop:
            sum += indy.fitness

        sum /= len(pop)
        self.object = sum
