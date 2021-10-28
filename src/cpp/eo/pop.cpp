#include <boost/python.hpp>
#include <pyeot.h>

#include <eoPop.h>

namespace bp=boost::python;

std::string pop_to_string(eoPop<PyEOT>& pop)
{
    std::string result;

    for(auto &i: pop){
        result += i.to_string();
        result += "\n";
    }
    return result;
}

int pop_size(eoPop<PyEOT>& pop) {
    return pop.size();
}

void pop_sort(eoPop<PyEOT>& pop) {
    pop.sort();
}

void pop_shuffle(eoPop<PyEOT>& pop){
    pop.shuffle();
}

PyEOT& pop_getitem(eoPop<PyEOT>& pop, bp::object key)
{
    bp::extract<int> x(key);
    if (!x.check())
        throw index_error("Slicing not allowed");

    int i = x();

    if (static_cast<unsigned>(i) >= pop.size()){
        throw index_error("Index out of bounds");
    }
    return pop[i];
}

void pop_setitem(eoPop<PyEOT>& pop, bp::object key, PyEOT& value)
{
    bp::extract<int> x(key);
    if (!x.check())
        throw index_error("Slicing not allowed");

    int i = x();

    if (static_cast<unsigned>(i) >= pop.size()){
        throw index_error("Index out of bounds");
    }

    pop[i] = value;
}

void pop_push_back(eoPop<PyEOT>& pop, PyEOT& p){
    pop.push_back(p);
}

void pop_resize(eoPop<PyEOT>& pop, unsigned i) {
    pop.resize(i);
}




void pop()
{
    using namespace boost::python;

    class_<eoPop<PyEOT>>("Pop", init<>() )
    .def( init< unsigned, eoInit<PyEOT>& >()[with_custodian_and_ward<1,3>()] )
    //     // .enable_pickling()
    .def("append", &eoPop<PyEOT>::append, "2 parameters : (new popsize,eoInit)")
    // .def("__str__", to_string<eoPop<PyEOT> >)
    // .def("__str__", pop_to_string)
    .def("__str__", pop_to_string)
    .def("__len__", pop_size)
    .def("sort",    pop_sort )
    .def("shuffle", pop_shuffle)
    .def("__getitem__", pop_getitem, return_internal_reference<>() )
    .def("__setitem__", pop_setitem)
    .def("best", &eoPop<PyEOT>::best_element, return_internal_reference<>() )
    .def("push_back", pop_push_back)
    .def("resize",    pop_resize)
    .def("swap", &eoPop<PyEOT>::swap)
    // .def_pickle(pyPop_pickle_suite())
    ;
}
