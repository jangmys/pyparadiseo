.. pyparadiseo documentation master file, created by
   sphinx-quickstart on Fri Jan  7 10:48:06 2022.
   You can adapt this file completely to your liking, but it should at least
   contain the root `toctree` directive.

.. warning:: documentation is work in progress

Welcome to pyparadiseo's documentation!
=======================================

.. toctree::
   :maxdepth: 1
   :caption: Contents:

   installation
   tutorials
   modules


Indices and tables
==================

* :ref:`modindex`
* :ref:`genindex`
* :ref:`search`

tutorials

Tutorials
---------
Problem Definition
++++++++++++++++++

Maximization/Minimization
+++++++++++++++++++++++++

Number of Objectives
++++++++++++++++++++

`Solution` class
++++++++++++++++
A Solution consists of an `encoding`, a fitness and (for multi-objective) a diversity and objectiveVector
- encoding : a python object
- fitness : must be convertible to double

## Search space definition
Most pyparadiseo algorithms use `initializer` callable objects.

takes solution as argument and initializes encoding.

can be
- for standard solution type
```init = pp.initializer.BinaryInit(100)```

- custom : pass a python callable that returns a (random) solution-encoding
    - ''




Fitness evaluation
++++++++++++++++++

Objective evaluator
+++++++++++++++++++



# Simple Genetic Algorithm
~~~~~~~~~~~~~~~~~~~~~~~~~~

# Local Search
~~~~~~~~~~~~~~
