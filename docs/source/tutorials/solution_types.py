Solution Types
==============
Almost all PyParadisEO functions accept a keyword argument `stype` which can be one of the following
- 'gen' : generic solution type (default)
- 'bin' : binary (0/1) vector solution
- 'real': real vector solution
- 'perm': permutation solution

For example
"""Python
from pyparadiseo import solution

sol = solution.empty(stype='bin')
"""
creates an empty solution of type `BinarySolution`.

To avoid typing this argument in subsequent PyParadisEO calls, one can set the solution type globally

"""Python
from pyparadiseo import config

config.set_solution_type('bin')
"""

Create solutions
================

Manipulate Solutions
====================
