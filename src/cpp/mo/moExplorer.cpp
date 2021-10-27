#include <pyeot.h>
#include "pymo.h"

//Base class
#include <explorer/moNeighborhoodExplorer.h>
//Hill-Climbers
#include <explorer/moSimpleHCexplorer.h>
#include <explorer/moFirstImprHCexplorer.h>
#include <explorer/moRandomBestHCexplorer.h>
#include <explorer/moNeutralHCexplorer.h>
//Random
#include <explorer/moRandomSearchExplorer.h>
#include <explorer/moRandomWalkExplorer.h>
#include <explorer/moRandomNeutralWalkExplorer.h>
//Stochastic
#include <explorer/moMetropolisHastingExplorer.h>
#include <explorer/moSAexplorer.h>
#include <explorer/moTSexplorer.h>
#include <explorer/moILSexplorer.h>
#include <explorer/moVNSexplorer.h>

#include <neighborhood/moNeighborhood.h>
#include <neighborhood/moDummyNeighborhood.h>

using namespace boost::python;

struct moNeighborhoodExplorerWrap : moNeighborhoodExplorer<PyNeighbor>, wrapper<moNeighborhoodExplorer<PyNeighbor>>
{
public:
    moNeighborhoodExplorerWrap(
    ) : moNeighborhoodExplorer<PyNeighbor>()
    {}

    moNeighborhoodExplorerWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        moEval<PyNeighbor>& _eval,
        boost::python::object obj
    ) : moNeighborhoodExplorer<PyNeighbor>(_nhood,_eval)
    {
        selectedNeighbor.setMove(obj);
        currentNeighbor.setMove(obj);
    }

    // pure virtual / no default
    void initParam(PyEOT& _solution)
    {
        this->get_override("initParam")(_solution);
    }

    void updateParam(PyEOT& _solution)
    {
        this->get_override("updateParam")(_solution);
    }

    bool isContinue(PyEOT& _solution)
    {
        return this->get_override("isContinue")(_solution);
    }

    bool accept(PyEOT& _solution)
    {
        return this->get_override("accept")(_solution);
    }

    void terminate(PyEOT& _solution)
    {
        this->get_override("terminate")(_solution);
    }

    void operator()(PyEOT& _sol)
    {
        this->get_override("operator()")(_sol);
    }

    // w/ default
    void move(PyEOT& _solution)
    {
        if (override f = this->get_override("move"))
        {
            this->get_override("move")(_solution);
            return;
        }
        moNeighborhoodExplorer<PyNeighbor>::move(_solution);
    }
    void default_move(PyEOT& _solution)
    {
        this->moNeighborhoodExplorer<PyNeighbor>::move(_solution);
    }

    //???
    bool callMoveApplied(){
        return moveApplied();
    }
};

// struct moSimpleHCexplorerWrap : moSimpleHCexplorer<PyNeighbor>, wrapper<moSimpleHCexplorer<PyNeighbor>>
// {
//     moNeighborhoodExplorerWrap(
//         moNeighborhood<PyNeighbor>& _nhood,
//         moEval<PyNeighbor>& _eval,
//         moNeighborComparator<PyNeighbor>& _cmpNbor,
//         moSolNeighborComparator<PyNeighbor>& _cmpSolNbor
//         boost::python::object obj
//     ) : moNeighborhoodExplorer<PyNeighbor>(_nhood,_eval)
//     {
//         selectedNeighbor.setMove(obj);
//         currentNeighbor.setMove(obj);
//     }
//
//
// }


void moExplorers()
{
    class_<moNeighborhoodExplorerWrap,boost::noncopyable>("moNeighborhoodExplorer",
        init<>())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        boost::python::object
    >())
    .def("initParam", pure_virtual(&moNeighborhoodExplorerWrap::initParam))
    .def("updateParam", pure_virtual(&moNeighborhoodExplorerWrap::updateParam))
    .def("isContinue", pure_virtual(&moNeighborhoodExplorerWrap::isContinue))
    .def("accept", pure_virtual(&moNeighborhoodExplorerWrap::accept))
    .def("terminate", pure_virtual(&moNeighborhoodExplorerWrap::terminate))
    .def("__call__",pure_virtual(&moNeighborhoodExplorerWrap::operator()))
    .def("move",&moNeighborhoodExplorer<PyNeighbor>::move,&moNeighborhoodExplorerWrap::default_move)
    .def("moveApplied",&moNeighborhoodExplorerWrap::callMoveApplied)
    ;


    //HillClimbers
    class_<moSimpleHCexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>>
    ("moSimpleHCexplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >())
    .def("initParam",&moSimpleHCexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moSimpleHCexplorer<PyNeighbor>::updateParam)
    .def("isContinue",&moSimpleHCexplorer<PyNeighbor>::isContinue)
    .def("accept",&moSimpleHCexplorer<PyNeighbor>::accept)
    .def("terminate",&moSimpleHCexplorer<PyNeighbor>::terminate)
    .def("__call__",&moSimpleHCexplorer<PyNeighbor>::operator())
    ;

    class_<moFirstImprHCexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>>
    ("moFirstImprHCexplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >())
    .def("initParam",&moSimpleHCexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moSimpleHCexplorer<PyNeighbor>::updateParam)
    .def("isContinue",&moSimpleHCexplorer<PyNeighbor>::isContinue)
    .def("accept",&moSimpleHCexplorer<PyNeighbor>::accept)
    .def("terminate",&moSimpleHCexplorer<PyNeighbor>::terminate)
    .def("__call__",&moSimpleHCexplorer<PyNeighbor>::operator())
    ;


    class_<moRandomBestHCexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>>
    ("moRandomBestHCexplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >())
    .def("initParam",&moRandomBestHCexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moRandomBestHCexplorer<PyNeighbor>::updateParam)
    .def("isContinue",&moRandomBestHCexplorer<PyNeighbor>::isContinue)
    .def("accept",&moRandomBestHCexplorer<PyNeighbor>::accept)
    .def("terminate",&moRandomBestHCexplorer<PyNeighbor>::terminate)
    .def("__call__",&moRandomBestHCexplorer<PyNeighbor>::operator())
    ;

    class_<moNeutralHCexplorer<PyNeighbor>,bases<moRandomBestHCexplorer<PyNeighbor>>>
    ("moNeutralHCexplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&,
        unsigned
    >())
    .def("initParam",&moNeutralHCexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moNeutralHCexplorer<PyNeighbor>::updateParam)
    .def("isContinue",&moNeutralHCexplorer<PyNeighbor>::isContinue)
    .def("accept",&moNeutralHCexplorer<PyNeighbor>::accept)
    // .def("terminate",&moNeutralHCexplorer<PyNeighbor>::terminate)
    // .def("__call__",&moNeutralHCexplorer<PyNeighbor>::operator())
    ;



    //Random
    class_<moRandomSearchExplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moRandomSearchExplorer",init<
        eoInit<PyEOT>&,
        eoEvalFunc<PyEOT>&,
        unsigned
    >())
    .def("initParam",&moRandomSearchExplorer<PyNeighbor>::initParam)
    .def("updateParam",&moRandomSearchExplorer<PyNeighbor>::updateParam)
    .def("terminate",&moRandomSearchExplorer<PyNeighbor>::terminate)
    .def("__call__",&moRandomSearchExplorer<PyNeighbor>::operator())
    .def("isContinue",&moRandomSearchExplorer<PyNeighbor>::isContinue)
    .def("accept",&moRandomSearchExplorer<PyNeighbor>::accept)
    ;

    class_<moRandomWalkExplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moRandomWalkExplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&
    >())
    .def("initParam",&moRandomWalkExplorer<PyNeighbor>::initParam)
    .def("updateParam",&moRandomWalkExplorer<PyNeighbor>::updateParam)
    .def("terminate",&moRandomWalkExplorer<PyNeighbor>::terminate)
    .def("__call__",&moRandomWalkExplorer<PyNeighbor>::operator())
    .def("isContinue",&moRandomWalkExplorer<PyNeighbor>::isContinue)
    .def("accept",&moRandomWalkExplorer<PyNeighbor>::accept)
    ;

    class_<moRandomNeutralWalkExplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moRandomNeutralWalkExplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&,
        unsigned
    >())
    .def("initParam",&moRandomNeutralWalkExplorer<PyNeighbor>::initParam)
    .def("updateParam",&moRandomNeutralWalkExplorer<PyNeighbor>::updateParam)
    .def("terminate",&moRandomNeutralWalkExplorer<PyNeighbor>::terminate)
    .def("__call__",&moRandomNeutralWalkExplorer<PyNeighbor>::operator())
    .def("isContinue",&moRandomNeutralWalkExplorer<PyNeighbor>::isContinue)
    .def("accept",&moRandomNeutralWalkExplorer<PyNeighbor>::accept)
    ;



    class_<moMetropolisHastingExplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moMetropolisHastingExplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&,
        unsigned int
    >())
    .def("initParam",&moMetropolisHastingExplorer<PyNeighbor>::initParam)
    .def("updateParam",&moMetropolisHastingExplorer<PyNeighbor>::updateParam)
    .def("terminate",&moMetropolisHastingExplorer<PyNeighbor>::terminate)
    .def("__call__",&moMetropolisHastingExplorer<PyNeighbor>::operator())
    .def("isContinue",&moMetropolisHastingExplorer<PyNeighbor>::isContinue)
    .def("accept",&moMetropolisHastingExplorer<PyNeighbor>::accept)
    ;






    class_<moSAexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moSAexplorer",init<
    moNeighborhood<PyNeighbor>&,
    moEval<PyNeighbor>&,
    moSolNeighborComparator<PyNeighbor>&,
    moCoolingSchedule<PyEOT>&
    >())
    .def("initParam",&moSAexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moSAexplorer<PyNeighbor>::updateParam)
    .def("terminate",&moSAexplorer<PyNeighbor>::terminate)
    .def("__call__",&moSAexplorer<PyNeighbor>::operator())
    .def("isContinue",&moSAexplorer<PyNeighbor>::isContinue)
    .def("accept",&moSAexplorer<PyNeighbor>::accept)
    .def("getTemperature",&moSAexplorer<PyNeighbor>::getTemperature)
    ;

    class_<moTSexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moTSexplorer",init<
        moNeighborhood<PyNeighbor>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&,
        moTabuList<PyNeighbor>&,
        moIntensification<PyNeighbor>&,
        moDiversification<PyNeighbor>&,
        moAspiration<PyNeighbor>&
    >())
    ;

    class_<moILSexplorer<PyNeighbor,PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moILSexplorer",init<
        moLocalSearch<PyNeighbor>&,
        moPerturbation<PyNeighbor>&,
        moAcceptanceCriterion<PyNeighbor>&
    >())
    .def("initParam",&moILSexplorer<PyNeighbor,PyNeighbor>::initParam)
    .def("updateParam",&moILSexplorer<PyNeighbor,PyNeighbor>::updateParam)
    .def("terminate",&moILSexplorer<PyNeighbor,PyNeighbor>::terminate)
    .def("__call__",&moILSexplorer<PyNeighbor,PyNeighbor>::operator())
    .def("isContinue",&moILSexplorer<PyNeighbor,PyNeighbor>::isContinue)
    .def("accept",&moILSexplorer<PyNeighbor,PyNeighbor>::accept)
    .def("move",&moILSexplorer<PyNeighbor,PyNeighbor>::move)
    ;

    class_<moVNSexplorer<PyNeighbor>,bases<moNeighborhoodExplorer<PyNeighbor>>,boost::noncopyable>
    ("moVNSexplorer",init<
        moVariableNeighborhoodSelection<PyEOT>&,
        moAcceptanceCriterion<PyNeighbor>&
    >())
    .def("initParam",&moVNSexplorer<PyNeighbor>::initParam)
    .def("updateParam",&moVNSexplorer<PyNeighbor>::updateParam)
    .def("terminate",&moVNSexplorer<PyNeighbor>::terminate)
    .def("__call__",&moVNSexplorer<PyNeighbor>::operator())
    .def("isContinue",&moVNSexplorer<PyNeighbor>::isContinue)
    .def("accept",&moVNSexplorer<PyNeighbor>::accept)
    .def("move",&moVNSexplorer<PyNeighbor>::move)
    ;
}
