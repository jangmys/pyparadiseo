#include "pymo.h"

#include <utils/def_abstract_functor.h>

#include <boost/python.hpp>


template<typename SolutionType>
void expose_nbor(std::string name)
{
    using namespace boost::python;

    //disambiguate getter/setter
    unsigned int (PyNeighbor<SolutionType>::*_getIndex)(void) const = &PyNeighbor<SolutionType>::index;
    void (PyNeighbor<SolutionType>::*_setIndex)(unsigned int) = &PyNeighbor<SolutionType>::index;
    void (PyNeighbor<SolutionType>::*_setIndexWithSol)(SolutionType&,unsigned int) = &PyNeighbor<SolutionType>::index;

    class_<PyNeighbor<SolutionType>,bases<SolutionType>>(
        make_name("Neighbor",name).c_str(),
        init<>())
        .def("setMove", &PyNeighbor<SolutionType>::setMove)
        .def("setMoveBack", &PyNeighbor<SolutionType>::setMoveBack)
        .def("move", &PyNeighbor<SolutionType>::move)
        .def("moveBack", &PyNeighbor<SolutionType>::moveBack)
        .def("equals", &PyNeighbor<SolutionType>::equals)
        .def("index",_setIndexWithSol)
        .def("index",_getIndex)
        .def("index",_setIndex)
        .def("reassign", &PyNeighbor<SolutionType>::operator=,return_internal_reference<>())
        ;
}

void mo()
{
    using namespace boost::python;

    expose_nbor<PyEOT>("");
    expose_nbor<BinarySolution>("Bin");
    expose_nbor<RealSolution>("Real");
    expose_nbor<IntSolution>("Perm");

    class_<BinNeighbor,bases<PyNeighbor<BinarySolution>>>(
        "BinNeighbor",
        init<>()
    )
    .def("move", &BinNeighbor::move)
    .def("move_back", &BinNeighbor::moveBack)
    ;
}
