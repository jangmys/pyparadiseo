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

#include <utils/def_abstract_functor.h>

using namespace boost::python;

template<typename SolutionType>
struct moNeighborhoodExplorerWrap : moNeighborhoodExplorer<PyNeighbor<SolutionType>>, wrapper<moNeighborhoodExplorer<PyNeighbor<SolutionType>>>
{
public:
    typedef PyNeighbor<SolutionType> NborT;

    moNeighborhoodExplorerWrap(
    ) : moNeighborhoodExplorer<NborT>()
    {}

    moNeighborhoodExplorerWrap(
        moNeighborhood<NborT>& _nhood,
        moEval<NborT>& _eval,
        boost::python::object obj
    ) : moNeighborhoodExplorer<NborT>(_nhood,_eval)
    {
        this->selectedNeighbor.setMove(obj);
        this->currentNeighbor.setMove(obj);
    }

    // pure virtual / no default
    void initParam(SolutionType& _solution)
    {
        this->get_override("initParam")(_solution);
    }

    void updateParam(SolutionType& _solution)
    {
        this->get_override("updateParam")(_solution);
    }

    bool isContinue(SolutionType& _solution)
    {
        return this->get_override("isContinue")(_solution);
    }

    bool accept(SolutionType& _solution)
    {
        return this->get_override("accept")(_solution);
    }

    void terminate(SolutionType& _solution)
    {
        this->get_override("terminate")(_solution);
    }

    void operator()(SolutionType& _sol)
    {
        this->get_override("operator()")(_sol);
    }

    // w/ default
    void move(SolutionType& _solution)
    {
        if (override f = this->get_override("move"))
        {
            this->get_override("move")(_solution);
            return;
        }
        moNeighborhoodExplorer<NborT>::move(_solution);
    }
    void default_move(SolutionType& _solution)
    {
        this->moNeighborhoodExplorer<NborT>::move(_solution);
    }

    //???
    bool callMoveApplied(){
        return this->moveApplied();
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

template<typename SolutionType>
void expose_moExplorers(std::string name)
{
    typedef PyNeighbor<SolutionType> NborT;

    class_<moNeighborhoodExplorerWrap<SolutionType>,boost::noncopyable>(make_name("moNeighborhoodExplorer",name).c_str(),
        init<>())
    .def(init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        boost::python::object
    >())
    .def("initParam", pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::initParam))
    .def("updateParam", pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::updateParam))
    .def("isContinue", pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::isContinue))
    .def("accept", pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::accept))
    .def("terminate", pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::terminate))
    .def("__call__",pure_virtual(&moNeighborhoodExplorerWrap<SolutionType>::operator()))
    .def("move",&moNeighborhoodExplorer<NborT>::move,&moNeighborhoodExplorerWrap<SolutionType>::default_move)
    .def("moveApplied",&moNeighborhoodExplorerWrap<SolutionType>::callMoveApplied)
    ;


    //HillClimbers
    class_<moSimpleHCexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>>
    (make_name("moSimpleHCexplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&
    >())
    .def("initParam",&moSimpleHCexplorer<NborT>::initParam)
    .def("updateParam",&moSimpleHCexplorer<NborT>::updateParam)
    .def("isContinue",&moSimpleHCexplorer<NborT>::isContinue)
    .def("accept",&moSimpleHCexplorer<NborT>::accept)
    .def("terminate",&moSimpleHCexplorer<NborT>::terminate)
    .def("__call__",&moSimpleHCexplorer<NborT>::operator())
    ;

    class_<moFirstImprHCexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>>
    (make_name("moFirstImprHCexplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&
    >())
    .def("initParam",&moSimpleHCexplorer<NborT>::initParam)
    .def("updateParam",&moSimpleHCexplorer<NborT>::updateParam)
    .def("isContinue",&moSimpleHCexplorer<NborT>::isContinue)
    .def("accept",&moSimpleHCexplorer<NborT>::accept)
    .def("terminate",&moSimpleHCexplorer<NborT>::terminate)
    .def("__call__",&moSimpleHCexplorer<NborT>::operator())
    ;


    class_<moRandomBestHCexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>>
    (make_name("moRandomBestHCexplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&
    >())
    .def("initParam",&moRandomBestHCexplorer<NborT>::initParam)
    .def("updateParam",&moRandomBestHCexplorer<NborT>::updateParam)
    .def("isContinue",&moRandomBestHCexplorer<NborT>::isContinue)
    .def("accept",&moRandomBestHCexplorer<NborT>::accept)
    .def("terminate",&moRandomBestHCexplorer<NborT>::terminate)
    .def("__call__",&moRandomBestHCexplorer<NborT>::operator())
    ;

    class_<moNeutralHCexplorer<NborT>,bases<moRandomBestHCexplorer<NborT>>>
    (make_name("moNeutralHCexplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&,
        unsigned
    >())
    .def("initParam",&moNeutralHCexplorer<NborT>::initParam)
    .def("updateParam",&moNeutralHCexplorer<NborT>::updateParam)
    .def("isContinue",&moNeutralHCexplorer<NborT>::isContinue)
    .def("accept",&moNeutralHCexplorer<NborT>::accept)
    // .def("terminate",&moNeutralHCexplorer<NborT>::terminate)
    // .def("__call__",&moNeutralHCexplorer<NborT>::operator())
    ;



    //Random
    class_<moRandomSearchExplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moRandomSearchExplorer",name).c_str(),init<
        eoInit<SolutionType>&,
        eoEvalFunc<SolutionType>&,
        unsigned
    >())
    .def("initParam",&moRandomSearchExplorer<NborT>::initParam)
    .def("updateParam",&moRandomSearchExplorer<NborT>::updateParam)
    .def("terminate",&moRandomSearchExplorer<NborT>::terminate)
    .def("__call__",&moRandomSearchExplorer<NborT>::operator())
    .def("isContinue",&moRandomSearchExplorer<NborT>::isContinue)
    .def("accept",&moRandomSearchExplorer<NborT>::accept)
    ;

    class_<moRandomWalkExplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moRandomWalkExplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&
    >())
    .def("initParam",&moRandomWalkExplorer<NborT>::initParam)
    .def("updateParam",&moRandomWalkExplorer<NborT>::updateParam)
    .def("terminate",&moRandomWalkExplorer<NborT>::terminate)
    .def("__call__",&moRandomWalkExplorer<NborT>::operator())
    .def("isContinue",&moRandomWalkExplorer<NborT>::isContinue)
    .def("accept",&moRandomWalkExplorer<NborT>::accept)
    ;

    class_<moRandomNeutralWalkExplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moRandomNeutralWalkExplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moSolNeighborComparator<NborT>&,
        unsigned
    >())
    .def("initParam",&moRandomNeutralWalkExplorer<NborT>::initParam)
    .def("updateParam",&moRandomNeutralWalkExplorer<NborT>::updateParam)
    .def("terminate",&moRandomNeutralWalkExplorer<NborT>::terminate)
    .def("__call__",&moRandomNeutralWalkExplorer<NborT>::operator())
    .def("isContinue",&moRandomNeutralWalkExplorer<NborT>::isContinue)
    .def("accept",&moRandomNeutralWalkExplorer<NborT>::accept)
    ;



    class_<moMetropolisHastingExplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moMetropolisHastingExplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&,
        unsigned int
    >())
    .def("initParam",&moMetropolisHastingExplorer<NborT>::initParam)
    .def("updateParam",&moMetropolisHastingExplorer<NborT>::updateParam)
    .def("terminate",&moMetropolisHastingExplorer<NborT>::terminate)
    .def("__call__",&moMetropolisHastingExplorer<NborT>::operator())
    .def("isContinue",&moMetropolisHastingExplorer<NborT>::isContinue)
    .def("accept",&moMetropolisHastingExplorer<NborT>::accept)
    ;






    class_<moSAexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moSAexplorer",name).c_str(),init<
    moNeighborhood<NborT>&,
    moEval<NborT>&,
    moSolNeighborComparator<NborT>&,
    moCoolingSchedule<SolutionType>&
    >())
    .def("initParam",&moSAexplorer<NborT>::initParam)
    .def("updateParam",&moSAexplorer<NborT>::updateParam)
    .def("terminate",&moSAexplorer<NborT>::terminate)
    .def("__call__",&moSAexplorer<NborT>::operator())
    .def("isContinue",&moSAexplorer<NborT>::isContinue)
    .def("accept",&moSAexplorer<NborT>::accept)
    .def("getTemperature",&moSAexplorer<NborT>::getTemperature)
    ;

    class_<moTSexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moTSexplorer",name).c_str(),init<
        moNeighborhood<NborT>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&,
        moTabuList<NborT>&,
        moIntensification<NborT>&,
        moDiversification<NborT>&,
        moAspiration<NborT>&
    >())
    ;

    class_<moILSexplorer<NborT,NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moILSexplorer",name).c_str(),init<
        moLocalSearch<NborT>&,
        moPerturbation<NborT>&,
        moAcceptanceCriterion<NborT>&
    >())
    .def("initParam",&moILSexplorer<NborT,NborT>::initParam)
    .def("updateParam",&moILSexplorer<NborT,NborT>::updateParam)
    .def("terminate",&moILSexplorer<NborT,NborT>::terminate)
    .def("__call__",&moILSexplorer<NborT,NborT>::operator())
    .def("isContinue",&moILSexplorer<NborT,NborT>::isContinue)
    .def("accept",&moILSexplorer<NborT,NborT>::accept)
    .def("move",&moILSexplorer<NborT,NborT>::move)
    ;

    class_<moVNSexplorer<NborT>,bases<moNeighborhoodExplorer<NborT>>,boost::noncopyable>
    (make_name("moVNSexplorer",name).c_str(),init<
        moVariableNeighborhoodSelection<SolutionType>&,
        moAcceptanceCriterion<NborT>&
    >())
    .def("initParam",&moVNSexplorer<NborT>::initParam)
    .def("updateParam",&moVNSexplorer<NborT>::updateParam)
    .def("terminate",&moVNSexplorer<NborT>::terminate)
    .def("__call__",&moVNSexplorer<NborT>::operator())
    .def("isContinue",&moVNSexplorer<NborT>::isContinue)
    .def("accept",&moVNSexplorer<NborT>::accept)
    .def("move",&moVNSexplorer<NborT>::move)
    ;
}



void moExplorers()
{
    expose_moExplorers<PyEOT>("");
    expose_moExplorers<BinarySolution>("Bin");
    expose_moExplorers<RealSolution>("Real");
}
