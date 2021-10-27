#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;


struct pyeoInit : eoInit<PyEOT> {
    pyeoInit() : eoInit<PyEOT>(){ };

    pyeoInit(boost::python::object _op) :
        eoInit<PyEOT>(),
        init_op(_op)
    { };

    void
    setGenerator(boost::python::object obj)
    {
        std::cout << "Setting generator\n";
        init_op = obj;
    }

    void
    operator () (PyEOT& _eo)
    {
        if (init_op.ptr() != Py_None) {
            // std::cout << "*** init" << std::endl;
            _eo.encoding = boost::python::call<boost::python::object>(init_op.ptr());
            _eo.invalidate();
        } else   {
            std::cout << "no move defined : do nothing";
        }
    }

private:
    boost::python::object init_op;
};


void
initialize()
{
    // eoUF : PyMOEO ---> void
    def_abstract_functor<eoInit<PyEOT> >("eoInit");

    class_<pyeoInit, bases<eoInit<PyEOT> >, boost::noncopyable>
        ("pyeoInit", init<>())
    .def(init<boost::python::object>())
    .def("set_generator", &pyeoInit::setGenerator)
    .def("__call__", &pyeoInit::operator ())
    ;
} // abstract
