Fixed-lenth solutions
=============================
.. currentmodule:: pyparadiseo.solution

Fixed-length solutions have a ``numpy`` array as solution encoding.

* RealSolution : dtype=real
* BinarySolution : dtype=int
* IntSolution : dtype=int


.. autoclass:: RealSolution
    :exclude-members:


.. autoclass:: BinarySolution
    :exclude-members: invalid,invalidate,invalidObjectiveVector,invalidateObjectiveVector,resize,array,carray,diversity,encoding,fitness,objectives

    Just like :class:`RealSolution` but with ``int``


.. autoclass:: IntSolution
    :exclude-members: invalid,invalidate,invalidObjectiveVector,invalidateObjectiveVector,resize,array,carray,diversity,encoding,fitness,objectives

    Just like :class:`RealSolution` but with ``int``
