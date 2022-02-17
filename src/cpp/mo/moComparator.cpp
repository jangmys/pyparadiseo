#include <pyeot.h>
#include "pymo.h"

#include <comparator/moComparator.h>
#include <comparator/moNeighborComparator.h>
#include <comparator/moSolNeighborComparator.h>
#include <comparator/moSolComparator.h>

using namespace boost::python;


struct moNeighborComparatorWrap : moNeighborComparator<PyNeighbor>
{
    moNeighborComparatorWrap() : moNeighborComparator<PyNeighbor>(){};

    moNeighborComparatorWrap(boost::python::object obj) : moNeighborComparator<PyNeighbor>(),cmp_op(obj){};

    //"virtual with default"
    bool operator()(const PyNeighbor& _neighbor1, const PyNeighbor& _neighbor2)
    {
        if(cmp_op.ptr() != Py_None)
        {
            // std::cout<<"call this\n";
            return boost::python::call<bool>(cmp_op.ptr(),_neighbor1.fitness(),_neighbor2.fitness());
        }else{
            // std::cout<<"default\n";
            return this->moNeighborComparator<PyNeighbor>::operator()(_neighbor1,_neighbor2);
        }
    }

    void setNborComp(boost::python::object obj)
    {
        cmp_op = obj;
    }

private:
    boost::python::object cmp_op;
};


struct moSolNeighborComparatorWrap : moSolNeighborComparator<PyNeighbor>
{
    moSolNeighborComparatorWrap() : moSolNeighborComparator<PyNeighbor>(){};

    moSolNeighborComparatorWrap(boost::python::object obj) : moSolNeighborComparator<PyNeighbor>(),cmp_op(obj){};

    //"virtual with default"
    bool operator()(const PyEOT& _sol, const PyNeighbor& _neighbor)
    {
        if(cmp_op.ptr() != Py_None)
        {
            // std::cout<<"call this\n";
            return boost::python::call<bool>(cmp_op.ptr(),_sol.fitness(),_neighbor.fitness());
        }else{
            // std::cout<<"default\n";
            return this->moSolNeighborComparator<PyNeighbor>::operator()(_sol,_neighbor);
        }
    }

    void setSolNborComp(boost::python::object obj)
    {
        cmp_op = obj;
    }

private:
    boost::python::object cmp_op;
};


void moComparators()
{
    //need to expose the base class
    class_<moNeighborComparator<PyNeighbor>,boost::noncopyable>("moNCompBase",init<>());

    class_<moNeighborComparatorWrap,bases<moNeighborComparator<PyNeighbor>>, boost::noncopyable>("moNeighborComparator",init<>())
    .def(init<boost::python::object>())
    .def("__call__",&moNeighborComparatorWrap::operator())
    .def("setNborComp",&moNeighborComparatorWrap::setNborComp)
    ;

    class_<moSolNeighborComparator<PyNeighbor>,boost::noncopyable>("moSNCompBase",init<>());

    class_<moSolNeighborComparatorWrap,bases<moSolNeighborComparator<PyNeighbor>>, boost::noncopyable>("moSolNeighborComparator",init<>())
    .def(init<boost::python::object>())
    .def("__call__",&moSolNeighborComparatorWrap::operator())
    .def("setSolNborComp",&moSolNeighborComparatorWrap::setSolNborComp)
    ;
}
