# PyParadisEO

A Python Framework for Metaheuristics

## Installation instructions

The following has been tested on
- Ubuntu 18.04.2 LTS
- cmake 3.21.3
- boost version 1.65.1
- python 3.6.13
- scikit-build==0.12.0
- numpy==1.19.5
- pytest==6.2.5

We're aware that there might be installation issues on different systems. In future versions this will hopefully become much easier.

1. clone this repository. For example :

`git clone git@gitlab.inria.fr:paradiseo/pyparadiseo.git`


2. In the `pyparadiseo` root directory (i.e. where setup.py is located) :

`python -m pip install .`

Note: this may create a directory `_skbuild`. If the installation fails, this directory should be deleted before you re-attempt to install pyparadiseo.


3. Now run the tests

`python -m pytest tests`

## Quick start : GA - Max One

Check out the example in `examples/` : a simple GA for the One-Max pseudo-problem

More documentation, examples and tutorials coming soon!
