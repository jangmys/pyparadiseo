import ioh

from ioh import get_problem

import pyparadiseo
from pyparadiseo import config
from pyparadiseo import solution
from pyparadiseo import bounds
from pyparadiseo import evaluator

from pyparadiseo import operator

from pyparadiseo.eo import algo
from pyparadiseo.eo import select_one



# https://arxiv.org/pdf/2102.06435.pdf




f = get_problem("Sphere", dimension=5, instance=1)

#Print some properties of the problem
print(f.meta_data)

print(f.state)


def random_search(problem: ioh.problem.Real, seed: int = 42, budget: int = None) -> ioh.RealSolution:
    np.random.seed(seed)

    if budget is None:
        budget = int(problem.meta_data.n_variables * 1e4)

    for _ in range(budget):
        x = np.random.uniform(problem.constraint.lb, problem.constraint.ub)

        # problem automatically tracks the current best search point
        f = problem(x)

    return problem.state.current_best


config.set_solution_type('real')

f_eval = evaluator.fitness(f)

for i in range(5):
    sol = solution.random(5,bounds=bounds.bound_box(f.constraint.lb,f.constraint.ub))

    # print(sol)
    f_eval(sol)
    print("current = ",sol.fitness)
    # print("res=",f(sol.encoding))
    print("#"*10,f.state)



config.set_solution_type('bin')

crossover_rate = [0,0.2,0.4,0.5,0.6,0.8]
mutation_rate = [0,0.2,0.4,0.5,0.6,0.8]

select = [
    select_one.random(),
    select_one.stoch_tournament(0.5),
    select_one.elite_sequential(),
    select_one.proportional(),
    select_one.det_tournament(2),
    select_one.det_tournament(6),
    select_one.det_tournament(10)
    ]

crossover = [
    operator.uniform_bit_cx(0.1),
    operator.uniform_bit_cx(0.3),
    operator.uniform_bit_cx(0.5),
    operator.uniform_bit_cx(0.7),
    operator.uniform_bit_cx(0.9),
    operator.n_point_bit_cx(1),
    operator.n_point_bit_cx(3),
    operator.n_point_bit_cx(5),
    operator.n_point_bit_cx(7),
    operator.n_point_bit_cx(9)
    ]

mutation = [
    operator.uniform_bit_mutation(),
    operator.standard_bit_mutation()
    ]









f_eval(sol)


print(sol)
