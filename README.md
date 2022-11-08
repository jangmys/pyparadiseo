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

### Build from source
Prerequisites

- scikit-build
- boost python

## Getting Started

The documentation of PyParadiseo is available [here](https://paradiseo.gitlabpages.inria.fr/pyparadiseo/)

Example of running EO's simple GA (SGA) for the One-Max test problem

```python

from pyparadiseo import Pop
from pyparadiseo.evaluator import FitnessEval,PopLoopEval

from pyparadiseo import evaluator
from pyparadiseo import operator
from pyparadiseo import population
from pyparadiseo import initializer

from pyparadiseo.eo import algo,select_one,continuator

import numpy as np

if __name__ == "__main__":
    #set solution type globally
    config.set_solution_type('bin')

    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: np.count_nonzero(sol))

    #generate and evaluate population
    init = initializer.random(size=20)
    pop = population.from_init(25, init)
    pop_eval=evaluator.pop_eval_from_fitness(eval)
    pop_eval(pop,pop)

    #assemble simple GA
    sga = algo.simpleGA(
        select_one.det_tournament(4),
        operator.OnePtBitCrossover(),.1,
        operator.DetBitFlip(),.7,
        eval,
        continuator.max_generations(self.NGENS)
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
