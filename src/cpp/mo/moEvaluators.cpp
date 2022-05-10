#include <boost/python.hpp>

#include <pyeot.h>
#include <pymo.h>

#include <utils/def_abstract_functor.h>

#include <eval/moEval.h>
#include <eval/moNeighborhoodEvaluation.h>
//
#include <eval/moFullEvalByCopy.h>
#include <eval/moFullEvalByModif.h>


template<typename SolutionType>
struct pyNeighborEval : moEval<PyNeighbor<SolutionType>> {
    pyNeighborEval() : moEval<PyNeighbor<SolutionType>>(){ };

    pyNeighborEval(boost::python::object _op) :
        moEval<PyNeighbor<SolutionType>>(),
        eval_op(_op)
    { };

    void
    setEvalFunc(boost::python::object obj)
    {
        eval_op = obj;
    }

    void
    operator () (SolutionType& _eo, PyNeighbor<SolutionType>& _nbor)
    {
        if (eval_op.ptr() != Py_None) {
            _nbor.setFitness(
                boost::python::call<boost::python::object>(eval_op.ptr(),_eo.encoding,_eo.getFitness(),_nbor.index())
            );
        } else {
            std::cout << "no function defined : do nothing";
        }
    }

private:
    boost::python::object eval_op;
};



template<typename SolutionType>
void expose_moEvaluators(std::string name){
    using namespace boost::python;

    def_abstract_functor<moEval<PyNeighbor<SolutionType> > >(make_name("moEval",name).c_str());


    def_abstract_functor<moNeighborhoodEvaluation<PyNeighbor<SolutionType>> >(make_name("moNeighborhoodEvaluation",name).c_str());


    class_<pyNeighborEval<SolutionType>, bases<moEval<PyNeighbor<SolutionType>>>>
        (make_name("NeighborEval",name).c_str(), init<>())
    .def(init<boost::python::object>()
    [WC1]
    )
    .def("set_eval_func", &pyNeighborEval<SolutionType>::setEvalFunc)
    .def("__call__", &pyNeighborEval<SolutionType>::operator ())
    ;


    class_<moFullEvalByCopy<PyNeighbor<SolutionType>>,bases<moEval<PyNeighbor<SolutionType>>>>
    (make_name("moFullEvalByCopy",name).c_str(),
    "parameters : eoEvalFunc \n \
    // __call__ evaluates neighbor by making a tmp copy of solution, move it, evaluate and set fitness of neighbor",
    init<
        eoEvalFunc<SolutionType>&
    >(args("_eoEval"), "__init__ docstring")
    [WC1]
    )
    .def("__call__",&moFullEvalByCopy<PyNeighbor<SolutionType>>::operator())
    ;


    class_<moFullEvalByModif<PyNeighbor<SolutionType>>,bases<moEval<PyNeighbor<SolutionType>>>>
    (make_name("moFullEvalByModif",name).c_str(),
    "parameters : eoEvalFunc \n \
    __call__ moves solution, evaluates it an moves it back : requires moveBack to be defined in PyNeighbor",
    init<
        eoEvalFunc<SolutionType>&
    >(args("_eoEval"), "__init__ docstring")
    [WC1]
    )
    .def("__call__",&moFullEvalByModif<PyNeighbor<SolutionType>>::operator())
    ;
};

void moEvaluators(){
    expose_moEvaluators<PyEOT>("");
    expose_moEvaluators<BinarySolution>("Bin");
}
