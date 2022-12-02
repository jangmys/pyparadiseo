"""
Breeding: combination of selecting and transforming a population

Breeding is thought of as a combination of selecting and transforming a
population. For efficiency reasons you might want to build your own
eoBreed derived class rather than relying on a seperate select and
transform function.


@see eoSelect, eoTransform, eoSelectTransform

base : eoBreed.h
``__call__(parent_pop,offspring_pop) -> None``

eoGeneralBreeder.h
eoOneToOneBreeder.h
"""
from pyparadiseo import config, utils

from .._core import eoBreed
eoBreed.__doc__="""Abstract base class for EO algorithms.

It defines a functor ``__call__(const pop1,pop2) -> None``.

Use ``eoBreed`` creation methods in :py:mod:`~pyparadiseo.eo.breeders`
"""

__all__=['breeder','select_transform','general_breeder','one_to_one_breeder','eoBreed']


def breeder(klass_or_stype=None, stype=None):
    """class decorator

    apply to python class that has

    ``__call__(parents,offspring)``

    """
    if klass_or_stype is not None and stype is None:
        stype = config._SOLUTION_TYPE

    base_ = utils.get_class("eoBreed" + config.TYPES[stype])

    def wrap(kls):
        if not hasattr(kls, "__call__"):
            print("need (__call__(pop1,pop2) -> None)")
        if not hasattr(kls, "__init__"):
            print("need (__init__")

        class derived(kls, base_):
            pass

        return derived

    if klass_or_stype is None:
        return wrap
    else:
        return wrap(klass_or_stype)


def select_transform(select, transform, stype=None):
    """
    make a breeder from a select and transform operators

    SelectTransform(parents,offspring)

    eoBreed.h

    Applies selector to parents (fill offspring), then transforms offspring

    select : parents -> offspring
    transform : offspring -> offspring

    Parameters
    ----------
    select :
    transform :
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoSelectTransform" + config.TYPES[stype])

    return class_(select, transform)


def general_breeder(select_one, gen_op, rate=1.0, interpret_as_rate=True, stype=None):
    """GeneralBreeder

    eoGeneralBreeder.h

    applies select_one to get a number (...) of offspring and calls gen_op on selected individuals

    Parameters
    ----------
    select_one : a selectoOne, to be used for all selections
    gen_op : a general operator (will generally be an eoOpContainer)
    rate : pour howMany, le nbre d'enfants a generer
    _interpret_as_rate  <a href="../../tutorial/html/eoEngine.html#howmany">explanation</a>
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoGeneralBreeder" + config.TYPES[stype])

    return class_(select_one, gen_op, rate, interpret_as_rate)


def one_to_one_breeder(gen_op, f_eval, p_replace=1.0, stype=None):
    """OneToOneBreeder

    eoOneToOneBreeder.h

    transforms a population using
    - an operator that MODIFIES only one parent from the populator (though it can use any number aside) and thus generates ONE offspring)
    - a local replacement between the parent and its offspring

    Ctor:
    * @param _op       a general operator (must MODIFY only ONE parent)
    * @param _eval     an eoEvalFunc to evaluate the offspring
    * @param _pReplace probability that the best of parent/offspring wins [1]
    * @param _howMany  eoHowMany offpsring to generate [100%]

    Typically, Differential Evolution (Storn and Price 94) and Deb `et al`'s G3 can be built on this
    """
    if stype is None:
        stype = config._SOLUTION_TYPE

    class_ = utils.get_class("eoOneToOneBreeder" + config.TYPES[stype])

    return class_(gen_op, f_eval, p_replace)
