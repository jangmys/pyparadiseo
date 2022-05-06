#include <pyeot.h>
#include "pymo.h"

#include <continuator/moContinuator.h>
#include <continuator/moIterContinuator.h>
#include <continuator/moFullEvalContinuator.h>
#include <continuator/moTimeContinuator.h>

#include <continuator/moTrueContinuator.h>


#include <continuator/moStatBase.h>

using namespace boost::python;

template<typename SolutionType>
struct moContinuatorWrap : moContinuator<PyNeighbor<SolutionType>>,wrapper<moContinuator<PyNeighbor<SolutionType>>>
{
public:
    //pure virtual inherited from eoUF<PyEOT&,bool> ....
    bool operator()(SolutionType& _ind)
    {
        return this->get_override("operator()")(_ind);
    }
};


struct moStatBaseWrap : moStatBase<PyEOT>,wrapper<moStatBase<PyEOT>>
{
public:
    void operator()(PyEOT& _ind)
    {
        this->get_override("operator()")(_ind);
    }
};

template<typename SolutionType>
void expose_moContinuators()
{
    typedef PyNeighbor<SolutionType> NborT;

    class_<moContinuatorWrap<SolutionType>,boost::noncopyable>
    ("moContinuator")
    .def("__call__", pure_virtual(&moContinuatorWrap<SolutionType>::operator()))
    ;

    class_<moIterContinuator<NborT>,bases<moContinuator<NborT>>>
    ("moIterContinuator",
    init<unsigned int, bool>(args("maxIter,verbose"),"__init__ docstring")
    )
    .def("__call__",&moIterContinuator<NborT>::operator())
    .def("init",&moIterContinuator<NborT>::init)
    .def("value",&moIterContinuator<NborT>::value)
    ;

    class_<moFullEvalContinuator<NborT>,bases<moContinuator<NborT>>>
    ("moFullEvalContinuator",
    init<eoEvalFuncCounter<PyEOT>&, unsigned int, bool>(args("_eval,_maxFullEval,_restartCounter"),"__init__ docstring")
    )
    .def("__call__",&moFullEvalContinuator<NborT>::operator())
    .def("init",&moFullEvalContinuator<NborT>::init)
    .def("value",&moFullEvalContinuator<NborT>::value)
    ;

    class_<moTimeContinuator<NborT>,bases<moContinuator<NborT>>>
    ("moTimeContinuator",
    init<time_t, bool>(args("_maxTime,_verbose"),"__init__ docstring")
    )
    .def("__call__",&moTimeContinuator<NborT>::operator())
    .def("init",&moTimeContinuator<NborT>::init)
    .def("maxTime",&moTimeContinuator<NborT>::maxTime)
    .add_property("startingTime",&moTimeContinuator<NborT>::getStartingTime,&moTimeContinuator<NborT>::setStartingTime)
    ;

    class_<moTrueContinuator<NborT>,bases<moContinuator<NborT>>>
    ("moTrueContinuator",init<>())
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


    class_<moStatBaseWrap,boost::noncopyable>
    ("moStatBase")
    .def("__call__", pure_virtual(&moStatBaseWrap::operator()))
    .def("lastCall", &moStatBase<PyEOT>::lastCall)
    .def("init", &moStatBase<PyEOT>::init)
    ;
}

void moContinuators()
{
    expose_moContinuators<PyEOT>();
}
