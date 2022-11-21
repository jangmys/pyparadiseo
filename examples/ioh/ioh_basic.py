import ioh

from ioh import get_problem

import pyparadiseo
from pyparadiseo import config
from pyparadiseo import solution
from pyparadiseo import bounds
from pyparadiseo import evaluator

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
    sol = solution.random(5,bounds=bounds.box(f.constraint.lb,f.constraint.ub))

    # print(sol)
    f_eval(sol)
    print("current = ",sol.fitness)
    # print("res=",f(sol.encoding))
    print("#"*10,f.state)











f_eval(sol)


print(sol)
