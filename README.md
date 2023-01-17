[![pipeline status](https://gitlab.inria.fr/paradiseo/pyparadiseo/badges/master/pipeline.svg)](https://gitlab.inria.fr/paradiseo/pyparadiseo/-/commits/master)

<font size="6"> For the documentation of PyParadiseo see [this](https://paradiseo.gitlabpages.inria.fr/pyparadiseo/). </font>

This README only gives a
<span style= "color : red"> very short introduction. </span>


### Table of Contents
- [Installation](#installation)
- [Usage](#getting-started)
- [Components](#components)

## Installation

The easiest way to get pyparadiseo is to install it via ``pip``. Currently the following Python versions are supported: 3.6, 3.7, 3.8, 3.9, 3.10

### 1) **Recommended** : install with ``pip``

You can install pyParadiseo with `pip`

```sh
    pip install pyparadiseo
```

### 2) Build from source

To build pyParadiseo, you'll need to have a few prerequisites installed on your system and set the corresponding paths in ``setup.py`` and ``CMakeLists.txt``

To compile the binary extension you need:

- cmake >= 3.14
- python3 >= 3.6
- boost-python
- boost-numpy

If you want to build pyparadiseo from source, the easiest should be to use this ``manylinux2014_x86_64`` [Docker image](https://hub.docker.com/repository/docker/jangmys/manylinux2014_boost180_cp36_310) with installed prerequisites.


## Getting Started

The documentation of PyParadiseo is available [here](https://paradiseo.gitlabpages.inria.fr/pyparadiseo/)

Example of running EO's simple GA (SGA) for the One-Max test problem

```python

from pyparadiseo import config

# problem dependent
from pyparadiseo import population
from pyparadiseo import initializer
from pyparadiseo import evaluator
from pyparadiseo import operator

from pyparadiseo.eo import algo,select_one,continuator

DIM=20
POP_SIZE=25
MAX_GEN=100

if __name__ == "__main__":
    #set global solution type 'bin'
    config.set_solution_type('bin')

    #make pyparadiseo evaluator from python function
    eval = evaluator.fitness(lambda sol: sum(sol))

    #generate and evaluate population
    pop=population.from_init(POP_SIZE,initializer.random(DIM))
    evaluator.pop_eval_from_fitness(eval)(pop,pop)

    #assemble simple GA
    sga = algo.simpleGA(
        select_one.det_tournament(4),
        operator.OnePtBitCrossover(),.1,
        operator.DetBitFlip(),.7,
        eval,
        continuator.max_generations(MAX_GEN)
    )
    # #run algo on pop and print best individual
    sga(pop)
    print(pop.best())
```
