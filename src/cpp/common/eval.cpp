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
template<class SolutionType>
struct pyeoFitnessEval : eoEvalFunc<SolutionType> {
    pyeoFitnessEval() : eoEvalFunc<SolutionType>(){ };

    pyeoFitnessEval(boost::python::object _op) :
        eoEvalFunc<SolutionType>(),
        eval_op(_op)
    { };

    void
    setEvalFunc(boost::python::object obj)
    {
        std::cout << "Setting fitness eval\n"<<std::endl;
        eval_op = obj;
    }

    void
    operator () (SolutionType& _eo)
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


template<class SolutionType>
struct pyeoObjectiveEval : eoEvalFunc<SolutionType> {
    pyeoObjectiveEval() : eoEvalFunc<SolutionType>(){ };

    pyeoObjectiveEval(boost::python::object _op) :
        eoEvalFunc<SolutionType>(),
        eval_op(_op)
    { };

    void
    setEvalFunc(boost::python::object obj)
    {
        std::cout << "Setting fitness eval\n";
        eval_op = obj;
    }

    void
    operator () (SolutionType& _eo)
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

template<class SolutionType>
void export_eval(std::string postfix)
{
    class_<pyeoFitnessEval<SolutionType>, bases<eoEvalFunc<SolutionType>>>
    (
        make_name("FitnessEval",postfix).c_str(),
        init<>()
    )
    .def(init<boost::python::object>()[WC1])
    .def("set_eval_func", &pyeoFitnessEval<SolutionType>::setEvalFunc)
    .def("__call__", &pyeoFitnessEval<SolutionType>::operator ())
    ;

    //===========================================================
    class_<pyeoObjectiveEval<SolutionType>, bases<eoEvalFunc<SolutionType>>>
    (
        make_name("ObjectiveEval",postfix).c_str(),
        init<>()
    )
    .def(init<boost::python::object>()[WC1])
    .def("set_eval_func", &pyeoObjectiveEval<SolutionType>::setEvalFunc)
    .def("__call__", &pyeoObjectiveEval<SolutionType>::operator ())
    ;

    //===========================================================
    class_<eoPopLoopEval<SolutionType>, bases<eoPopEvalFunc<SolutionType>>>
    (
        make_name("eoPopLoopEval",postfix).c_str(),
        init<eoEvalFunc<SolutionType>&>()[WC1]
    )
    .def("__call__", &eoPopLoopEval<SolutionType>::operator ())
    ;
}

void
evaluate()
{
    export_eval<PyEOT>("");
    export_eval<BinarySolution>("Bin");
    export_eval<RealSolution>("Real");
}
