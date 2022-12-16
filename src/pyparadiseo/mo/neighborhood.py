"""
MO neighborhoods

A neighborhood specifies how to iterate over neighbors.

Neighbor
========
A Neighbor is **a place where a solution can be moved to**.

- a fitness / objective vector
- a 1D index
- a ```move``` function that moves the solution

Neighbors are not directly exposed to the user. Instead the ``move function`` is passed to the MO algorithm.

The ``move`` function ...

Examples of moves
~~~~~~~~~~~~~~~~~
# bitflip for binary solution
def move_bitflip(nbor,sol):
    sol[nbor.index()] = 1 - sol[nbor.index()]

# swap for permutation
class move_swap():
    def __init__(self,n_vars):
        self.table = []
        for i in range(0,n_vars):
            for j in range(i+1,n_vars):
                self.table.append((i,j))

    def __call__(self,nbor,sol):
        i,j=self.table[nbor.index()]

        sol[i],sol[j] = sol[j],sol[i]



Move (neighbor) evaluation
==========================
An neighbor evaluator evaluates the cost of a neighbor = the cost of the solution if it were moved there.

1. full eval by copy

- copy solution to tmp
- move tmp
- eval tmp
- delete tmp

2. full eval by modif

if a `move_back` is provided

- move solution
- eval
- move back

3. incremental eval

use solution cost and neighbor index to compute neighbor cost. incremental evaluators are closely related to the move definition but do not use the move directly.

for instance, an incremental evaluator for the move_bitflip is

def eval(sol,nbor):
    if sol[nbor.index()]:
        return sol.fitness - 1
    else:
        return sol.fitness + 1

It might be convienient to define both, the move and incremental evaluator in the same class.

For example swap evaluation in the TSP

class move_swap()
    def __init__(self,n_vars):
        self.table = []
        for i in range(0,n_vars):
            for j in range(i+1,n_vars):
                self.table.append((i,j))

    def __call__(self,nbor,sol):
        i,j=self.table[nbor.index()]

        sol[i],sol[j] = sol[j],sol[i]

    def eval(self,nbor,sol):
        a,b=self.table[nbor.index()]
        nbor_fitness = sol.fitness
        nbor_fitness -= cost_of_edge(a-1,a)
        nbor_fitness -= cost_of_edge(a,a+1)
        nbor_fitness += cost_of_edge(a-1,b)
        nbor_fitness += cost_of_edge(b,a+1)

        nbor_fitness -= cost_of_edge(b-1,b)
        nbor_fitness -= cost_of_edge(b,b+1)
        nbor_fitness += cost_of_edge(b-1,a)
        nbor_fitness += cost_of_edge(a,b+1)

        return nbor_fitness




"""
from pyparadiseo import utils,config

#ABC
from .._core import moNeighborhood
from .._core import moIndexNeighborhood

__all__=['moNeighborhood','moIndexNeighborhood','neighborhood','ordered','random']

moNeighborhood.__doc__="""
Abstract Base Class. A Neighborhood defines how to iterate over Neighbors.

Subclassing moNeighborhood allows to define custom neighborhoods. Following functions must be specified by subclasses.

init : (solution,neighbor)->None
    initialize first neighbor

next : Callable (solution,neighbor)->None
    set neighbor to next

cont : Callable (solution)->Bool
    continue ?

hasNeighbor : Callable (solution)->Bool
    solution has neighbor?

isRandom : bool
    True if neighborhood is random

Example
=======
from pyparadiseo.mo import moNeighborhood

class my_nhood(moNeighborhood):
    def __init__(self,nhood_size):
        moNeighborhood.__init__(self)
        self.current_index = 0
        self.size = nhood_size

    def init(self,sol,nbor):
        self.current_index = 0
        nbor.index = self.current_index

    def next(self,sol,nbor):
        self.current_index += 1
        nbor.index = self.current_index

    def cont(self,sol):
        return (self.currentIndex < self.size - 1)

    def hasNeighbor(self,sol):
        return (self.size > 0)

    def isRandom(self,sol):
        return (self.size > 0)

"""


def neighborhood(init,next,cont,has_neighbor,is_random=False,stype=None):
    """
    A user-defined Neighborhood.

    Providing the required functions is equivalent to inheriting from py:class:`moNeighborhood`

    It may be more convenient to use this, as there is no need to stick to method-names of the base class, nor parent initialization.

    Also, it is possible to leave methods as 'None'.

    Parameters
    ==========
    init : Callable (solution,neighbor)->None
        initialize first neighbor
    next : Callable (solution,neighbor)->None
        set neighbor to next
    cont : Callable (solution)->Bool
        continue ?
    has_neighbor : Callable (solution)->Bool
        solution has neighbor?
    is_random : bool, Optional
        True if neighborhood is random

    Example
    =======
    # Neighborhood that contains a single neighbor

    def init(sol,nbor):
        nbor.encoding = perturb(sol)

    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("pyNeighborhood"+config.TYPES[stype])

    return class_(init,next,cont,has_neighbor,is_random)


def ordered(neighborhood_size: int,stype=None):
    """
    An ordered indexed neighborhood

    a special sort of IndexNeighborhood

    Parameters
    ----------
    neighborhood_size
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moOrderNeighborhood"+config.TYPES[stype])
    return class_(neighborhood_size)


def random(neighborhood_size: int,max_neighbors: int=0,with_replacement=True,stype=None):
    """
    A random neighborhood

    a special sort of IndexNeighborhood

    Parameters
    ----------
    neighborhood_size - int, size of neighborhood
    max_neighors - int, max nb visited nbors (0 represents infinity)
    with_replacement = bool
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_=None
    if with_replacement:
        class_ = utils.get_class("moRndWithReplNeighborhood"+config.TYPES[stype])
        return class_(neighborhood_size,max_neighbors)
    else:
        class_ = utils.get_class("moRndWithoutReplNeighborhood"+config.TYPES[stype])
        return class_(neighborhood_size)
