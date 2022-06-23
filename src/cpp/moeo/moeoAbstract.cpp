#include <algo/moeoEA.h>

#include <utils/def_abstract_functor.h>

#include <pyeot.h>

template <typename SolutionType>
void moeo_export_abstract(std::string type)
{
    def_abstract_functor<moeoEA<SolutionType>>(make_name("moeoEA",type).c_str());
}

void moeo_abstract()
{
    moeo_export_abstract<PyEOT>("");
    moeo_export_abstract<BinarySolution>("Bin");
    moeo_export_abstract<RealSolution>("Real");
}
