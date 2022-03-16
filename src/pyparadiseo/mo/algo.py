#ABC
from .._core import moLocalSearch as LocalSearch

from .._core import moSimpleHC as SimpleHC
from .._core import moFirstImprHC as FirstImprHC
from .._core import moRandomBestHC as RandomBestHC
from .._core import moNeutralHC as NeutralHC

from .._core import moRandomSearch as RandomSearch
from .._core import moRandomWalk as RandomWalk
from .._core import moRandomNeutralWalk as RandomNeutralWalk

from .._core import moMetropolisHasting as MetropolisHasting
from .._core import moSA as SA
from .._core import moTS as TS


#SimpleHC(Neighborhood,solEval,nborEval)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator,moNeighborComparator,moSolNeighborComparator)
