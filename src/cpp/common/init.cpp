#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>
#include <utils/eoRndGenerators.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

namespace p=boost::python;
namespace np=boost::python::numpy;


struct pyeoInit : eoInit<PyEOT> {
    pyeoInit() : eoInit<PyEOT>(){ };

    pyeoInit(p::object _op) :
        eoInit<PyEOT>(),
        init_op(_op)
    { };

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
            _eo.encoding = p::call<p::object>(init_op.ptr());
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






void
initialize()
{
    using namespace boost::python;

    // eoUF : PyMOEO ---> void
    def_abstract_functor<eoInit<PyEOT> >("eoInit");

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

} // abstract
