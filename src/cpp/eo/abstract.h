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

#include <pyeot.h>


template <typename SolutionType>
void export_abstract(std::string type)
{
    // eoUF : eoPop<PyMOEO> ---> void
    def_abstract_functor<eoTransform<SolutionType> >(make_name("eoTransform",type).c_str(),
        "Abstract Base Class.\n\n Functor : Population ==> void.\n\n Transforms a Population. ");

    /* EO SELECTORS */
    def_abstract_functor<eoSelect<SolutionType> >(
        make_name("eoSelect",type).c_str(),
        "Abstract Base Class.\n\n \
        Functor : (Population, Population) ==> void.\n\n \
        Selects individuals from first population, putting them in second. ");

    def_abstract_functor<eoAlgo<SolutionType>>(make_name("eoAlgo",type).c_str(),"Abstract base class. Unary functor : Population -> void");

    def_abstract_functor<eoEvalFunc<SolutionType> >(make_name("eoEvalFunc",type).c_str());
    def_abstract_functor<eoPopEvalFunc<SolutionType> >(make_name("eoPopEvalFunc",type).c_str());

    // eoUF : PyMOEO ---> void
    def_abstract_functor<eoInit<SolutionType> >(make_name("eoInit",type).c_str(),"docstring");
    def_abstract_functor<eoInit<BinarySolution> >(make_name("BinaryInit",type).c_str(),"docstring");

    def_abstract_functor<eoBreed<SolutionType> >(make_name("eoBreed",type).c_str());
    def_abstract_functor<eoContinue<SolutionType> >(make_name("eoContinue",type).c_str());
    def_abstract_functor<eoMerge<SolutionType> >(make_name("eoMerge",type).c_str());

    def_abstract_functor<eoReplacement<SolutionType> >(make_name("eoReplacement",type).c_str());
}

#endif
