#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>

#include <utils/def_abstract_functor.h>
#include <pyeot.h>

using namespace boost::python;


void
transform()
{
    class_<eoSGATransform<PyEOT>, bases<eoTransform<PyEOT> > >
        ("eoSGATransform",
        "Simple GA Transform\n\n"
        "Transforms a population by successive application of crossover\n"
        "and mutation operators\n",
        init<
            eoQuadOp<PyEOT>&,
            double,
            eoMonOp<PyEOT>&,
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
        .def("__call__", &eoSGATransform<PyEOT>::operator (),
        (arg("_pop")),
        "applies _mutate and _cross operators on _pop"
        )
        ;
}
