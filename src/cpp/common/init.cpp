#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <eoEvalFunc.h>
#include <eoInit.h>
#include <es/eoRealInitBounded.h>
#include <utils/eoRealVectorBounds.h>

#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>
#include <utils/eoRndGenerators.h>

#include <pyeot.h>
#include <pso/pypot.h>

#include <utils/def_abstract_functor.h>

#include <random>

namespace p=boost::python;
namespace np=boost::python::numpy;

/*
Use a Python callable to initialize SolutionType object

Python callable must have 0 input parameter and return Solution encoding (any Python object)
*/
template<typename SolutionType>
class pyeoInit : public eoInit<SolutionType> {
public:
    pyeoInit(p::object _op) :
        eoInit<SolutionType>(),
        init_op(_op)
    {    };

    void
    operator () (SolutionType& _eo)
    {
        _eo.set_encoding(p::call<p::object>(init_op.ptr()));
        _eo.invalidate();
    }

private:
    boost::python::object init_op;
};

/*
Initializer for BinarySolution

Set encoding to random 0/1
*/
struct BinarySolInit : eoInit<BinarySolution> {
    BinarySolInit(unsigned _dim) : eoInit<BinarySolution>(),_dimension(_dim),_gen(eoUniformGenerator<bool>()){ };

    void
    operator () (BinarySolution& _eo)
    {
        _eo.resize(_dimension);

        _eo.encoding = np::from_data(_eo.data(), np::dtype::get_builtin<int>(),
            p::make_tuple(_eo.size()),p::make_tuple(sizeof(int)),
            p::object()
        );

        for(unsigned i=0;i<_dimension;i++){
            _eo[i] = (int)_gen();
        }
        _eo.invalidate();
    }

private:
    unsigned _dimension;
    eoUniformGenerator<bool> _gen;
};

/*
Permutation Initializer

Set encoding to random permutation
*/
struct PermutationInit : eoInit<IntSolution> {

    PermutationInit(unsigned _chromSize, unsigned _startFrom = 0) : chromSize(_chromSize), startFrom(_startFrom){}

    virtual void operator()(IntSolution& _eo)
    {
        _eo.resize(chromSize);

        for(unsigned i=0;i<chromSize;i++){
            _eo[i]=i;
        }

        //the numpy array
        _eo.encoding = np::from_data(_eo.data(), np::dtype::get_builtin<int>(),
            p::make_tuple(_eo.size()),p::make_tuple(sizeof(int)),
            p::object()
        );

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(_eo.begin(),_eo.end(), g);


        _eo.invalidate();
    }

private:
    unsigned chromSize;
    unsigned startFrom;
    UF_random_generator<unsigned int> gen;
};


//TODO : merge with unbounded (FIxedLength)
template<class T>
class RealInitBounded : public eoInit<T>{
public:
    /** Ctor - from eoRealVectorBounds */
    RealInitBounded(eoRealVectorBounds & _bounds):bounds(_bounds)
    {
        if (!bounds.isBounded())
            throw eoException("Needs bounded bounds to initialize a std::vector<double>");
    }

    /** simply passes the argument to the uniform method of the bounds */
    virtual void operator()(T& _eo)
    {
        _eo.resize(bounds.size());

        _eo.encoding = np::from_data(_eo.data(), np::dtype::get_builtin<double>(),
            p::make_tuple(_eo.size()),p::make_tuple(sizeof(double)),
            p::object()
        );

        for(unsigned i=0;i<_eo.size();i++)
        {
            _eo[i] = bounds.uniform(i);
        }
    }
private:
    eoRealVectorBounds & bounds;
};

// void operator_wrap(eoRealVectorBounds& b, PyEOT& _eo)
// {
//     // get ndarray from object
//     np::ndarray e1 = np::from_object(_eo.encoding, np::dtype::get_builtin<double>());
//
//     //get std::vector from ndarray
//     std::vector<double> ind1(reinterpret_cast<double*>(e1.get_data()),
//         reinterpret_cast<double*>(e1.get_data())+e1.shape(0));
//
//     b.uniform(ind1);
// }

template<class T>
void export_pyinit(std::string name){
    using namespace boost::python;

    class_<pyeoInit<T>, bases<eoInit<T>>>
        (make_name("pyeoInit",name).c_str(), init<boost::python::object>())
    .def("__call__", &pyeoInit<T>::operator())
    ;
}



void
initialize()
{
    using namespace boost::python;

    export_pyinit<PyEOT>("");
    export_pyinit<BinarySolution>("Bin");
    export_pyinit<RealSolution>("Real");

    class_<BinarySolInit, bases<eoInit<BinarySolution>>>
    ("BinaryInit", init<unsigned>())
    .def("__call__", &BinarySolInit::operator ())
    ;

    class_<PermutationInit, bases<eoInit<IntSolution>>>
    ("PermutationInit", init<unsigned,optional<unsigned>>())
    .def("__call__", &PermutationInit::operator ())
    ;

    class_<RealInitBounded<RealSolution>, bases<eoInit<RealSolution>>>
        ("RealBoundedInit",
        init<eoRealVectorBounds&>()
        [
        with_custodian_and_ward<1,2>()
        ]
    )
    .def("__call__", &RealInitBounded<RealSolution>::operator ())
    ;

    class_<RealInitBounded<RealParticle>, bases<eoInit<RealParticle>>>
        ("RealBoundedParticleInit",
        init<eoRealVectorBounds&>()
        [
        with_custodian_and_ward<1,2>()
        ]
    )
    .def("__call__", &RealInitBounded<RealParticle>::operator ())
    ;
}
