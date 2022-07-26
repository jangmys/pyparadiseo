Operators (:mod:`pyparadiseo.operator`)
=======================================

.. currentmodule:: pyparadiseo.operator

Mutation (aka **eoMonOp<EOType>**)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mutation operators are ``eoMonOp`` functors with the following signature:

.. code-block::

    bool operator()(EOType&)

Mutation operators modify one solution object and return ``True`` if the solution has changed.


Crossover (aka **eoQuadOp<EOType>**)
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
Mutation operators are ``eoQuadOp`` functors with the following signature:

.. code-block::

    bool operator()(EOType&,EOType&)

Crossover operators modify two solution objects and return ``True`` if one of the solutions has changed.

-------------------

Generic
\\\\\\\\
For convenience, pyparadiseo provides wrappers that allow to use Python callables that modify solution encodings as mutation or crossover operators.

When implementing variation operators it is the programmers responsibility to make sure that the operators are compatible with the solution definition.


.. autofunction:: pyparadiseo.operator.make_mutation

.. autofunction:: pyparadiseo.operator.make_crossover


Generic
\\\\\\\\
.. autosummary::
    :toctree: _autosummary

    pyparadiseo.operator.pyMonOp
    pyparadiseo.operator.pyBinOp
    pyparadiseo.operator.pyQuadOp

.. .. autoclass:: pyparadiseo.operator.pyMonOp
..     :members:
..     :undoc-members:
..     :show-inheritance:


Binary
\\\\\\\\
* Mutation

.. autosummary::
    :toctree: _autosummary

    bit_flip
    bit_flip_n
    bit_inversion
    bit_next
    bit_previous

* Crossover

.. autosummary::
    :toctree: _autosummary

    one_point_bit_cx
    n_point_bit_cx
    uniform_bit_cx

.. NPtsBitCrossover
.. BitGxOver


Real
\\\\\\\\
Mutation
---------
.. autosummary::
    :toctree: _autosummary

    uniform_real_mutation

.. DetUniformMutation

Crossover
------------
.. autosummary::
    :toctree: _autosummary

    segment_cx
    hypercube_cx
    sbx
    uniform_real_cx




.. .. automodule:: pyparadiseo.operator
..    :members:
..    :undoc-members:
..    :show-inheritance:
..    :imported-members:
