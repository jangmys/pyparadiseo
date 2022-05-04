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
#include <pypot.h>

#include <utils/def_abstract_functor.h>

#include <random>

namespace p=boost::python;
namespace np=boost::python::numpy;


template<typename SolutionType>
struct pyeoInit : eoInit<SolutionType> {
    pyeoInit() : eoInit<SolutionType>(){ };

    pyeoInit(p::object _op) :
        eoInit<SolutionType>(),
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
    operator () (SolutionType& _eo)
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
struct FixedSizeInit : eoInit<FixedSizeSolution<T>> {
    FixedSizeInit(unsigned _dim) : eoInit<FixedSizeSolution<T>>(),_dimension(_dim),_gen(eoUniformGenerator<T>()){ };

    void
    operator () (FixedSizeSolution<T>& _eo)
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


struct BinarySolInit : eoInit<BinarySolution> {
    BinarySolInit(unsigned _dim) : eoInit<BinarySolution>(),_dimension(_dim),_gen(eoUniformGenerator<bool>()){ };

    void
    operator () (BinarySolution& _eo)
    {
        _eo.encoding = np::zeros(p::make_tuple(_dimension),np::dtype::get_builtin<bool>());

        for(unsigned i=0;i<_dimension;i++){
            _eo.encoding[i] = _gen();
        }
        _eo.invalidate();
    }

private:
    unsigned _dimension;
    eoUniformGenerator<bool> _gen;
};



//TODO : merge with unbounded (FIxedLength)
template<class T>
class pyRealInitBounded : public eoInit<T>{
public:
    /** Ctor - from eoRealVectorBounds */
    pyRealInitBounded(eoRealVectorBounds & _bounds):bounds(_bounds)
    {
        if (!bounds.isBounded())
            throw eoException("Needs bounded bounds to initialize a std::vector<double>");
    }

    /** simply passes the argument to the uniform method of the bounds */
    virtual void operator()(T& _eo)
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




template<class SolutionType>
class pyInitPermutation : public eoInit<SolutionType>{
public:
    pyInitPermutation(unsigned _chromSize, unsigned _startFrom = 0) : chromSize(_chromSize), startFrom(_startFrom){}

    virtual void operator()(SolutionType& _eo)
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

template<class T>
void export_realInitBounded(){
    using namespace boost::python;

    class_<pyeoInit<T>, bases<eoInit<T> >, boost::noncopyable>
        ("pyeoInit", init<>())
    .def(init<boost::python::object>())
    .def("set_generator", &pyeoInit<T>::setGenerator)
    .def("__call__", &pyeoInit<T>::operator())
    ;

    class_<pyRealInitBounded<T>, bases<eoInit<T>>> ("RealBoundedInit",
        init<eoRealVectorBounds&>()
        [
        with_custodian_and_ward<1,2>()
        ]
    )
    .def("__call__", &pyRealInitBounded<T>::operator ())
    ;

    class_<pyInitPermutation<T>, bases<eoInit<T>>, boost::noncopyable> ("PermutationInit", init<unsigned,optional<unsigned>>())
    .def("__call__", &pyInitPermutation<T>::operator ())
    ;


}



void
initialize()
{
    using namespace boost::python;

    // eoUF : PyMOEO ---> void
    // def_abstract_functor<eoInit<PyEOT> >("eoInit","docstring");

    // class_<pyeoInit, bases<eoInit<PyEOT> >, boost::noncopyable>
    //     ("pyeoInit", init<>())
    // .def(init<boost::python::object>())
    // .def("set_generator", &pyeoInit::setGenerator)
    // .def("__call__", &pyeoInit::operator())
    // ;

    class_<FixedSizeInit<bool>, bases<eoInit<FixedSizeSolution<bool>>>>
        ("BinaryInit", init<unsigned>())
    .def("__call__", &FixedSizeInit<bool>::operator ())
    ;

    class_<BinarySolInit, bases<eoInit<BinarySolution>>>
        ("BinarySolInit", init<unsigned>())
    .def("__call__", &BinarySolInit::operator ())
    ;

    export_realInitBounded<PyEOT>();

    // class_<pyInitPermutation, bases<eoInit<PyEOT>>, boost::noncopyable> ("PermutationInit", init<unsigned,optional<unsigned>>())
    // .def("__call__", &pyInitPermutation::operator ())
    // ;
}
