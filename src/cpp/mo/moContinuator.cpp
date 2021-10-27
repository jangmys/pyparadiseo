#include <pyeot.h>
#include "pymo.h"

#include <continuator/moContinuator.h>
#include <continuator/moIterContinuator.h>
#include <continuator/moFullEvalContinuator.h>
#include <continuator/moTimeContinuator.h>

#include <continuator/moTrueContinuator.h>


#include <continuator/moStatBase.h>

using namespace boost::python;

struct moContinuatorWrap : moContinuator<PyNeighbor>,wrapper<moContinuator<PyNeighbor>>
{
public:
    //pure virtual inherited from eoUF<PyEOT&,bool> ....
    bool operator()(PyEOT& _ind)
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


void moContinuators()
{
    class_<moContinuatorWrap,boost::noncopyable>
    ("moContinuator")
    .def("__call__", pure_virtual(&moContinuatorWrap::operator()))
    ;

    class_<moIterContinuator<PyNeighbor>,bases<moContinuator<PyNeighbor>>>
    ("moIterContinuator",
    init<unsigned int, bool>(args("maxIter,verbose"),"__init__ docstring")
    )
    .def("__call__",&moIterContinuator<PyNeighbor>::operator())
    .def("init",&moIterContinuator<PyNeighbor>::init)
    .def("value",&moIterContinuator<PyNeighbor>::value)
    ;

    class_<moFullEvalContinuator<PyNeighbor>,bases<moContinuator<PyNeighbor>>>
    ("moFullEvalContinuator",
    init<eoEvalFuncCounter<PyEOT>&, unsigned int, bool>(args("_eval,_maxFullEval,_restartCounter"),"__init__ docstring")
    )
    .def("__call__",&moFullEvalContinuator<PyNeighbor>::operator())
    .def("init",&moFullEvalContinuator<PyNeighbor>::init)
    .def("value",&moFullEvalContinuator<PyNeighbor>::value)
    ;

    class_<moTimeContinuator<PyNeighbor>,bases<moContinuator<PyNeighbor>>>
    ("moTimeContinuator",
    init<time_t, bool>(args("_maxTime,_verbose"),"__init__ docstring")
    )
    .def("__call__",&moTimeContinuator<PyNeighbor>::operator())
    .def("init",&moTimeContinuator<PyNeighbor>::init)
    .def("maxTime",&moTimeContinuator<PyNeighbor>::maxTime)
    .add_property("startingTime",&moTimeContinuator<PyNeighbor>::getStartingTime,&moTimeContinuator<PyNeighbor>::setStartingTime)
    ;

    class_<moTrueContinuator<PyNeighbor>,bases<moContinuator<PyNeighbor>>>
    ("moTrueContinuator",init<>())
    .def("__call__",&moTrueContinuator<PyNeighbor>::operator())
    .def("init",&moTrueContinuator<PyNeighbor>::init)
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
