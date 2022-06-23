import matplotlib
import matplotlib.pyplot as plt

import random


##as in deap/example/knapsack....
random.seed(64)

NBR_ITEMS = 20
MAX_WEIGHT = 50

#a dictionary containing per-item weights and values
items = {}
for i in range(NBR_ITEMS):
    items[i] = (random.randint(1, 10), random.uniform(0, 100))

def evalKnapsack(individual):
    weight = 0.0
    value = 10000.0
    for item in individual:
        weight += items[item][0]
        value += items[item][1]
    if len(individual) > NBR_ITEMS or weight > MAX_WEIGHT:
        return 10000, 0             # Ensure overweighted bags are dominated
    return weight, value

def mutSet(individual):
    """Mutation that pops or add an element."""
    if random.random() < 0.5:
        if len(individual) > 0:     # We cannot pop from an empty set
            individual.remove(random.choice(sorted(tuple(individual))))
    else:
        individual.add(random.randrange(NBR_ITEMS))
    return True
    # return individual, #True

def cxSet(ind1, ind2):
    """Apply a crossover operation on input sets. The first child is the
    intersection of the two sets, the second child is the difference of the
    two sets.
    """
    temp = set(ind1)                # Used in order to keep type
    ind1 &= ind2                    # Intersection (inplace)
    ind2 ^= temp                    # Symmetric Difference (inplace)
    return True
    # return ind1, ind2

def make_solutionGenerator(_length):
    length = _length
    def make_sol():
        a = set()
        for i in range(length):
            if random.random() < 0.5:
                a.add(i)
        return a
    return make_sol


def do_plot(pop):
    l1 = []
    l2 = []

    for indy in pop:
        l1.append(indy.objectiveVector[0])
        l2.append(indy.objectiveVector[1])

    fig, ax = plt.subplots()
    ax.scatter(l1, l2)
    ax.set(xlabel='weight', ylabel='value', title='Knapsack')
    ax.grid()
    plt.show()
