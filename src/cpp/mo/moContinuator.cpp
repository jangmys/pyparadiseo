#include <pyeot.h>
#include "pymo.h"

#include <continuator/moContinuator.h>
#include <continuator/moIterContinuator.h>
#include <continuator/moFullEvalContinuator.h>
#include <continuator/moTimeContinuator.h>

#include <continuator/moTrueContinuator.h>
#include <continuator/moStatBase.h>

#include <continuator/moUpdater.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;

template<typename SolutionType>
struct moContinuatorWrap : moContinuator<PyNeighbor<SolutionType>>,wrapper<moContinuator<PyNeighbor<SolutionType>>>
{
public:
    //pure virtual inherited from eoUF<SolutionType&,bool> ....
    bool operator()(SolutionType& _ind)
    {
        return this->get_override("operator()")(_ind);
    }
};

template<typename SolutionType>
struct moStatBaseWrap : moStatBase<SolutionType>,wrapper<moStatBase<SolutionType>>
{
public:
    void operator()(SolutionType& _ind)
    {
        this->get_override("operator()")(_ind);
    }
};

template<typename SolutionType>
void expose_moContinuators(std::string name)
{
    typedef PyNeighbor<SolutionType> NborT;

    def_abstract_functor<moUpdater>("moUpdater");



    class_<moContinuatorWrap<SolutionType>,boost::noncopyable>
    (make_name("moContinuator",name).c_str())
    .def("__call__", pure_virtual(&moContinuatorWrap<SolutionType>::operator()))
    ;


    class_<moIterContinuator<NborT>,bases<moContinuator<NborT>>>
    (make_name("moIterContinuator",name).c_str(),
    init<unsigned int, optional<bool>>(
        args("maxIter,verbose"),"__init__ docstring")
    )
    .def("__call__",&moIterContinuator<NborT>::operator())
    .def("init",&moIterContinuator<NborT>::init)
    .def("value",&moIterContinuator<NborT>::value)
    ;


    class_<moFullEvalContinuator<NborT>,bases<moContinuator<NborT>>>
    (make_name("moFullEvalContinuator",name).c_str(),
    init<eoEvalFuncCounter<SolutionType>&, unsigned int, optional<bool>>(
        args("_eval,_maxFullEval,_restartCounter"),"__init__ docstring")
    )
    .def("__call__",&moFullEvalContinuator<NborT>::operator())
    .def("init",&moFullEvalContinuator<NborT>::init)
    .def("value",&moFullEvalContinuator<NborT>::value)
    ;


    class_<moTimeContinuator<NborT>,bases<moContinuator<NborT>>>
    (make_name("moTimeContinuator",name).c_str(),
    init<time_t, optional<bool>>(
        args("_maxTime,_verbose"),"__init__ docstring")
    )
    .def("__call__",&moTimeContinuator<NborT>::operator())
    .def("init",&moTimeContinuator<NborT>::init)
    .def("maxTime",&moTimeContinuator<NborT>::maxTime)
    .add_property("startingTime",&moTimeContinuator<NborT>::getStartingTime,&moTimeContinuator<NborT>::setStartingTime)
    ;

    class_<moTrueContinuator<NborT>,bases<moContinuator<NborT>>>
    (make_name("moTrueContinuator",name).c_str(),init<>())
    .def("__call__",&moTrueContinuator<NborT>::operator())
    .def("init",&moTrueContinuator<NborT>::init)
    ;

    //directly inheriting from moContinuator (FIXME: should be easy to add)
    /*
    // moTrueContinuator
    moValueParamContinuator
    moBestNoImproveContinuator
    moCheckpoint
    moCombinedContinuator
    moEvalsContinuator
    moFitContinuator
    moNeighborEval
    */


    //why is this here???
    class_<moStatBaseWrap<SolutionType>,boost::noncopyable>
    (make_name("moStatBase",name).c_str())
    .def("__call__", pure_virtual(&moStatBaseWrap<SolutionType>::operator()))
    .def("lastCall", &moStatBase<SolutionType>::lastCall)
    .def("init", &moStatBase<SolutionType>::init)
    ;
}

void moContinuators()
{
    expose_moContinuators<PyEOT>("");
    expose_moContinuators<BinarySolution>("Bin");
    expose_moContinuators<RealSolution>("Real");
    expose_moContinuators<IntSolution>("Perm");
}
