Getting started
===============
.. toctree::
   :maxdepth: 1
   :caption: First steps:


Overview
--------
How PyParadisEO exposes ParadisEO to Python
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
In ParadisEO, the main mechanisms for building algorithms adapted to a specific optimization problem are template specialization and inheritance.

Typically, in ParadisEO, you will

1. define you solution type - problem-specific encoding and attributes - as a child class of the `EO` class
2. specialize the `eoEvalFunc<T>` template for your solution and inherit from that abstract base class, defining the `operator(T& sol)` method
3. do the same for other components, adapting them to your needs, if necessary.
4. specialize some algorithm template for your solution-type
5. run

Basically, PyParadisEO provides factory functions allowing users to instantiate ParadisEO components and algorithms from Python.

Instead of specializing class templates, you specify the solution type as a parameter in the factory functions - the solution type also can be set globally.

Instead of inheriting from abstract functors, you pass Python callables with appropriate signatures to factory functions.

PyParadisEO provides the following solution types:

- generic
    - the encoding can be any Python object
- binary
- real
- permutation

Solution types
--------------

Operators
---------

Algorithms
----------
