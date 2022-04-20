#ifndef ABSTRACT_H
#define ABSTRACT_H

#include <utils/def_abstract_functor.h>

#include <eoAlgo.h>
#include <eoTransform.h>
#include <eoSelect.h>
#include <eoEvalFunc.h>
#include <eoPopEvalFunc.h>
#include <eoInit.h>
#include <eoBreed.h>
#include <eoContinue.h>
#include <eoMerge.h>
#include <eoReplacement.h>

// #include <pyeot.h>



template <typename SolutionType>
void export_abstract()
{
    // eoUF : eoPop<PyMOEO> ---> void
    def_abstract_functor<eoTransform<SolutionType> >("eoTransform",
        "Abstract Base Class.\n\n Functor : Population ==> void.\n\n Transforms a Population. ");

    /* EO SELECTORS */
    def_abstract_functor<eoSelect<SolutionType> >(
        "eoSelect",
        "Abstract Base Class.\n\n \
        Functor : (Population, Population) ==> void.\n\n \
        Selects individuals from first population, putting them in second. ");


    def_abstract_functor<eoAlgo<SolutionType>>("eoAlgo","Abstract base class. Unary functor : Population -> void");

    def_abstract_functor<eoEvalFunc<SolutionType> >("eoEvalFunc");
    def_abstract_functor<eoPopEvalFunc<SolutionType> >("eoPopEvalFunc");

    // eoUF : PyMOEO ---> void
    def_abstract_functor<eoInit<SolutionType> >("eoInit","docstring");
    def_abstract_functor<eoBreed<SolutionType> >("eoBreed");
    def_abstract_functor<eoContinue<SolutionType> >("eoContinue");
    def_abstract_functor<eoMerge<SolutionType> >("eoMerge");

    def_abstract_functor<eoReplacement<SolutionType> >("eoReplacement");
}

#endif
