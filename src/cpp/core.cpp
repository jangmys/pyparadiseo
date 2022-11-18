#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/iterable_converter.h>
#include <utils/to_string.h>
#include <utils/to_std_vector.h>
#include <utils/index_error.h>
#include <utils/eoLogger.h>

#include "abstract.h"

#include <es/eoReal.h>
#include <core/moeoVector.h>

#include <apply.h>

#include <pyeot.h>
#include <pso/pypot.h>

#include <eoPop.h>

#include "pickle.h"


namespace bp=boost::python;

// static members, need to be instantiated somewhere
unsigned int moeoObjectiveVectorTraits::nObj;
std::vector < bool > moeoObjectiveVectorTraits::bObj;

bool FitnessTraits::_minimizing = false;

//-------------common-------------
extern void registerConverters();
extern void eo_abstract();
extern void bounds();
extern void int_bounds();
extern void valueParam();

//-------------EO - core-------------
extern void initialize();
extern void evaluate();
extern void random_numbers();

//-------------EO-------------
extern void pop();
extern void geneticOps();
extern void transform();
extern void mergers();
extern void reduce();
extern void selectOne();
extern void PyEOTSelectOne();
extern void selectors();
extern void replacement();
extern void breeders();
extern void eoAlgos();
extern void algos();
extern void continuators();
extern void add_checkpoint();
extern void reduce();
extern void real_op();
extern void bit_op();

//-------------MO-------------
extern void mo();
extern void moEvaluators();
extern void moNeighborhoods();
extern void moExplorers();
extern void moContinuators();
extern void moAlgos();
extern void moComparators();

//-------------MOEO-------------
extern void moeo_abstract();
extern void moeo_algos();

//-------------PSO-------------
extern void eoParticleSwarm();

//TEST
// struct Bar { int x; };
//
//
// template<typename Bar>
// class Foo {
// public:
//     Foo(boost::python::object _op) : op(_op){}
//
//     Bar const& get_bar() { return b; }
//
//     void print_first(const eoPop<Bar>& pop)
//     {
//         std::cout<<"\nfirst\t"<<pop[0]<<std::endl;
//     }
//
//     Bar const& get_first(const eoPop<Bar>& pop)
//     {
//         std::cout<<"\nget_first";
//
//         return pop[0];
//     }
//
//     Bar const& get_first_fun(const eoPop<Bar>& pop)
//     {
//         std::cout<<" >>> "<<boost::python::extract<const Bar&>(op(pop))<<std::endl;
//
//         b = boost::python::extract<Bar>(op(pop));
//
//
//         // return boost::python::extract<const Bar&>(op(pop));
//         // return op(pop);
//
//         //
//         std::cout<<"\ngot\t"<<b<<std::endl;
//         //
//         return b;
//     }
//
//     private:
//         Bar b;
//
//         boost::python::object op;
// };


template<typename T>
std::string vec_to_string(std::vector<T>& _vec)
{
    std::string result;

    result += "[ ";
    for(auto &&v : _vec){
        result += std::to_string(v);
        result += ' ';
    }
    result += "]";

    return result;
}


BOOST_PYTHON_MODULE(_core)
{
    using namespace boost::python;

	scope().attr("__doc__")="pyparadiseo is a metaheuristic optimization framework";

    docstring_options docopt;
    docopt.enable_all();
    docopt.disable_cpp_signatures(); // keep user and py signatures
    // docopt.disable_py_signatures();

    register_exception_translator<index_error>(&translate_index_error);

    Py_Initialize(); //needed to call Python code from c++
    numpy::initialize();

    implicitly_convertible<double, object>();
    implicitly_convertible<int, object>();
    implicitly_convertible<unsigned int, object>();
    implicitly_convertible<bool, object>();

    // implicitly_convertible<std::complex<double>, std::any>();

    registerConverters();//numpy scalar converter!!!
    iterable_converter()
        .from_python<std::vector<bool> >()
        .from_python<std::vector<float> >()
        .from_python<std::vector<double> >()
        .from_python<std::vector<int> >()
        .from_python<std::vector<unsigned int> >()
    ;

    // from MOEO, but in Pyparadiseo the basic solution type PyEOT has an objective vector too
    class_<moeoObjectiveVectorTraits, boost::noncopyable>("ObjectiveVectorTraits",bp::no_init)
        .def("setup", &moeoObjectiveVectorTraits::setup)
            .staticmethod("setup")
        .def("nb_objectives",&moeoObjectiveVectorTraits::nObjectives)
            .staticmethod("nb_objectives")
        .def("minimizing", &moeoObjectiveVectorTraits::minimizing)
            .staticmethod("minimizing")
        .def("maximizing", &moeoObjectiveVectorTraits::maximizing)
            .staticmethod("maximizing")
        .def("tolerance", &moeoObjectiveVectorTraits::tolerance)
            .staticmethod("tolerance")
        ;


    class_<FitnessTraits,boost::noncopyable>("FitnessTraits",bp::no_init)
        .def("set_minimizing",&FitnessTraits::set_minimizing)
            .staticmethod("set_minimizing")
        .def("is_minimizing",&FitnessTraits::is_minimizing)
            .staticmethod("is_minimizing")
    ;

    class_<DoubleFitness<FitnessTraits>>("Fitness",init<optional<double>>())
        .def("setup",&DoubleFitness<FitnessTraits>::setup).staticmethod("setup")
        .def("__lt__",&DoubleFitness<FitnessTraits>::operator<)
    ;

    // need this to be able to derive moeoObjectiveVector from std::vector<double>
    class_< std::vector<double> >("DoubleVec")
        .def(bp::vector_indexing_suite<std::vector<double> >())
        .def("__str__",vec_to_string<double>)
        // .def_pickle(PickleSuite<std::vector<double>>())
        ;
    class_< std::vector<bool> >("BoolVec")
        .def(bp::vector_indexing_suite<std::vector<bool> >())
        .def("__str__",vec_to_string<bool>)
        // .def_pickle(PickleSuite<std::vector<bool>>())
        ;
    class_< std::vector<int> >("IntVec")
        .def(bp::vector_indexing_suite<std::vector<int> >())
        // .def_pickle(PickleSuite<std::vector<int>>())
        ;
    class_< std::vector<std::vector<int>> >("IntVec2")
        .def(bp::vector_indexing_suite<std::vector<std::vector<int>> >())
        // .def_pickle(PickleSuite<std::vector<std::vector<double>>>())
        ;
    class_< std::vector<unsigned int> >("UIntVec")
        .def(bp::vector_indexing_suite<std::vector<unsigned int> >())
        // .def_pickle(PickleSuite<std::vector<unsigned int>>())
        ;
    class_< std::vector<long int> >("LongIntVec")
        .def(bp::vector_indexing_suite<std::vector<long int> >())
        // .def_pickle(PickleSuite<std::vector<long int>>())
        ;


    //only doubles ... need to expose this to use it as a base class for moeoRealObjectiveVector
    //(all member functions will be in moeoRealObjectiveVector)
    class_< moeoObjectiveVector<moeoObjectiveVectorTraits, double>,
        bp::bases< std::vector<double> >
        >("__ObjectiveVector", init<>())
        .def( init< std::vector < double > const& >()[with_custodian_and_ward<1,2>()] )
        .def("setup", &moeoObjectiveVector<moeoObjectiveVectorTraits, double>::setup)
            .staticmethod("setup")
        .def("nObjectives",&moeoObjectiveVector<moeoObjectiveVectorTraits, double>::nObjectives)
            .staticmethod("nObjectives")
        .def("minimizing", &moeoObjectiveVector<moeoObjectiveVectorTraits, double>::minimizing)
            .staticmethod("minimizing")
        .def("maximizing", &moeoObjectiveVector<moeoObjectiveVectorTraits, double>::maximizing)
            .staticmethod("maximizing")
        ;

    typedef moeoRealObjectiveVector<moeoObjectiveVectorTraits> ObjectiveVector;

    //objectiveVector contains doubles ...
    // TODO : assignment objvec = [2,1]
    class_<ObjectiveVector,
        bases< moeoObjectiveVector<moeoObjectiveVectorTraits,double> >
        >("ObjectiveVector",init<optional<double>>())
        .def( init< std::vector < double > const& >()[with_custodian_and_ward<1,2>()] )
        .def( "dominates",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::dominates)
        .def( "__eq__",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::operator==)
        .def( "__lt__",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::operator<)
        .def( "__str__",to_py_str<moeoRealObjectiveVector<moeoObjectiveVectorTraits>>)
        .def("__copy__", &generic__copy__<ObjectiveVector>)
        .def("__deepcopy__", &generic__deepcopy__<ObjectiveVector>)
        .def_pickle(ObjectiveVector_pickle_suite())
        // .def_pickle(PickleSuite<std::vector<double>>())
    ;


    void (PyEO::*fx2)(boost::python::object) = &PyEO::setObjectiveVector;

    class_<PyEO>("_PyEO",init<>())
    .def(init<const PyEO&>())
    .add_property("fitness", &PyEO::getFitness, &PyEO::setFitness,"real")
    .add_property("diversity", &PyEO::getDiversity, &PyEO::setDiversity,"real (for MOEO)")
    .add_property("objectives", &PyEO::getObjectiveVector, fx2, "vector of reals (for MOEO)")
    .def("invalidateObjectiveVector",&PyEO::invalidateObjectiveVector)
    .def("invalidObjectiveVector",&PyEO::invalidObjectiveVector)
    .def("invalidate", &PyEO::invalidate)
    .def("invalid", &PyEO::invalid)
    .def("__lt__", &PyEO::operator<)
    .def("__repr__", &PyEO::repr)
    .def("__str__", &PyEO::to_string)
    .def("__copy__", &generic__copy__<PyEO>)
    .def("__deepcopy__", &generic__deepcopy__<PyEO>)
    .def_readwrite("data", &PyEO::data)
    ;

    class_<PyEOT,bases<PyEO>>("Solution",init<optional<object>>())
        .def(init<const PyEOT&>())
        .add_property("encoding", &PyEOT::get_encoding, &PyEOT::set_encoding,"object, solution encoding")
        .def_pickle(PyEOT_pickle_suite<PyEOT>())
        .def("__getitem__", &PyEOT::get_item, "forward __getitem__ to encoding.\n\n fail if encoding object doesn't provide indexing capabilities")
        .def("__setitem__", &PyEOT::set_item, "see __getitem__")
        .def("__len__", &PyEOT::get_len, "forward __len__ to encoding")
        .def("__str__", &PyEOT::to_string)
        .def("__repr__", &PyEOT::repr)
        .def("__lt__", &PyEOT::operator<)
        .def("__gt__", &PyEOT::operator>)
        .def("__eq__", &PyEOT::operator==)
        .def("__copy__", &generic__copy__<PyEOT>)
        .def("__deepcopy__", &generic__deepcopy__<PyEOT>)

        // .enable_pickling()
    ;

    implicitly_convertible<object, PyEOT>();
    // implicitly_convertible<std::vector<double>, PyEOT>();
    // implicitly_convertible<std::vector<bool>, PyEOT>();

    // implicitly_convertible<PyEOT, object>();
    // class_<Bar>("Bar");
    //
    // class_<Foo<Bar>>("Foo", init<boost::python::object>())
    //     .def("get_bar", &Foo<Bar>::get_bar
    //         , return_value_policy<copy_const_reference>())
    //    ;
    //
    // class_<Foo<PyEOT>>("FooSol", init<boost::python::object>())
    //     .def("get_bar", &Foo<PyEOT>::get_bar, return_value_policy<copy_const_reference>())
    //     .def("print_first", &Foo<PyEOT>::print_first)
    //     .def("get_first", &Foo<PyEOT>::get_first, return_value_policy<copy_const_reference>())
    //     .def("get_first_fun", &Foo<PyEOT>::get_first_fun, return_value_policy<copy_const_reference>())
    // ;

    //=======================================
    // VectorSolution
    //=======================================

    // class_<VectorSolution<double>,bases<PyEO,std::vector<double>>>("RealSolution",init<optional<unsigned int>>())
    // .def(vector_indexing_suite<std::vector<double>>())
    // .def(init<const VectorSolution<double>&>())
    // .def("resize", vec_resize<double>)
    // .def("__repr__", &VectorSolution<double>::repr)
    // .def("__str__", &VectorSolution<double>::to_string)
    // .add_property("array",&VectorSolution<double>::get_array,&VectorSolution<double>::set_array)
    // ;

    class_<RealSolution,bases<PyEO>>("RealSolution",init<optional<unsigned int>>())
    // .def(init<boost::python::object>())
    .def(init<const RealSolution&>())
    .add_property("encoding", &RealSolution::get_encoding, &RealSolution::set_encoding)
    .def("__getitem__", &RealSolution::get_item, "forward __getitem__ to encoding.\n\n fail if encoding object doesn't provide indexing capabilities")
    .def("__setitem__", &RealSolution::set_item, "see __getitem__")
    .def("resize", &RealSolution::resize)
    .def("__len__", &RealSolution::size)
    .def("__repr__", &RealSolution::repr)
    .def("__str__", &RealSolution::to_string)
    .def_readwrite("carray",&RealSolution::vec)
    .add_property("array",&RealSolution::get_encoding,&RealSolution::set_encoding)
    .def_pickle(PyEOT_pickle_suite<RealSolution>())
    ;


    class_<BinarySolution,bases<PyEO>>("BinarySolution",init<optional<unsigned int>>())
    .def(init<const BinarySolution&>())
    .add_property("encoding", &BinarySolution::get_encoding, &BinarySolution::set_encoding)
    .def("__getitem__", &BinarySolution::get_item, "forward __getitem__ to encoding.\n\n fail if encoding object doesn't provide indexing capabilities")
    .def("__setitem__", &BinarySolution::set_item, "see __getitem__")
    .def("resize", &BinarySolution::resize)
    .def("__len__", &BinarySolution::size)
    .def("__repr__", &BinarySolution::repr)
    .def("__str__", &BinarySolution::to_string)
    .def_readwrite("carray",&BinarySolution::vec)
    .add_property("array",&BinarySolution::get_encoding,&BinarySolution::set_encoding)
    .def_pickle(PyEOT_pickle_suite<BinarySolution>())
    ;

    class_<IntSolution,bases<PyEO>>("IntSolution",init<optional<unsigned int>>())
    .def(init<const IntSolution&>())
    .def("resize", &IntSolution::resize)
    .def("__len__", &IntSolution::size)
    .def("__repr__", &IntSolution::repr)
    .def("__str__", &IntSolution::to_string)
    .def_readwrite("carray",&IntSolution::vec)
    .add_property("array",&IntSolution::get_array,&IntSolution::set_array)
    .def_pickle(PyEOT_pickle_suite<IntSolution>())
    ;


    class_<VectorParticle<double>,bases<RealSolution>>("RealParticle",init<optional<unsigned,double,double,double>>())
    .def_readwrite("best_fitness",&VectorParticle<double>::bestFitness)
    .def("__repr__",&VectorParticle<double>::repr)
    .def("__str__",&VectorParticle<double>::to_string)
    ;

    // .def(vector_indexing_suite<std::vector<bool>>())
    // .def(init<const VectorSolution<bool>&>())
    // .def("resize", vec_resize<bool>)
    // .def("__repr__", &VectorSolution<bool>::repr)
    // .def("__str__", &VectorSolution<bool>::to_string)
    // .add_property("array",&VectorSolution<bool>::get_array,&VectorSolution<bool>::set_array)
    // // .def_readwrite("array", &VectorSolution<double>::encoding);
    // ;


    //maybe inheriting from vector is better than using ndarray encodings...
    //(at least for reuse of c++ operators it should be....)
    // class_<eoVector<double,double>,bases<std::vector<double>>>("eoVector",init<unsigned,double>())
    // .def(vector_indexing_suite<std::vector<double>>())
    // ;
    //
    // class_<moeoVector<realObjVec,double>,bases<std::vector<double>>>("moeoVector",init<unsigned,double>())
    // .def(vector_indexing_suite<std::vector<double>>())
    // ;



    //Operators
    // =====================

    eo_abstract();

    //common
    initialize();
    evaluate();
    random_numbers();
    valueParam();

    //EO (evolutionary)
    pop();
    geneticOps();

    mergers();
    reduce();
    transform();

    selectOne();
    PyEOTSelectOne();
    selectors();

    replacement();
    breeders();
    eoAlgos();
    continuators();
    add_checkpoint();

    // algos();
    // reduce();
    bounds();
    int_bounds();
    real_op();
    bit_op();

    //MO (localsearch)
    mo();
    moContinuators(); //BEFORE Evaluators! (moUpdater)
    moEvaluators();
    moNeighborhoods();
    moExplorers();
    moAlgos();
    moComparators();

    //PSO
    eoParticleSwarm();

    //MOEO
    moeo_abstract();
    moeo_algos();


    // def("apply",&apply<PyEOT>);
    // def("apply",&apply<RealSolution>);
    // def("apply",&apply<RealParticle>);


    // fitnessAssign();
    // diversityAssign();
    // moeoreplacement();

    //TODO : set this from python ...
    eo::log << eo::setlevel(eo::warnings);
}

// to avoid having to build with libeo.a
// don't remove this !!!
std::ostream & operator << ( std::ostream& _os, const eoPrintable& _o )
{
    _o.printOn(_os);
    return _os;
};
