#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>

#include <utils/def_abstract_functor.h>
#include <pyeot.h>

using namespace boost::python;

template<typename SolutionType>
void
export_transform(std::string name)
{
    class_<eoSGATransform<SolutionType>, bases<eoTransform<SolutionType> > >
        ("eoSGATransform",
        "Simple GA Transform\n\n"
        "Transforms a population by successive application of crossover\n"
        "and mutation operators\n",
        init<
            eoQuadOp<SolutionType>&,
            double,
            eoMonOp<SolutionType>&,
            double
        >((arg("self"),arg("_cross"),arg("_cProba"),arg("_mutate"),arg("_mProba")),
        "Construct from a mutation and a crossover.\n\n"
        ":param _cross: a crossover\n"
        ":type _cross: eoQuadOp\n"
        ":param _cProba: crossover probabilty\n"
        ":type _cProba: float\n"
        ":param _mutate: mutation operator\n"
        ":type _mutate: eoMonOp\n"
        ":param _mProba: mutation probabilty\n"
        ":type _mProba: float\n"
        )
        )
        .def("__call__", &eoSGATransform<SolutionType>::operator (),
        (arg("_pop")),
        "applies _mutate and _cross operators on _pop"
        )
        ;
}

void transform(){
    export_transform<PyEOT>("");
    // export_transform<BinarySolution>("Bin");
    // export_transform<RealSolution>("Real");
}
