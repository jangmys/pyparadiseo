Config (:mod:`pyparadiseo.config`)
==============================================

.. currentmodule:: pyparadiseo.config

Global variables
----------------
Set global pyparadiseo variables.

* Solution type
    - {'gen','bin','real','perm'}. default='gen'
* Maximization / minimization
    - Maximization is default
* Number of objectives / optimization directions
    - single-objective is default

.. autosummary::
    :toctree: _autosummary

    set_solution_type
    set_maximize_fitness
    set_minimize_fitness
    is_minimizing
    setup_objectives
