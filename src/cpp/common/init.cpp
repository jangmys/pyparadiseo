#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoRealInitBounded.h>
#include <utils/eoRealVectorBounds.h>

#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>
#include <utils/eoRndGenerators.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <random>

namespace p=boost::python;
namespace np=boost::python::numpy;


struct pyeoInit : eoInit<PyEOT> {
    pyeoInit() : eoInit<PyEOT>(){ };

    pyeoInit(p::object _op) :
        eoInit<PyEOT>(),
        init_op(_op)
    {
    };

    void
    setGenerator(p::object obj)
    {
        std::cout << "Setting generator\n";
        init_op = obj;
    }

    void
    operator () (PyEOT& _eo)
    {
        if (init_op.ptr() != Py_None) {
            // std::cout << "*** init" << std::endl;
            _eo.setEncoding(p::call<p::object>(init_op.ptr()));
            _eo.invalidate();
        } else   {
            std::cout << "no move defined : do nothing";
        }
    }

private:
    boost::python::object init_op;
};

template<typename T>
struct FixedLengthInit : eoInit<PyEOT> {
    FixedLengthInit(unsigned _dim) : eoInit<PyEOT>(),_dimension(_dim),_gen(eoUniformGenerator<T>()){ };

    void
    operator () (PyEOT& _eo)
    {
        _eo.encoding = np::zeros(p::make_tuple(_dimension),np::dtype::get_builtin<T>());

        for(unsigned i=0;i<_dimension;i++){
            _eo.encoding[i] = _gen();
        }
        _eo.invalidate();
    }

private:
    unsigned _dimension;
    eoUniformGenerator<T> _gen;
};

//TODO : merge with unbounded (FIxedLength)
class pyRealInitBounded : public eoInit<PyEOT>{
public:
    /** Ctor - from eoRealVectorBounds */
    pyRealInitBounded(eoRealVectorBounds & _bounds):bounds(_bounds)
    {
        if (!bounds.isBounded())
            throw eoException("Needs bounded bounds to initialize a std::vector<double>");
    }

    /** simply passes the argument to the uniform method of the bounds */
    virtual void operator()(PyEOT& _eo)
    {
        _eo.encoding = np::zeros(p::make_tuple(bounds.size()),np::dtype::get_builtin<double>());

        // get ndarray from object
        np::ndarray arr = np::from_object(_eo.encoding, np::dtype::get_builtin<double>());

        for(unsigned i=0;i<arr.shape(0);i++)
        {
            arr[i] = bounds.uniform(i);
        }

        _eo.invalidate();
    }
private:
    eoRealVectorBounds & bounds;
};


class pyInitPermutation : public eoInit<PyEOT>{
public:
    pyInitPermutation(unsigned _chromSize, unsigned _startFrom = 0) : chromSize(_chromSize), startFrom(_startFrom){}

    virtual void operator()(PyEOT& _eo)
    {
        _eo.encoding = np::zeros(p::make_tuple(chromSize),np::dtype::get_builtin<int>());

        np::ndarray arr = np::from_object(_eo.encoding, np::dtype::get_builtin<int>());

        int* ptr = reinterpret_cast<int*>(arr.get_data());
        std::vector<int> vec(ptr, ptr + arr.shape(0));

        for(unsigned i=0;i<arr.shape(0);i++)
        {
            vec[i] = startFrom + i;
        }

        std::random_device rd;
        std::mt19937 g(rd());

        std::shuffle(vec.begin(),vec.end(), g);
        std::copy(vec.begin(), vec.end(), ptr);

        _eo.invalidate();
    }

private:
    unsigned chromSize;
    unsigned startFrom;
    UF_random_generator<unsigned int> gen;
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



void
initialize()
{
    using namespace boost::python;

    // eoUF : PyMOEO ---> void
    def_abstract_functor<eoInit<PyEOT> >("eoInit","docstring");

    class_<pyeoInit, bases<eoInit<PyEOT> >, boost::noncopyable>
        ("pyeoInit", init<>())
    .def(init<boost::python::object>())
    .def("set_generator", &pyeoInit::setGenerator)
    .def("__call__", &pyeoInit::operator())
    ;

    class_<FixedLengthInit<bool>, bases<eoInit<PyEOT>>>
        ("BinaryInit", init<unsigned>())
    .def("__call__", &FixedLengthInit<bool>::operator ())
    ;

    class_<pyRealInitBounded, bases<eoInit<PyEOT>>, boost::noncopyable> ("RealBoundedInit",
    init<eoRealVectorBounds&>()
    [
        with_custodian_and_ward<1,2>()
    ]
    )
    .def("__call__", &pyRealInitBounded::operator ())
    ;

    class_<pyInitPermutation, bases<eoInit<PyEOT>>, boost::noncopyable> ("PermutationInit", init<unsigned,optional<unsigned>>())
    .def("__call__", &pyInitPermutation::operator ())
    ;
}
