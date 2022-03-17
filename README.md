[![pipeline status](https://gitlab.inria.fr/paradiseo/pyparadiseo/badges/master/pipeline.svg)](https://gitlab.inria.fr/paradiseo/pyparadiseo/-/commits/master)

### Table of Contents
- [Installation](#installation)
- [Usage](#getting-started)
- [Components](#components)



## Installation

For now, PyParadiseo is only available for Python 3.6.
You can install PyParadiseo with `pip`

```console
pip install pyparadiseo
```

## Getting Started

Example of running EO's simple GA (SGA) for the One-Max test problem

```python
# problem dependent
from pyparadiseo import Pop
from pyparadiseo.evaluator import FitnessEval,PopLoopEval
# encoding dependent
from pyparadiseo.initializer import BinaryInit
from pyparadiseo.operator import OnePtBitCrossover,DetBitFlip
# independent
from pyparadiseo.eo import algo,selector,continuator

import numpy as np

if __name__ == "__main__":
    #make pyparadiseo evaluator from python function
    eval = FitnessEval(lambda sol: np.count_nonzero(sol))
    #generate and evaluate population
    pop = Pop(25, BinaryInit(20))
    PopLoopEval(eval)(pop,pop)
    #assemble simple GA
    sga = algo.SGA(
        selector.DetTournamentSelect(),
        OnePtBitCrossover(),.2,
        DetBitFlip(),.6,
        eval,
        continuator.GenContinue(100)
    )
    #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
```

## Components
- **EO** (Population-based single-objective)
- **MO** (Trajectory-based single-objective)
- **MOEO** (Multi-objective)
- **Encodings** : Binary, Integer, Real, Permutation, Custom
- **Genetic Operators** : ...
