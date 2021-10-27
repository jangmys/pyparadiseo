#include <boost/python.hpp>

#include <pyeot.h>
#include <pymo.h>

#include <utils/def_abstract_functor.h>

#include <eval/moEval.h>
#include <eval/moNeighborhoodEvaluation.h>
//
#include <eval/moFullEvalByCopy.h>
#include <eval/moFullEvalByModif.h>


void moEvaluators(){
    using namespace boost::python;

    def_abstract_functor<moEval<PyNeighbor> >("moEval");
    def_abstract_functor<moNeighborhoodEvaluation<PyNeighbor> >("moNeighborhoodEvaluation");

    class_<moFullEvalByCopy<PyNeighbor>,bases<moEval<PyNeighbor>>>
    ("moFullEvalByCopy",
    "parameters : eoEvalFunc \n \
    // __call__ evaluates neighbor by making a tmp copy of solution, move it, evaluate and set fitness of neighbor",
    init<
        eoEvalFunc<PyEOT>&
    >(args("_eoEval"), "__init__ docstring")
    [WC1]
    )
    .def("__call__",&moFullEvalByCopy<PyNeighbor>::operator())
    ;

    class_<moFullEvalByModif<PyNeighbor>,bases<moEval<PyNeighbor>>>
    ("moFullEvalByModif",
    "parameters : eoEvalFunc \n \
    __call__ moves solution, evaluates it an moves it back : requires moveBack to be defined in PyNeighbor",
    init<
        eoEvalFunc<PyEOT>&
    >(args("_eoEval"), "__init__ docstring")
    [WC1]
    )
    .def("__call__",&moFullEvalByModif<PyNeighbor>::operator())
    ;
};
