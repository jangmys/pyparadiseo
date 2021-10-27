#include "pymo.h"

#include <utils/def_abstract_functor.h>

#include <boost/python.hpp>

// BOOST_PYTHON_MODULE(_mo)
//building different modules on the C++ level is difficult (possible?) because of
//cross-dependencies : building ONE big library, split into python modules via
//__init__ / import ....
void mo()
{
    using namespace boost::python;

    //disambiguate getter/setter
    unsigned int (PyNeighbor::*getIndex)(void) const = &moIndexNeighbor<PyEOT>::index;
    void (PyNeighbor::*setIndex)(unsigned int) = &moIndexNeighbor<PyEOT>::index;
    void (PyNeighbor::*setIndexWithSol)(PyEOT&,unsigned int) = &moIndexNeighbor<PyEOT>::index;

    class_<PyNeighbor,boost::noncopyable>("Neighbor",init<>())
    .def(init<
        PyNeighbor&
    >()
    [WC1]
    )
    .def(init<
        boost::python::object
    >())
    .def(init<
        boost::python::object,
        boost::python::object
    >())
    .add_property("fitness",&PyNeighbor::getFitness,&PyNeighbor::setFitness)
    .def("setMove", &PyNeighbor::setMove)
    .def("setMoveBack", &PyNeighbor::setMoveBack)
    .def("move", &PyNeighbor::move)
    .def("moveBack", &PyNeighbor::moveBack)
    .def("equals", &PyNeighbor::equals, &PyNeighbor::default_equals)
    .def("index",setIndexWithSol,&PyNeighbor::default_index)
    .def("index",getIndex)
    .def("index",setIndex)
    .def("__lt__",&PyNeighbor::operator<)
    .def("__str__",&PyNeighbor::to_string)
    .def("reassign", &moIndexNeighbor<PyEOT>::operator=,return_internal_reference<>())
    ;

    // moEvaluators();
}


// #include "def_abstract_functor.h"
//
// #include <neighborhood/moNeighbor.h>
// #include <neighborhood/moIndexNeighbor.h>
// #include <neighborhood/moDummyNeighbor.h>
//
// #include <neighborhood/moNeighborhood.h>
// #include <neighborhood/moDummyNeighborhood.h>
//
// #include <explorer/moNeighborhoodExplorer.h>
// #include <algo/moLocalSearch.h>
//
// #include <eoOp.h>
//
// using namespace boost::python;
//
//
// void mo()
// {
//     // def_abstract_functor<moEval<PyNeighbor> >("moEval");
//
//
//     //dummy
//     // class_<moDummyNeighbor<PyMOEO>,bases<PyNeighbor>>
//     // ("moDummyNeighbor",init<>())
//     // .def("move",&moDummyNeighbor<PyMOEO,PyMOEO::Fitness>::move)
//     // ;
// }
