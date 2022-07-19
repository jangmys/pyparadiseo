The Evolving Objects (EO) component allows to build population-based metaheurtistics

PyParadisEO provides three algorithms:

1. simpleGA
    Simple Genetic Algorithm following Holland and Goldberg
2. easyEA
    Easy-to-Use Evolutionary Algorithm : Breed and Replace
3. fastGA
    Fine-tuneable GA


simpleGA
--------
**eoSGA.h**

Simple GA following Holland and Goldberg.

It requires :
- a `selector`
    - pass a selectOne!
- a `crossover` and a `crossover-rate`
- a `mutation` and a `mutation-rate`
- a fitness `evaluator`
- a `continuator` (stopping criterion)

Pseudo-code
```
1. select pop.size() individuals from pop and copy to offspring
2. apply crossover to offspring
3. apply mutation to offspring
4. replace parents with offspring
5. repeat unless termination criterion met
```

Notes:
- Pop needs to be evaluated before running SGA


-------------------------------------

easyEA
------
**eoEasyEA.h**

An easy-to-use evolutionary algorithm.

It requires
- a continuator
- a fitness evaluator
- a breeder = select + transform
- a replacer = merge + reduce

Notes:
- Pop is evaluated before EA-loop

-----------------------------------------------

fastGA
------
**eoFastGA.h**

default offspring-size = popsize

for i in range(offspring-size):
    if(flip(xover_rate)):
        A,B = select_cross two individuals
        A,B = do crossover
        eval(A,B)
        C = select_aftercross one(A,B)
        if(flip(mutation_rate)):
            C= do mutate
        insert(C)
    else:
        C = select_mut
        mutate(C)
        insert(C)

    pop-eval
    replace


It requires
- selectOne
    - for crossover
    - for selection after crossover
    - for mutation
- crossover proba
- crossover
- mutation
- population eval
- replacement
- continuator
- offspring_size (int)

Notes:
- no initial evaluation of Pop


# COMPONENTS

## Mergers
    (const source_pop, dest_pop) --> void

    copy (part of) source_pop into dest_pop
## Reduce
    (pop, unsigned int) --> void

    reduce pop to specified size
## Replace
    (parents,offspring) --> void

    replace parents by offspring : can be obtained as merge+reduce

## Select
    - many
    - one

## Transform
    pop --> void

    apply operators to pop

## Continuator

## Breeder
