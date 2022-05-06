"""
Breeding: combination of selecting and transforming a population

Breeding is thought of a combination of selecting and transforming a
population. For efficiency reasons you might want to build your own
eoBreed derived class rather than relying on a seperate select and
transform function.

@see eoSelect, eoTransform, eoSelectTransform

base : eoBreed.h
__call__(parent_pop,offspring_pop) --> None

eoGeneralBreeder.h
eoOneToOneBreeder.h
"""

from pyparadiseo import config,utils

from .._core import eoSelectTransform as SelectTransform
from .._core import eoGeneralBreeder as GeneralBreeder
from .._core import eoOneToOneBreeder as OneToOneBreeder

__all__ = [SelectTransform,GeneralBreeder,OneToOneBreeder]

class _SelectTransform():
    """SelectTransform(parents,offspring)

    eoBreed.h

    Applies selector to parents (fill offspring), then transforms offspring

    select : parents -> offspring
    transform : offspring -> offspring

    Parameters
    ----------
    select :
    transform :
    """
    def __new__(cls,select,transform,type=None):
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoSelectTransform"+config.TYPES[type])

        return class_(select,transform)


class _GeneralBreeder():
    """GeneralBreeder

    eoGeneralBreeder.h

    applies select_one to get a number (...) of offspring and calls gen_op on selected individuals
    """
    def __new__(cls,select_one,gen_op,rate=1.0,interpret_as_rate=True,type=None):
        """
          /** Ctor:
           *
           * @param _select a selectoOne, to be used for all selections
           * @param _op a general operator (will generally be an eoOpContainer)
           * @param _rate               pour howMany, le nbre d'enfants a generer
           * @param _interpret_as_rate  <a href="../../tutorial/html/eoEngine.html#howmany">explanation</a>
           */
        """
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoGeneralBreeder"+config.TYPES[type])

        return class_(select_one,gen_op,rate,interpret_as_rate)


class _OneToOneBreeder():
    """OneToOneBreeder

    eoOneToOneBreeder.h

    transforms a population using
     *   - an operator that MODIFIES only one parent from the populator
     *     (though it can use any number aside) and thus generates ONE offspring)
     *   - a local replacement between the parent and its offspring
     *
     * Typically, Differential Evolution (Storn and Price 94) and Deb et al's
     *   G3 can be built on this
    """
    def __new__(cls,gen_op,f_eval,p_replace=1.0,type=None):
        """
        /** Ctor:
        * @param _op       a general operator (must MODIFY only ONE parent)
        * @param _eval     an eoEvalFunc to evaluate the offspring
        * @param _pReplace probability that the best of parent/offspring wins [1]
        * @param _howMany  eoHowMany offpsring to generate [100%]
        */
        """
        if type is None:
            type = config._SOLUTION_TYPE

        class_ = utils.get_class("eoOneToOneBreeder"+config.TYPES[type])

        return class_(gen_op,f_eval,p_replace)
