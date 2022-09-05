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

namespace bp=boost::python;

// static members, need to be instantiated somewhere
unsigned int moeoObjectiveVectorTraits::nObj;
std::vector < bool > moeoObjectiveVectorTraits::bObj;

bool FitnessTraits::_minimizing = false;

//TODO: __copy__ OK, error when trying to __deepcopy__
struct PyEOT_pickle_suite : bp::pickle_suite
{
    typedef doubleFitness Fitness;
    typedef double Diversity;
    typedef realObjVec ObjectiveVector;

    static bp::tuple getstate(bp::object _eot)
    {
        PyEOT const& p = bp::extract<PyEOT const&>(_eot)();

        return bp::make_tuple(
                p.invalidObjectiveVector()?bp::object():bp::object(p.objectiveVector()),
                p.invalidFitness()?bp::object():bp::object(p.fitness().get()),
                p.invalidDiversity()?bp::object():bp::object(p.diversity()),
                p.get_encoding());
    }

    static void setstate(bp::object _eot, bp::tuple state)
    {
        using namespace boost::python;
        PyEOT& p = bp::extract<PyEOT&>(_eot);

        //https://stackoverflow.com/questions/22674774/get-single-element-from-a-boostpythonobject-list-for-use-in-python-routine
        /*
        The boost::python::object's operator[] returns a boost::python::proxy object. While the proxy class has an implicit conversion to boost::python::object, there are many areas in the API where an explicit conversion is required.

        Explicitly constructing a boost::python::object from the proxy should resolve the conversion exception:
        */
        p.setObjectiveVector( bp::object(state[0]) );
        p.setFitness( bp::object(state[1]) );
        p.setDiversity( state[2] ); //checks for None

        if(bp::object(state[3]).ptr() != Py_None)
        {
            p.set_encoding(state[3]);
        }else{
            p.set_encoding(bp::object())
            ;
        }
    }
};


extern void registerConverters();

extern void eo_abstract();

// extern template void export_abstract<PyEOT>(std::string s);
// extern template void export_abstract<RealSolution>(std::string s);
// extern template void export_abstract<BinarySolution>(std::string s);

extern void bounds();

//common
extern void valueParam();

//EO - core
extern void initialize();
extern void evaluate();
extern void random_numbers();



//EO
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

//MO
extern void mo();
extern void moEvaluators();
extern void moNeighborhoods();
extern void moExplorers();
extern void moContinuators();
extern void moAlgos();
extern void moComparators();

//MOEO
extern void moeo_abstract();
extern void moeo_algos();


extern void eoParticleSwarm();





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
        ;
    class_< std::vector<bool> >("BoolVec")
        .def(bp::vector_indexing_suite<std::vector<bool> >())
        ;
    class_< std::vector<int> >("IntVec")
        .def(bp::vector_indexing_suite<std::vector<int> >())
        ;
    class_< std::vector<std::vector<int>> >("IntVec2")
        .def(bp::vector_indexing_suite<std::vector<std::vector<int>> >())
        ;

    class_< std::vector<unsigned int> >("UIntVec")
        .def(bp::vector_indexing_suite<std::vector<unsigned int> >())
        ;
    class_< std::vector<long int> >("LongIntVec")
        .def(bp::vector_indexing_suite<std::vector<long int> >())
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

    //objectiveVector contains doubles ...
    // TODO : assignment objvec = [2,1]
    class_< moeoRealObjectiveVector<moeoObjectiveVectorTraits>,
        bases< moeoObjectiveVector<moeoObjectiveVectorTraits,double> >
        >("ObjectiveVector",init<optional<double>>())
        .def( init< std::vector < double > const& >()[with_custodian_and_ward<1,2>()] )
        .def( "dominates",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::dominates)
        .def( "__eq__",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::operator==)
        .def( "__lt__",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::operator<)
        .def( "__str__",to_py_str<moeoRealObjectiveVector<moeoObjectiveVectorTraits>>)
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
    ;

    class_<PyEOT,bases<PyEO>>("Solution",init<optional<object>>())
        .def(init<const PyEOT&>())
        .add_property("encoding", &PyEOT::get_encoding, &PyEOT::set_encoding,"object, solution encoding")
        .def("__getitem__", &PyEOT::get_item, "forward __getitem__ to encoding.\n\n fail if encoding object doesn't provide indexing capabilities")
        .def("__setitem__", &PyEOT::set_item, "see __getitem__")
        .def("__len__", &PyEOT::get_len, "forward __len__ to encoding")
        .def("__str__", &PyEOT::to_string)
        .def("__repr__", &PyEOT::repr)
        .def("__lt__", &PyEOT::operator<)
        .def("__gt__", &PyEOT::operator>)
        .def("__eq__", &PyEOT::operator==)
        .def_pickle(PyEOT_pickle_suite())
    ;

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
    .def("resize", &RealSolution::resize)
    .def("__len__", &RealSolution::size)
    .def("__repr__", &RealSolution::repr)
    .def("__str__", &RealSolution::to_string)
    .def_readwrite("carray",&RealSolution::vec)
    .add_property("array",&RealSolution::get_encoding,&RealSolution::set_encoding)
    ;


    class_<BinarySolution,bases<PyEO>>("BinarySolution",init<optional<unsigned int>>())
    .def(init<const BinarySolution&>())
    .add_property("encoding", &BinarySolution::get_encoding, &BinarySolution::set_encoding)
    .def("resize", &BinarySolution::resize)
    .def("__len__", &BinarySolution::size)
    .def("__repr__", &BinarySolution::repr)
    .def("__str__", &BinarySolution::to_string)
    .def_readwrite("carray",&BinarySolution::vec)
    .add_property("array",&BinarySolution::get_encoding,&BinarySolution::set_encoding)
    // .add_property("array",&BinarySolution::get_array,&BinarySolution::set_array)
    ;

    class_<IntSolution,bases<PyEO>>("IntSolution",init<optional<unsigned int>>())
    .def(init<const IntSolution&>())
    .def("resize", &IntSolution::resize)
    .def("__len__", &IntSolution::size)
    .def("__repr__", &IntSolution::repr)
    .def("__str__", &IntSolution::to_string)
    .def_readwrite("carray",&IntSolution::vec)
    .add_property("array",&IntSolution::get_array,&IntSolution::set_array)
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
