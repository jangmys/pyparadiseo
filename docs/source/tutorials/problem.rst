Problem definition
==================

In PyParadisEO the minimum required to define an optimization problem is
    - a solution encoding
    - a fitness evaluation

Solution definition
-------------------
All PyParadisEO algorithms act on `Solution` class objects.

    .. code-block:: python

        import pyparadiseo
        # Solution with empty (None) encoding
        sol = pyparadiseo.Solution()
        # Make Solution with encoding [1,2,3.0,'A']
        sol = pyparadiseo.Solution([1,2,3.0,'A'])


Solutions can be constructed explicitly, either by leaving the 'encoding' member empty or by setting it to any Python object.

Most PyParadisEO algorithms use `initializer` objects that tell algorithms how to create new solutions.
To be used in a PyParadisEO algorithm an initializer must be an instance of `eoInit` (a unary functor that sets the encoding of a Solution.

There are three ways to create initializers.

1. Use standard initializers

    The `initializer` module provides initializers for the following standard encodings
    - Binary (`pyparadiseo.initializer.BinaryInit`)
    - Real (`pyparadiseo.initializer.RealBoundedInit`)
    - Permutation (`pyparadiseo.initializer.PermutationInit`)

    These standard encodings are based on numpy arrays and use random initialization. For example

    .. code-block:: python

        # create a permutation-initializer of length 8
        perm_init = pyparadiseo.initializer.PermutationInit(8)
        # initialize sol
        perm_init(sol)

        # or
        pyparadiseo.initializer.PermutationInit(8)(sol)

        # print solution
        print(sol)

2. Specialize the `eoInit` base class, overriding the `__call__` operator. For instance

    .. code-block:: python

        from pyparadiseo.initializer import eoInit

        class myInit(eoInit):
            def __init__(self,n):
                eoInit.__init__(self)
                self.size = n
            def __call__(self,sol):
                # eoInit == set encoding
                sol.encoding = np.random.randint(0,2,self.size)

        myInit(12)(sol)

3. Make an initializer from a Python callable

    It might be more convenient to simply make an initializer from a Python callable returning a Solution.encoding.
    The constructor of the `pyparadiseo.Init` class accepts any callable that takes no input argument and returns a python object. For example

    .. code-block:: python

        # create initializer from a function
        def foo() : return set(range(10))
        myinit = pyparadiseo.initializer.Init(foo)

        # create initializer from a lambda
        myInit = pyparadiseo.initializer.Init(lambda : set(range(10)))


Fitness evaluation
------------------
Basic fitness evaluators are instances of the eoEvalFunc base class, which is basically a setter for the solution fitness.
Users must ensure that the fitness evaluator is compatible with the solution encoding.
One can make a fitness evaluator in two ways.

1. Specialize the `eoEvalFunc` base class, overriding the `__call__` operator.

    .. code-block:: python

        from pyparadiseo.evaluator import eoEvalFunc

        class myEval(eoEvalFunc):
            def __init__(self):
                eoEvalFunc.__init__(self)
            def __call__(self, sol):
                # eoEvalFunc == set fitness
                sol.fitness = np.sum(sol.encoding)

2. Make fitness evaluator from a Python callable that receives a solution encoding and returns a fitness value (anything convertible to double)

    .. code-block:: python

        from pyparadiseo.evaluator import FitnessEval

        # from a python function
        def foo(x):
            return np.sum(x)
        eval = FitnessEval(foo)
        eval(sol)

        # from a lambda
        eval = FitnessEval(lambda x: np.sum(x))
        eval(sol)
