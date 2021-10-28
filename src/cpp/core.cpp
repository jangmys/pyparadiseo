#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/iterable_converter.h>
#include <utils/to_string.h>
#include <utils/to_std_vector.h>
#include <utils/index_error.h>

#include <pyeot.h>

namespace bp=boost::python;

// static members, need to be instantiated somewhere
unsigned int moeoObjectiveVectorTraits::nObj;
std::vector < bool > moeoObjectiveVectorTraits::bObj;


//TODO: __copy__ OK, error when trying to __deepcopy__
struct PyEOT_pickle_suite : bp::pickle_suite
{
    typedef double Fitness;
    typedef double Diversity;
    typedef realObjVec ObjectiveVector;

    static bp::tuple getstate(bp::object _eot)
    {
        PyEOT const& p = bp::extract<PyEOT const&>(_eot)();

        return bp::make_tuple(
                p.invalidObjectiveVector()?bp::object():bp::object(p.objectiveVector()),
                p.invalidFitness()?bp::object():bp::object(p.fitness()),
                p.invalidDiversity()?bp::object():bp::object(p.diversity()),
                p.encoding);
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
        p.setFitness( state[1] );
        p.setDiversity( state[2] ); //checks for None

        if(bp::object(state[3]).ptr() != Py_None)
        {
            p.encoding = state[3];
        }else{
            p.encoding = bp::object();
        }
    }
};


extern void registerConverters();

//EO - core
extern void initialize();
extern void evaluate();
extern void random_numbers();

//EO
extern void pop();
extern void geneticOps();
extern void transform();

extern void selectOne();
extern void PyEOTSelectOne();
extern void selectors();

extern void replacement();
extern void breeders();
extern void eoAlgos();

extern void algos();
extern void continuators();
extern void reduce();

//MO
extern void mo();
extern void moEvaluators();
extern void moNeighborhoods();
extern void moExplorers();
extern void moContinuators();
extern void moAlgos();

BOOST_PYTHON_MODULE(_core)
{
    using namespace boost::python;

    register_exception_translator<index_error>(&translate_index_error);

    Py_Initialize(); //needed to call Python code from c++
    numpy::initialize();

    registerConverters();//numpy scalar converter!!!
    iterable_converter()
        .from_python<std::vector<bool> >()
        .from_python<std::vector<float> >()
        .from_python<std::vector<double> >()
        .from_python<std::vector<int> >()
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

    // need this to be able to derive moeoObjectiveVector from std::vector<double>
    class_< std::vector<double> >("DoubleVec")
        .def(bp::vector_indexing_suite<std::vector<double> >())
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
        >("ObjectiveVector",init<>())
        .def( init< std::vector < double > const& >()[with_custodian_and_ward<1,2>()] )
        .def( "dominates",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::dominates)
        .def( "__eq__",&moeoRealObjectiveVector<moeoObjectiveVectorTraits>::operator==)
        .def( "__str__",to_py_str<moeoRealObjectiveVector<moeoObjectiveVectorTraits>>)
        ;

    void (PyEOT::*fx2)(boost::python::object) = &PyEOT::setObjectiveVector;

    class_<PyEOT>("PyEOT",init<>())
        .def(init<const PyEOT&>())
        .add_property("encoding", &PyEOT::getEncoding, &PyEOT::setEncoding)
        .add_property("fitness", &PyEOT::getFitness, &PyEOT::setFitness)
        .add_property("objectiveVector", &PyEOT::getObjectiveVector, fx2)
        .add_property("diversity", &PyEOT::getDiversity, &PyEOT::setDiversity)
        .def("invalidateObjectiveVector",&PyEOT::invalidateObjectiveVector)
        .def("invalidObjectiveVector",&PyEOT::invalidObjectiveVector)
        .def("invalidate", &PyEOT::invalidate)
        // .def("invalid", &PyEOT::invalid) //identical to invalidateObjectiveVector
        .def("__getitem__", &PyEOT::get_item)
        .def("__setitem__", &PyEOT::set_item)
        .def("__str__", &PyEOT::to_string)
        .def("__len__", &PyEOT::get_len)
        .def_pickle(PyEOT_pickle_suite())
    ;
        // .add_property("objectiveVector", &PyMOEO::getObjectiveVector, &PyMOEO::setObjectiveVector)
        // .def("__lt__", &PyMOEO::operator<)
        // .def("__gt__", &PyMOEO::operator>)
        // .def("__eq__", &PyMOEO::operator==)
        // ;



    //common
    initialize();
    evaluate();
    random_numbers();

    //EO (evolutionary)
    pop();
    geneticOps();
    transform();

    selectOne();
    PyEOTSelectOne();
    selectors();

    replacement();
    breeders();
    eoAlgos();
    continuators();
    // algos();
    // reduce();

    //MO (localsearch)
    mo();
    moEvaluators();
    moNeighborhoods();
    moExplorers();
    moContinuators();
    moAlgos();

    //MOEO
    // fitnessAssign();
    // diversityAssign();
    // moeoreplacement();
}

// to avoid having to build with libeo.a
// don't remove this !!!
std::ostream & operator << ( std::ostream& _os, const eoPrintable& _o )
{
    _o.printOn(_os);
    return _os;
};
