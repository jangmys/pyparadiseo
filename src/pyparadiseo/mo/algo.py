#ABC
from ..core import moLocalSearch as LocalSearch

from ..core import moSimpleHC as SimpleHC
from ..core import moFirstImprHC as FirstImprHC
from ..core import moRandomBestHC as RandomBestHC
from ..core import moNeutralHC as NeutralHC

from ..core import moRandomSearch as RandomSearch
from ..core import moRandomWalk as RandomWalk
from ..core import moRandomNeutralWalk as RandomNeutralWalk

from ..core import moMetropolisHasting as MetropolisHasting
from ..core import moSA as SA
from ..core import moTS as TS


#SimpleHC(Neighborhood,solEval,nborEval)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator)
#SimpleHC(Neighborhood,solEval,nborEval,moContinuator,moNeighborComparator,moSolNeighborComparator)
