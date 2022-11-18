import ioh

from ioh import get_problem

import pyparadiseo
from pyparadiseo import config
from pyparadiseo import solution
from pyparadiseo import bounds
from pyparadiseo import evaluator
from pyparadiseo import population
from pyparadiseo import initializer

from pyparadiseo import operator

from pyparadiseo.eo import algo
from pyparadiseo.eo import select_one
from pyparadiseo.eo import replacement
from pyparadiseo.eo import continuator

import numpy as np

# see https://arxiv.org/pdf/2102.06435.pdf




def random_search(problem: ioh.problem.Real, seed: int = 42, budget: int = None) -> ioh.RealSolution:
    np.random.seed(seed)

    if budget is None:
        budget = int(problem.meta_data.n_variables * 1e4)

    for _ in range(budget):
        x = np.random.uniform(problem.constraint.lb, problem.constraint.ub)

        # problem automatically tracks the current best search point
        f = problem(x)

    return problem.state.current_best

# ------------------ Problem ------------------
# f = get_problem("Sphere", dimension=5, instance=1)

#Print some properties of the problem
# print(f.meta_data)
# print(f.state)
config.set_solution_type('bin')
# config.set_minimize_fitness()



from ioh import problem

# W-model parameters as in https://arxiv.org/pdf/2102.06435.pdf
pb_dims = [20,20,16,48,25,32,128,128,128,50,100,150,128,192,192,192,256,75,150]
mu = [2,2,1,3,1,1,4,4,4,1,2,3,2,3,3,3,4,1,2]
epi = [6,6,5,9,23,2,11,14,8,36,21,16,32,21,21,21,52,60,32]
gamma = [10,18,72,72,90,397,0,0,128,245,256,613,256,16,256,403,2,16,4]
max_target = [10,10,16,16,25,32,32,32,32,50,50,50,64,64,64,64,64,75,75]

Wmodel_pbs = []
for i,dim in enumerate(pb_dims):
    print("num",i+1,"dim",dim,"mu",mu[i],"epi",epi[i],"gamma",gamma[i])

    Wmodel_pbs.append(problem.WModelOneMax(i+1, n_variables=dim, dummy_select_rate = 0, epistasis_block_size = epi[i], neutrality_mu = mu[i], ruggedness_gamma = gamma[i]))


for pb in Wmodel_pbs:
    # print(pb.meta_data)
    f_eval = evaluator.fitness(pb,stype='bin')
    sol = solution.random(size=pb.meta_data.n_variables,stype='bin')

    # print(type(sol))
    f_eval(sol)
    # print(sol)

    # print(pb.meta_data.n_variables)


# ------------------  FastGA ------------------
# config.set_solution_type('bin')

# f_eval = evaluator.fitness(f)
#
# for i in range(5):
#     sol = solution.random(lb=f.constraint.lb,ub=f.constraint.ub)
#
#     # print(sol)
#     f_eval(sol)
#     print("current = ",sol.fitness)
#     # print("res=",f(sol.encoding))
#     print("#"*10,f.state)




# == _rate_crossover
crossover_rate = [0,0.2,0.4,0.5,0.6,0.8]
mutation_rate = [0,0.2,0.4,0.5,0.6,0.8]

# == selectC
selectC = [
    eo.select_one.random(),
    eo.select_one.stoch_tournament(0.5),
    # select_one.elite_sequential(),
    eo.select_one.proportional(),
    eo.select_one.det_tournament(2),
    eo.select_one.det_tournament(6),
    eo.select_one.det_tournament(10)
    ]

# == crossover
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
    operator.standard_bit_mutation(),
    operator.conditional_bit_mutation(),
    operator.shifted_bit_mutation(),
    operator.normal_bit_mutation(variance=1.5),
    operator.fast_bit_mutation(beta=1.5),
    operator.bit_flip_n(3,False),
    operator.bit_flip_n(5,False),
    operator.bit_flip_n(7,False),
    operator.bit_flip_n(9,False)
    ]

selectM = selectC

replacement = [
    eo.replace.plus(),
    eo.replace.comma(),
    eo.replace.ssga_worse(),
    eo.replace.ssga_stoch_tournament(0.51),
    eo.replace.ssga_stoch_tournament(0.71),
    eo.replace.ssga_stoch_tournament(0.91),
    eo.replace.ssga_det_tournament(2),
    eo.replace.ssga_det_tournament(4),
    eo.replace.ssga_det_tournament(6),
    eo.replace.ssga_det_tournament(8),
    eo.replace.ssga_det_tournament(10)
]


import time
t1 = time.time()
c=0

# ----------------run all----------------
# for pb in Wmodel_pbs:



pb = np.random.choice(Wmodel_pbs)

print(pb.meta_data)
print("val:",pb(np.asarray([0]*pb.meta_data.n_variables)),pb.objective)


# exit()

for i1,p_cross in enumerate(crossover_rate[3:4]):
    for i4,p_mut in enumerate(mutation_rate[3:4]):
        for i2,crsel in enumerate(selectC):
            for i3,cr in enumerate(crossover):
                for i5,msel in enumerate(selectM):
                    for i6,mut in enumerate(mutation):
                        for i7,repl in enumerate(replacement):
                            print(i1,i2,i3,i4,i5,i6,i7)
                            # pb = np.random.choice(Wmodel_pbs)

                            popeval = evaluator.pop_eval_from_fitness(pb)

                            pop = population.from_init(20,initializer.random(size=pb.meta_data.n_variables,stype='bin'))

                            popeval(pop,pop)

                            c += 1
                            fastGA = algo.fastGA(
                                p_cross,crsel,cr,
                                select_one.random(),
                                p_mut,msel,mut,
                                popeval,
                                repl,
                                continuator.max_generations(10)
                            )

                            fastGA(pop)

                            print(pop.best().fitness,pb.objective.y)


# -------------------choose random-------------------
pb = np.random.choice(Wmodel_pbs)

# f_eval = evaluator.fitness(pb,stype='bin')
popeval = evaluator.pop_eval_from_fitness(pb)

fastGA = algo.fastGA(
    np.random.choice(crossover_rate),
    np.random.choice(selectC),
    np.random.choice(crossover),
    select_one.random(),
    np.random.choice(mutation_rate),
    np.random.choice(selectM),
    np.random.choice(mutation),
    popeval,
    np.random.choice(replacement),
    continuator.max_generations(1000)
)

pop = population.from_init(20,initializer.random(size=pb.meta_data.n_variables,stype='bin'))

popeval(pop,pop)

print(pop.best())

fastGA(pop)

print(pop.best())

print(c)
print(time.time()-t1)




# f_eval(sol)
# print(sol)
