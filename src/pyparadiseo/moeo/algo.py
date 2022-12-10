"""
Multiobjective Algorithms
=========================

easyEA :

NSGA :

NSGA-II :

MOGA :

SPEA2 :

SEEA :

"""
from pyparadiseo import config, utils

from pyparadiseo.eo.continuator import  eoContinue

__all__=['easyea','nsga','nsgaII','moga','spea2','seea']

def easyea(eval, breed, continuate, replacement, fitness_assign, diversity_assign, evalFitAndDivBeforeSelection=False):
    """
    An easy class to design multi-objective evolutionary algorithms.

    Parameters
    ==========
    eval : eoEvalFunc
        evaluation function
    breed : eoBreed
        breeder
    continuate : eoContinue
        termination
    replacement : eoReplacement
        replacement
    fitness_assign : moeoFitnessAssignment
        assign fitness
    diversity_assign : moeoDiversityAssignment
        assign diversity
    evalFitAndDivBeforeSelection : bool
        evaluate fitness and diversity before selection
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

        class_ = utils.get_class("moeoEasyEA" + config.TYPES[stype])

        return class_(continuate, eval, breed, replacement, fitness_assign, diversity_assign, evalFitAndDivBeforeSelection)

# NSGA / NSGAII /MOGA
#####################
# virtual void operator () (eoPop < MOEOT > &_pop)
# {
#     eoPop < MOEOT > offspring, empty_pop;
#     popEval (empty_pop, _pop);	// a first eval of _pop
#     // evaluate fitness and diversity
#     fitnessAssignment(_pop);
#     diversityAssignment(_pop);
#     do
#     {
#         // generate offspring, worths are recalculated if necessary
#         breed (_pop, offspring);
#         // eval of offspring
#         popEval (_pop, offspring);
#         // after replace, the new pop is in _pop. Worths are recalculated if necessary
#         replace (_pop, offspring);
#     }
#     while (continuator (_pop));
# }


def nsga(eval, transform, continuate, stype=None):
    """
    NSGA (Non-dominated Sorting Genetic Algorithm).
    N. Srinivas, K. Deb, "Multiobjective Optimization Using Nondominated Sorting in Genetic Algorithms".
    Evolutionary Computation, Vol. 2(3), No 2, pp. 221-248 (1994).
    This class builds the NSGA algorithm only by using the fine-grained components of the ParadisEO-MOEO framework.

    Parameters
    ==========
    eval : eoEvalFunc or eoPopEvalFunc
        evaluate objectives
    transform : eoGenOp
        (cross,p_cross,mutate,p_mutate),(GenOp),(eoTransform)
    continuator : int or eoContinue
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moeoNSGA" + config.TYPES[stype])

    #continuator argument : int or eoContinue
    if isinstance(continuate,int):
        continuate = utils.get_class("eoGenContinue" + config.TYPES[stype])(continuate)

    #eval function argument : eoEvalFunc or eoPopLoopEval
    if isinstance(eval,utils.get_class("eoEvalFunc" + config.TYPES[stype])):
        eval = utils.get_class("eoPopLoopEval" + config.TYPES[stype])(eval)

    #transform : tuple(cross,pCross,mut,pMut) or eoGenOp or eoTransform
    if isinstance(transform,tuple):
        transform = utils.get_class("eoSGAGenOp" + config.TYPES[stype])(*transform)

    return class_(continuate, eval, transform)


def nsgaII(eval, transform, continuate, stype=None):
    """
    NSGA-II (Non-dominated Sorting Genetic Algorithm II).

    Deb, K., S. Agrawal, A. Pratap, and T. Meyarivan. A fast elitist non-dominated sorting genetic algorithm for multi-objective optimization: NSGA-II. IEEE Transactions on Evolutionary Computation, Vol. 6, No 2, pp 182-197 (2002).
    This class builds the NSGA-II algorithm only by using the fine-grained components of the ParadisEO-MOEO framework.

    Parameters
    ==========
    eval : eval_func or pop_eval
    transform : (cross,p_cross,mutate,p_mutate) [tuple]
        (GenOp)
        (Transform)
    continuator : int or eoContinue
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moeoNSGAII" + config.TYPES[stype])

    #continuator argument : int or eoContinue
    if isinstance(continuate,int):
        continuate = utils.get_class("eoGenContinue" + config.TYPES[stype])(continuate)

    #eval function argument : eoEvalFunc or eoPopLoopEval
    if isinstance(eval,utils.get_class("eoEvalFunc" + config.TYPES[stype])):
        eval = utils.get_class("eoPopLoopEval" + config.TYPES[stype])(eval)

    #transform : tuple(cross,pCross,mut,pMut) or eoGenOp or eoTransform
    if isinstance(transform,tuple):
        transform = utils.get_class("eoSGAGenOp" + config.TYPES[stype])(*transform)

    return class_(continuate, eval, transform)


def moga(eval, transform, continuate, stype=None):
    """
    MOGA
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moeoMOGA" + config.TYPES[stype])

    return class_(continuate, eval, transform)


# //SPEA2
# virtual void operator () (eoPop < MOEOT > &_pop)
# {
#     eoPop < MOEOT >empty_pop, offspring;
#     popEval (empty_pop, _pop);// a first eval of _pop
#     //
#     fitnessAssignment(_pop); //a first fitness assignment of _pop
#     diversityAssignment(_pop);//a first diversity assignment of _pop
#
#     archive(_pop);//a first filling of archive <=============================================
#     while (continuator (_pop))
#     {
#         // generate offspring, worths are recalculated if necessary
#         breed (_pop, offspring);
#         popEval (_pop, offspring); // eval of offspring
#         // after replace, the new pop is in _pop. Worths are recalculated if necessary
#         replace (_pop, offspring);
#
#         fitnessAssignment(_pop); //fitness assignment of _pop
#         diversityAssignment(_pop); //diversity assignment of _pop
#         archive(_pop); //control of archive
#     }
# }
def spea2(eval, transform, continuate, archive, k=1, nocopy=False, stype=None):
    """
    SPEA2 algorithm.
    E. Zitzler, M. Laumanns, and L. Thiele. SPEA2: Improving the Strength Pareto Evolutionary Algorithm. Technical Report 103,
    Computer Engineering and Networks Laboratory (TIK), ETH Zurich, Zurich, Switzerland, 2001.
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moeoSPEA2" + config.TYPES[stype])

    return class_(continuate, eval, transform, archive)


def seea(eval, transform, continuate, archive, stype=None):
    """
    SEEA (Simple Elitist Evolutionary Algorithm).
    Liefooghe A. Jourdan L., Talbi E.-G.. Metaheuristics and Their Hybridization to Solve the Bi-objective Ring Star Problem: a Comparative Study. Technical Report RR-6515, INRIA, 2008
    This class builds SEEA by using the fine-grained components of the ParadisEO-MOEO framework.
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("moeoSEEA" + config.TYPES[stype])

    return class_(continuate, eval, transform, archive)
