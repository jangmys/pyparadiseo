#include <pyeot.h>
#include <pymo.h>


#include <sampling/moSampling.h>

using namespace boost::python;



template<typename SolutionType,typename ValueType>
void expose_moSampling(std::string name)
{
    typedef PyNeighbor<SolutionType> NborT;

    class_<moSampling<NborT>>
    (make_name("moSampling",name).c_str(),init<
        eoInit<SolutionType>&,
        moLocalSearch<NborT>&,
        moStat<SolutionType,ValueType>&,
        optional<bool>
    >())
    // .def("add_stat",&moSampling<NborT>::add)
    .def("__call__",&moSampling<NborT>::operator())
    ;
}


void moSample()
{
    // expose_moSampling<PyEOT,double>("");
    // expose_moSampling<BinarySolution,double>("Bin");
    // expose_moSampling<RealSolution,double>("Real");
}
