.. _pop-ref:

Population
==========
In ParadisEO, a population (`eoPop`) is a C++ std::vector<EOT> of individuals.

In PyParadisEO, populations can be manipulated much like lists of solutions.

However, users should not create populations directly, but through the named constructors of the `pyparadiseo.population` module.

Creating a population
---------------------
This is how you make an empty population::

    from pyparadiseo import population

    pop = population.empty()
