Operators (:mod:`pyparadiseo.operator`)
=======================================

.. currentmodule:: pyparadiseo

.. autosummary::
    :toctree: _autosummary
    :template: module.rst

    operator

Mutation (aka **eoMonOp<EOType>**)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mutation operators are ``eoMonOp`` functors with the following signature:

.. code-block::

    bool operator()(EOType&)

Mutation operators modify one solution object and return ``True`` if the solution has changed.


.. toctree::
    :maxdepth: 2

    operators/mutation


Crossover (aka **eoQuadOp<EOType>**)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mutation operators are ``eoQuadOp`` functors with the following signature:

.. code-block::

    bool operator()(EOType&,EOType&)

Crossover operators modify two solution objects and return ``True`` if one of the solutions has changed.


.. toctree::
    :maxdepth: 2

    operators/crossover

-------------------

Generic
\\\\\\\\
For convenience, pyparadiseo provides wrappers that allow to use Python callables that modify solution encodings as mutation or crossover operators.

When implementing variation operators it is the programmers responsibility to make sure that the operators are compatible with the solution definition.


.. autofunction:: operator.make_mutation

.. autofunction:: operator.make_crossover
