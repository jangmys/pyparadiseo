[![pipeline status](https://gitlab.inria.fr/paradiseo/pyparadiseo/badges/master/pipeline.svg)](https://gitlab.inria.fr/paradiseo/pyparadiseo/-/commits/master)

For the documentation of PyParadiseo see [this](https://paradiseo.gitlabpages.inria.fr/pyparadiseo/).

This README only gives a short introduction.


### Table of Contents
- [Installation](#installation)
- [Usage](#getting-started)
- [Components](#components)

## Installation

The easiest way to get pyparadiseo is to install it via ``pip``. Currently the following Python versions are supported: 3.6, 3.7, 3.8, 3.9, 3.10

### Install with ``pip``

You can install pyParadiseo with `pip`


```bash
    pip install pyparadiseo
```

### Build from source

To build pyParadiseo, you'll need to have a few prerequisites installed on your system and set the corresponding paths in ``setup.py`` and ``CMakeLists.txt``

### Prerequisites

To compile the binary extension you need:
    - cmake >= 3.14
    - python3 >= 3.6
    - boost-python
    - boost-numpy

### Docker image
Here is a ``manylinux2014_x86_64`` `Docker image`_ with installed prerequisites

.. _Docker image: https://hub.docker.com/repository/docker/jangmys/manylinux2014_boost180_cp36_310


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
