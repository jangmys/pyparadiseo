#include <pyeot.h>
#include "pymo.h"

#include <comparator/moComparator.h>
#include <comparator/moNeighborComparator.h>
#include <comparator/moSolNeighborComparator.h>
#include <comparator/moSolComparator.h>

using namespace boost::python;


template<typename SolutionType>
struct moNeighborComparatorWrap : moNeighborComparator<PyNeighbor<SolutionType>>
{
    typedef PyNeighbor<SolutionType> NborT;

    moNeighborComparatorWrap() : moNeighborComparator<NborT>(){};

    moNeighborComparatorWrap(boost::python::object obj) : moNeighborComparator<NborT>(),cmp_op(obj){};

    //"virtual with default"
    bool operator()(const NborT& _neighbor1, const NborT& _neighbor2)
    {
        if(cmp_op.ptr() != Py_None)
        {
            // std::cout<<"call this\n";
            return boost::python::call<bool>(cmp_op.ptr(),_neighbor1.fitness(),_neighbor2.fitness());
        }else{
            // std::cout<<"default\n";
            return this->moNeighborComparator<NborT>::operator()(_neighbor1,_neighbor2);
        }
    }

    void setNborComp(boost::python::object obj)
    {
        cmp_op = obj;
    }

private:
    boost::python::object cmp_op;
};

template<typename SolutionType>
struct moSolNeighborComparatorWrap : moSolNeighborComparator<PyNeighbor<SolutionType>>
{
    typedef PyNeighbor<SolutionType> NborT;

    moSolNeighborComparatorWrap() : moSolNeighborComparator<NborT>(){};

    moSolNeighborComparatorWrap(boost::python::object obj) : moSolNeighborComparator<NborT>(),cmp_op(obj){};

    //"virtual with default"
    bool operator()(const PyEOT& _sol, const NborT& _neighbor)
    {
        if(cmp_op.ptr() != Py_None)
        {
            // std::cout<<"call this\n";
            return boost::python::call<bool>(cmp_op.ptr(),_sol.fitness(),_neighbor.fitness());
        }else{
            // std::cout<<"default\n";
            return this->moSolNeighborComparator<NborT>::operator()(_sol,_neighbor);
        }
    }

    void setSolNborComp(boost::python::object obj)
    {
        cmp_op = obj;
    }

private:
    boost::python::object cmp_op;
};

template<typename SolutionType>
void expose_moComparators()
{
    typedef PyNeighbor<SolutionType> NborT;

    //need to expose the base class
    class_<moNeighborComparator<NborT>,boost::noncopyable>("moNCompBase",init<>());

    class_<moNeighborComparatorWrap<SolutionType>,bases<moNeighborComparator<NborT>>, boost::noncopyable>("moNeighborComparator",init<>())
    .def(init<boost::python::object>())
    .def("__call__",&moNeighborComparatorWrap<SolutionType>::operator())
    .def("setNborComp",&moNeighborComparatorWrap<SolutionType>::setNborComp)
    ;

    class_<moSolNeighborComparator<NborT>,boost::noncopyable>("moSNCompBase",init<>());

    class_<moSolNeighborComparatorWrap<SolutionType>,bases<moSolNeighborComparator<NborT>>, boost::noncopyable>("moSolNeighborComparator",init<>())
    .def(init<boost::python::object>())
    .def("__call__",&moSolNeighborComparatorWrap<SolutionType>::operator())
    .def("setSolNborComp",&moSolNeighborComparatorWrap<SolutionType>::setSolNborComp)
    ;
}


void moComparators()
{
    expose_moComparators<PyEOT>();
}
