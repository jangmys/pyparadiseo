#include "pymo.h"

#include <utils/def_abstract_functor.h>

#include <boost/python.hpp>

template<typename SolutionType>
void expose_mo(std::string name)
{
    using namespace boost::python;

    //disambiguate getter/setter
    unsigned int (PyNeighbor<SolutionType>::*getIndex)(void) const = &moIndexNeighbor<SolutionType>::index;
    void (PyNeighbor<SolutionType>::*setIndex)(unsigned int) = &moIndexNeighbor<SolutionType>::index;
    void (PyNeighbor<SolutionType>::*setIndexWithSol)(SolutionType&,unsigned int) = &moIndexNeighbor<SolutionType>::index;

    class_<PyNeighbor<SolutionType>,boost::noncopyable>(
        make_name("Neighbor",name).c_str(),
        init<>())
    .def(init<
        PyNeighbor<SolutionType>&
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
    .add_property("fitness",&PyNeighbor<SolutionType>::getFitness,&PyNeighbor<SolutionType>::setFitness)
    .def("setMove", &PyNeighbor<SolutionType>::setMove)
    .def("setMoveBack", &PyNeighbor<SolutionType>::setMoveBack)
    .def("move", &PyNeighbor<SolutionType>::move)
    .def("moveBack", &PyNeighbor<SolutionType>::moveBack)
    .def("equals", &PyNeighbor<SolutionType>::equals, &PyNeighbor<SolutionType>::default_equals)
    .def("index",setIndexWithSol)
    .def("index",getIndex)
    .def("index",setIndex)
    .def("__lt__",&PyNeighbor<SolutionType>::operator<)
    .def("__str__",&PyNeighbor<SolutionType>::to_string)
    .def("reassign", &moIndexNeighbor<SolutionType>::operator=,return_internal_reference<>())
    ;

    // moEvaluators();
}

void mo()
{
    expose_mo<PyEOT>("");
    expose_mo<BinarySolution>("Bin");
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
