#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>
// #include <eoEvalFuncPtr.h>

#include <utils/def_abstract_functor.h>
#include <pyeot.h>
#include <pso/pypot.h>

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
    operator () (SolutionType& _eo)
    {
        if (eval_op.ptr() != Py_None) {
            _eo.setFitness(
                // boost::python::call<boost::python::object>(eval_op.ptr(),_eo.encoding)
                eval_op(_eo.encoding)
            );
            // _eo.setFitness(
            //     boost::python::call<boost::python::object>(eval_op.ptr(),_eo.encoding)
            // );
        }
        else if(eo_eval_op.ptr() != Py_None){
            eo_eval_op(boost::ref(_eo));
        }
        else {
            std::cout << "no function defined : do nothing";
        }
    }

    void set_eval_op(boost::python::object obj){ eval_op = obj; }
    void set_eo_eval_op(boost::python::object obj){ eo_eval_op = obj; }

    boost::python::object get_eval_op() const {return eval_op;}
    boost::python::object get_eo_eval_op() const {return eo_eval_op;}

    // std::string className() const
    // {
    //     return "pyeoFitnessEval"; // never saw the use of className anyway
    // }

private:
    boost::python::object eval_op;
    boost::python::object eo_eval_op;
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
class pyPopEval : public eoPopEvalFunc<SolutionType> {
public:
    pyPopEval(boost::python::object _op) : pop_eval_op(_op){};

    void operator()(eoPop<SolutionType>& _parents,eoPop<SolutionType>& _offspring) {
        pop_eval_op(boost::ref(_parents),boost::ref(_offspring));
    }

private:
    boost::python::object pop_eval_op;
};



template<typename SolutionType>
struct fitness_pickle_suite : bp::pickle_suite
{
    static bp::tuple getstate(const pyeoFitnessEval<SolutionType>& f)
    {
        return bp::make_tuple(
                f.get_eval_op(),
                f.get_eo_eval_op()
        );
    }

    static void setstate(pyeoFitnessEval<SolutionType>& f, bp::tuple state)
    {
        using namespace boost::python;

        f.set_eval_op(state[0]);
        f.set_eo_eval_op(state[1]);
    }
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
    .def("set_eval_op", &pyeoFitnessEval<SolutionType>::set_eval_op)
    .def("__call__", &pyeoFitnessEval<SolutionType>::operator ())
    .def_pickle(fitness_pickle_suite<SolutionType>())
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
    class_<pyPopEval<SolutionType>, bases<eoPopEvalFunc<SolutionType>>>
    (
        make_name("pyPopEval",postfix).c_str(),
        init<boost::python::object>()[WC1]
    )
    .def("__call__", &pyPopEval<SolutionType>::operator())
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


// typedef doubleFitness (*real_eval_func)(const RealSolution&);



// // class sphere : public pyeoFitnessEval<RealSolution>
// template<class EOT>
// class sphere : public eoEvalFunc<EOT>
// {
// public:
//     void operator() (EOT& _sol) {
//         double sum = 0.0f;
//         for (unsigned int i = 0; i < _sol.size(); i++)
//             sum += _sol[i] * _sol[i];
//         _sol.fitness(sqrt(sum));
//     }
// };





void
evaluate()
{
    export_eval<PyEOT>("");
    export_eval<BinarySolution>("Bin");
    export_eval<IntSolution>("Perm");
    export_eval<RealSolution>("Real");
    export_eval<RealParticle>("RealPart");

    // class_<sphere<RealSolution>, bases<eoEvalFunc<RealSolution>>>
    // (
    //     "sphere",
    //     init<>()
    // )
    // .def("__call__", &sphere<RealSolution>::operator ())
    // ;
}
