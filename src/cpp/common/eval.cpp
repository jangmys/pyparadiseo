#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>

#include <utils/def_abstract_functor.h>
#include <pyeot.h>

using namespace boost::python;

//could unify these by passing objecitve_traits ...
// 1 --> set fitness
// >1 -> set objectives


struct pyeoFitnessEval : eoEvalFunc<PyEOT> {
    pyeoFitnessEval() : eoEvalFunc<PyEOT>(){ };

    pyeoFitnessEval(boost::python::object _op) :
        eoEvalFunc<PyEOT>(),
        eval_op(_op)
    { };

    void
    setEvalFunc(boost::python::object obj)
    {
        std::cout << "Setting fitness eval\n"<<std::endl;
        eval_op = obj;
    }

    void
    operator () (PyEOT& _eo)
    {
        if (eval_op.ptr() != Py_None) {
            _eo.setFitness(
                boost::python::call<boost::python::object>(eval_op.ptr(),_eo.encoding)
            );
            // _eo.invalidate();
        } else {
            std::cout << "no function defined : do nothing";
        }
    }

private:
    boost::python::object eval_op;
};



struct pyeoObjectiveEval : eoEvalFunc<PyEOT> {
    pyeoObjectiveEval() : eoEvalFunc<PyEOT>(){ };

    pyeoObjectiveEval(boost::python::object _op) :
        eoEvalFunc<PyEOT>(),
        eval_op(_op)
    { };

    void
    setEvalFunc(boost::python::object obj)
    {
        std::cout << "Setting fitness eval\n";
        eval_op = obj;
    }

    void
    operator () (PyEOT& _eo)
    {
        if (eval_op.ptr() != Py_None) {
            //std::cout << "*** oeval" << std::endl;
            _eo.setObjectiveVector(
                boost::python::call<boost::python::object>(eval_op.ptr(),_eo.encoding)
            );
            // _eo.invalidate();
        } else {
            std::cout << "no function defined : do nothing";
        }
    }

private:
    boost::python::object eval_op;
};

void
evaluate()
{
    def_abstract_functor<eoEvalFunc<PyEOT> >("eoEvalFunc");

    class_<pyeoFitnessEval, bases<eoEvalFunc<PyEOT>>>
        ("FitnessEval", init<>())
    .def(init<boost::python::object>()
    [WC1]
    )
    .def("set_eval_func", &pyeoFitnessEval::setEvalFunc)
    .def("__call__", &pyeoFitnessEval::operator ())
    ;

    class_<pyeoObjectiveEval, bases<eoEvalFunc<PyEOT> >, boost::noncopyable>
        ("ObjectiveEval", init<>())
    .def(init<boost::python::object>()
    [WC1]
    )
    .def("set_eval_func", &pyeoObjectiveEval::setEvalFunc)
    .def("__call__", &pyeoObjectiveEval::operator ())
    ;

    //===========================================================

    def_abstract_functor<eoPopEvalFunc<PyEOT> >("eoPopEvalFunc");

    class_<eoPopLoopEval<PyEOT>, bases<eoPopEvalFunc<PyEOT> > >
    (
        "eoPopLoopEval",
        init<
            eoEvalFunc<PyEOT>&
        >()[WC1]
    )
    .def("__call__", &eoPopLoopEval<PyEOT>::operator ());
}
