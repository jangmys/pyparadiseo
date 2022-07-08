// #include "PyMOEO.h"
// #include "PyMO.h"
#include <pyeot.h>
#include "pymo.h"

//MO
#include <explorer/moNeighborhoodExplorer.h>
#include <continuator/moContinuator.h>

//base
#include <algo/moLocalSearch.h>
//Hill-Climbers
#include <algo/moSimpleHC.h>
#include <algo/moFirstImprHC.h>
#include <algo/moRandomBestHC.h>
#include <algo/moNeutralHC.h>
//Random
#include <algo/moRandomSearch.h>
#include <algo/moRandomWalk.h>
#include <algo/moRandomNeutralWalk.h>
//stochastic
#include <algo/moMetropolisHasting.h>
#include <algo/moSA.h>
#include <algo/moTS.h>
#include <algo/moILS.h>
#include <algo/moVNS.h>

//EO
#include <eoOp.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;

template<typename SolutionType>
struct moLocalSearchWrap : moLocalSearch<PyNeighbor<SolutionType>>,wrapper<moLocalSearch<PyNeighbor<SolutionType>>>
{
    typedef PyNeighbor<SolutionType> NborT;

    moLocalSearchWrap(
        moNeighborhoodExplorer<NborT>& _nhexpl,
        moContinuator<NborT>& _cont,
        eoEvalFunc<SolutionType>& _eval
    ) : moLocalSearch<NborT>(_nhexpl,_cont,_eval){};

    //virtual operator() has a default implementation
    bool operator()(SolutionType& _sol)
    {
        if(override f = this->get_override("operator()"))
        {
            return this->get_override("operator()")(_sol);
        }
        return moLocalSearch<NborT>::operator()(_sol);
    }
    bool default_op(SolutionType& _sol){
        return this->moLocalSearch<NborT>::operator()(_sol);
    }

    // void setMove(boost::python::object _obj)
    // {
    //     this->searchExplorer.getSelectedNeighbor().setMove(_obj);
    //     this->searchExplorer.getCurrentNeighbor().setMove(_obj);
    // }
    // void setMoveBack(boost::python::object _obj)
    // {
    //     this->searchExplorer.getSelectedNeighbor().setMoveBack(_obj);
    //     this->searchExplorer.getCurrentNeighbor().setMoveBack(_obj);
    // }
};



template<typename X>
void setMove(const X& _ls, boost::python::object _obj)
{
    _ls.getNeighborhoodExplorer().getSelectedNeighbor().setMove(_obj);
    _ls.getNeighborhoodExplorer().getCurrentNeighbor().setMove(_obj);
}

template<typename X>
void setMoveBack(const X& _ls, boost::python::object _obj)
{
    _ls.getNeighborhoodExplorer().getSelectedNeighbor().setMoveBack(_obj);
    _ls.getNeighborhoodExplorer().getCurrentNeighbor().setMoveBack(_obj);
}


template<typename SolutionType>
void expose_moAlgos(std::string name)
{
    typedef PyNeighbor<SolutionType> NborT;

    class_<moLocalSearchWrap<SolutionType>,bases<eoMonOp<SolutionType>>,boost::noncopyable>
    (make_name("moLocalSearch",name).c_str(),
        init<
            moNeighborhoodExplorer<NborT>&,
            moContinuator<NborT>&,
            eoEvalFunc<SolutionType>&
        >()
        [WC3]
    )
    .def("__call__",&moLocalSearch<NborT>::operator(),&moLocalSearchWrap<SolutionType>::default_op)
    .def("getNeighborhoodExplorer",&moLocalSearch<NborT>::getNeighborhoodExplorer,return_internal_reference<>())
    .def("set_move",setMove<moLocalSearchWrap<SolutionType>>)
    .def("set_move_back",setMoveBack<moLocalSearchWrap<SolutionType>>)
    // .def("set_move",&moLocalSearchWrap<SolutionType>::setMove)
    // .def("set_move_back",&moLocalSearchWrap<SolutionType>::setMoveBack)
    ;


    class_<moSimpleHC<NborT>, bases<moLocalSearch<NborT>>>
    (make_name("moSimpleHC",name).c_str(),
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&,
            moNeighborComparator<NborT>&,
            moSolNeighborComparator<NborT>&
        >()[WC6]
    )
    .def("set_move",setMove<moSimpleHC<NborT>>)
    .def("set_move_back",setMoveBack<moSimpleHC<NborT>>)
    ;



    class_<moFirstImprHC<NborT>, bases<moLocalSearch<NborT>>>
    (make_name("moFirstImprHC",name).c_str(),
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&,
            moNeighborComparator<NborT>&,
            moSolNeighborComparator<NborT>&
        >()[WC6]
    )
    .def("set_move",setMove<moFirstImprHC<NborT>>)
    .def("set_move_back",setMoveBack<moFirstImprHC<NborT>>)
    ;


    class_<moRandomBestHC<NborT>, bases<moLocalSearch<NborT>>>
    (make_name("moRandomBestHC",name).c_str(),
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&,
            moNeighborComparator<NborT>&,
            moSolNeighborComparator<NborT>&
        >()[WC6]
    )
    .def("set_move",setMove<moRandomBestHC<NborT>>)
    .def("set_move_back",setMoveBack<moRandomBestHC<NborT>>)
    ;



    class_<moNeutralHC<NborT>, bases<moLocalSearch<NborT>>, boost::noncopyable>
    (make_name("moNeutralHC",name).c_str(),
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            unsigned int
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            unsigned int,
            moContinuator<NborT>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4,
            with_custodian_and_ward<1,6>>>>()
        ]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            unsigned int,
            moContinuator<NborT>&,
            moNeighborComparator<NborT>&,
            moSolNeighborComparator<NborT>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4,
            with_custodian_and_ward<1,6,
            with_custodian_and_ward<1,7,
            with_custodian_and_ward<1,8>>>>>>()
        ]
    )
    .def("set_move",setMove<moNeutralHC<NborT>>)
    .def("set_move_back",setMoveBack<moNeutralHC<NborT>>)
    ;


    //Random : What for ???
    class_<moRandomSearch<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moRandomSearch",name).c_str(),
        init<
            eoInit<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            unsigned
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3>>()
        ]
    )
    .def(
        init<
            eoInit<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            unsigned,
            moContinuator<NborT>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,5>>>()
        ]
    )
    ;

    class_<moRandomWalk<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moRandomWalk",name).c_str(),
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            unsigned
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4>>>()
        ]
    )
    .def(
        init<
            moNeighborhood<NborT>&,
            eoEvalFunc<SolutionType>&,
            moEval<NborT>&,
            moContinuator<NborT>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4,
            with_custodian_and_ward<1,5>>>>()
        ]
    )
    .def("set_move",setMove<moRandomWalk<NborT>>)
    ;


    class_<moRandomNeutralWalk<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moRandomNeutralWalk",name).c_str(),init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned,
        moContinuator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,6
        >>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned,
        moContinuator<NborT>&,
        moSolNeighborComparator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,6,
        with_custodian_and_ward<1,7
        >>>>>()
    ]
    )
    .def("set_move",setMove<moRandomNeutralWalk<NborT>>)
    ;


    class_<moMetropolisHasting<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moMetropolisHasting",name).c_str(),init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned int
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned int,
        moContinuator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,6
        >>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned,
        moContinuator<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,6,
        with_custodian_and_ward<1,7,
        with_custodian_and_ward<1,8
        >>>>>>()
    ]
    )
    .def("set_move",setMove<moMetropolisHasting<NborT>>)
    ;


    class_<moSA<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moSA",name).c_str(),init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,optional<
        double,
        double,
        unsigned,
        double>
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4
        >>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        moCoolingSchedule<SolutionType>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,5
        >>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        moCoolingSchedule<SolutionType>&,
        moContinuator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,5,
        with_custodian_and_ward<1,6
        >>>>>()
    ]
    )
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        moCoolingSchedule<SolutionType>&,
        moSolNeighborComparator<NborT>&,
        moContinuator<NborT>&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4,
        with_custodian_and_ward<1,5,
        with_custodian_and_ward<1,6,
        with_custodian_and_ward<1,7
        >>>>>>()
    ]
    )
    .def("set_move",setMove<moSA<NborT>>)
    ;


    class_<moTS<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moTS",name).c_str(),init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned int,
        unsigned int
    >())
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        unsigned int,
        moTabuList<NborT>&
    >())
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        moContinuator<NborT>&,
        moTabuList<NborT>&,
        moAspiration<NborT>&
    >())
    .def(init<
        moNeighborhood<NborT>&,
        eoEvalFunc<SolutionType>&,
        moEval<NborT>&,
        moNeighborComparator<NborT>&,
        moSolNeighborComparator<NborT>&,
        moContinuator<NborT>&,
        moTabuList<NborT>&,
        moIntensification<NborT>&,
        moDiversification<NborT>&,
        moAspiration<NborT>&
    >())
    .def("set_move",setMove<moTS<NborT>>)
    ;


    class_<moILS<NborT,NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moILS",name).c_str(),init<
        moLocalSearch<NborT>&,
        eoEvalFunc<SolutionType>&,
        eoMonOp<SolutionType>&,
        unsigned int
    >())
    .def(init<
        moLocalSearch<NborT>&,
        eoEvalFunc<SolutionType>&,
        eoMonOp<SolutionType>&,
        moContinuator<NborT>&
    >())
    .def(init<
        moLocalSearch<NborT>&,
        eoEvalFunc<SolutionType>&,
        moContinuator<NborT>&,
        moPerturbation<NborT>&,
        moAcceptanceCriterion<NborT>&
    >())
    ;


    class_<moVNS<NborT>,bases<moLocalSearch<NborT>>,boost::noncopyable>
    (make_name("moVNS",name).c_str(),init<
        moVariableNeighborhoodSelection<SolutionType>&,
        moAcceptanceCriterion<NborT>&,
        eoEvalFunc<SolutionType>&,
        moContinuator<NborT>&
    >())
    ;
}


void moAlgos()
{
    expose_moAlgos<PyEOT>("");
    expose_moAlgos<BinarySolution>("Bin");
    expose_moAlgos<RealSolution>("Real");
    expose_moAlgos<IntSolution>("Perm");
}
