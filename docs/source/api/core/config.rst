Global (:mod:`pyparadiseo`)
==============================================

.. currentmodule:: pyparadiseo


Set global pyparadiseo variables.

* Solution type
    - {'gen','bin','real','perm'}. default='gen'
* Maximization / minimization
    - Maximization is default
* Number of objectives / optimization directions
    - single-objective is default

.. autosummary::
    :toctree: generated/

    config.set_solution_type
    config.set_maximize_fitness
    config.set_minimize_fitness
    config.is_minimizing
    config.setup_objectives


.. .. automodule:: pyparadiseo.config
..     :members:
..     :undoc-members:
..     :show-inheritance:
