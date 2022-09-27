#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <pickle.h>
#include <pyeot.h>
#include <pso/pypot.h>

#include <eoPop.h>

namespace bp=boost::python;

template<class SolutionType>
std::string pop_to_string(eoPop<SolutionType>& pop)
{
    std::string result;

    for(auto &i: pop){
        result += i.to_string();
        result += "\n";
    }
    return result;
}

template<class SolutionType>
int pop_size(eoPop<SolutionType>& pop) {
    return pop.size();
}

template<class SolutionType>
void pop_shuffle(eoPop<SolutionType>& pop){
    pop.shuffle();
}

template<class SolutionType>
SolutionType& pop_getitem(eoPop<SolutionType>& pop, bp::object key)
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


template<class SolutionType>
void pop_setitem(eoPop<SolutionType>& pop, bp::object key, SolutionType& value)
{
    bp::extract<int> x(key);
    if (!x.check())
        throw index_error("Population index not convertible to int");

    int i = x();
    if (static_cast<unsigned>(i) >= pop.size()){
        throw index_error("Index out of bounds");
    }

    pop[i] = value;
}

template<class SolutionType>
void pop_merge(eoPop<SolutionType>& pop, eoPop<SolutionType>& pop2){
    for(auto&& ind : pop2)
        pop.push_back(ind);
}

template<class SolutionType>
void pop_push_back(eoPop<SolutionType>& pop, SolutionType& p){
    pop.push_back(p);
}

template<class SolutionType>
void pop_resize(eoPop<SolutionType>& pop, unsigned i) {
    pop.resize(i);
}

template<class SolutionType>
struct pyPop_pickle_suite : boost::python::pickle_suite
{
    static boost::python::tuple getstate(const eoPop<SolutionType>& _pop)
    {
        boost::python::list entries;
        for (unsigned i = 0; i != _pop.size(); ++i)
            entries.append( PyEOT_pickle_suite<SolutionType>::getstate(_pop[i]) );

        return boost::python::make_tuple(boost::python::object(_pop.size()), entries);
    }

    static void setstate( eoPop<SolutionType>& _pop, boost::python::tuple pickled)
    {
        int sz = boost::python::extract<int>(pickled[0]);
        boost::python::list entries = boost::python::list(pickled[1]);
        _pop.resize(sz);
        for (unsigned i = 0; i != _pop.size(); ++i)
            {
                PyEOT_pickle_suite<SolutionType>::setstate(_pop[i], boost::python::tuple(entries[i]) );
            }
    }
};

template<class SolutionType>
struct SolVec_pickle_suite : bp::pickle_suite
{
    static boost::python::tuple getstate(const std::vector<SolutionType>& _vec)
    {
        boost::python::list entries;
        for (unsigned i = 0; i != _vec.size(); ++i)
            entries.append( PyEOT_pickle_suite<SolutionType>::getstate(_vec[i]) );

        return boost::python::make_tuple(boost::python::object(_vec.size()), entries);
    }

    static void setstate( std::vector<SolutionType>& _vec, boost::python::tuple pickled)
    {
        int sz = boost::python::extract<int>(pickled[0]);
        boost::python::list entries = boost::python::list(pickled[1]);
        _vec.resize(sz);
        for (unsigned i = 0; i != _vec.size(); ++i)
            {
                PyEOT_pickle_suite<SolutionType>::setstate(_vec[i], boost::python::tuple(entries[i]) );
            }
    }
};


// .def_pickle(ObjectiveVector_pickle_suite())



BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(pop_sort_overload, sort, 0, 1)

template<class SolutionType>
void expose_pop(std::string name)
{
    using namespace boost::python;

    class_<std::vector<SolutionType>>(("_SolutionVector"+name).c_str())
    .def(vector_indexing_suite<std::vector<SolutionType>>())
    .def_pickle(SolVec_pickle_suite<SolutionType>())
    ;

    class_<eoPop<SolutionType>,bases<std::vector<SolutionType>>>
    ((name+"Pop").c_str(),"A vector of Solutions.",init<>() )
    .def( init< unsigned, eoInit<SolutionType>& >()[with_custodian_and_ward<1,3>()] )
    .def("append", &eoPop<SolutionType>::append,
        "Append initialized individuals at the end of Pop"
        "\n\n"
        "new pop_size should larger than current pop_size"
    )
    .def("__str__", pop_to_string<SolutionType>)
    .def("sort", static_cast<void (eoPop<SolutionType>::*)(void)>(&eoPop<SolutionType>::sort))
    .def("sort", static_cast<void (eoPop<SolutionType>::*)(std::vector<const SolutionType*>&) const >(&eoPop<SolutionType>::sort))
    .def("shuffle", pop_shuffle<SolutionType>)
    .def("best", &eoPop<SolutionType>::best_element, return_internal_reference<>(),
        "Return best individual"
    )
    .def("worst", &eoPop<SolutionType>::worse_element, return_internal_reference<>(),
        "Return worst individual"
    )
    .def("push_back", pop_push_back<SolutionType>)
    .def("merge", pop_merge<SolutionType>)
    .def("resize",    pop_resize<SolutionType>)
    .def("swap", &eoPop<SolutionType>::swap)
    .def_pickle(pyPop_pickle_suite<SolutionType>())
    ;
}



void pop()
{
    //EO/MOEO solutions
    expose_pop<PyEOT>("");
    expose_pop<RealSolution>("Real");
    expose_pop<BinarySolution>("Binary");
    expose_pop<IntSolution>("Int");
    //Particles
    expose_pop<RealParticle>("RealParticle");

    
}
