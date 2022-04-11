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


struct moLocalSearchWrap : moLocalSearch<PyNeighbor>,wrapper<moLocalSearch<PyNeighbor>>
{
    moLocalSearchWrap(
        moNeighborhoodExplorer<PyNeighbor>& _nhexpl,
        moContinuator<PyNeighbor>& _cont,
        eoEvalFunc<PyEOT>& _eval
    ) : moLocalSearch<PyNeighbor>(_nhexpl,_cont,_eval){};

    //virtual operator() has a default implementation
    bool operator()(PyEOT& _sol)
    {
        if(override f = this->get_override("operator()"))
        {
            return this->get_override("operator()")(_sol);
        }
        return moLocalSearch<PyNeighbor>::operator()(_sol);
    }
    bool default_op(PyEOT& _sol){
        return this->moLocalSearch<PyNeighbor>::operator()(_sol);
    }

    void setMove(boost::python::object _obj)
    {
        searchExplorer.getSelectedNeighbor().setMove(_obj);
        searchExplorer.getCurrentNeighbor().setMove(_obj);
    }
    void setMoveBack(boost::python::object _obj)
    {
        searchExplorer.getSelectedNeighbor().setMoveBack(_obj);
        searchExplorer.getCurrentNeighbor().setMoveBack(_obj);
    }
};



template<typename X>
void setMove(X& _ls, boost::python::object _obj)
{
    _ls.getNeighborhoodExplorer().getSelectedNeighbor().setMove(_obj);
    _ls.getNeighborhoodExplorer().getCurrentNeighbor().setMove(_obj);
}

template<typename X>
void setMoveBack(X& _ls, boost::python::object _obj)
{
    _ls.getNeighborhoodExplorer().getSelectedNeighbor().setMoveBack(_obj);
    _ls.getNeighborhoodExplorer().getCurrentNeighbor().setMoveBack(_obj);
}



void moAlgos()
{
    class_<moLocalSearchWrap,boost::noncopyable>
    ("moLocalSearch",
        init<
            moNeighborhoodExplorer<PyNeighbor>&,
            moContinuator<PyNeighbor>&,
            eoEvalFunc<PyEOT>&
        >()
        [WC3]
    )
    .def("__call__",&moLocalSearch<PyNeighbor>::operator(),&moLocalSearchWrap::default_op)
    .def("getNeighborhoodExplorer",&moLocalSearch<PyNeighbor>::getNeighborhoodExplorer,return_internal_reference<>())
    .def("setMove",&moLocalSearchWrap::setMove)
    .def("setMoveBack",&moLocalSearchWrap::setMoveBack)
    ;



    class_<moSimpleHC<PyNeighbor>, bases<moLocalSearch<PyNeighbor>>, boost::noncopyable>
    ("moSimpleHC",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&,
            moNeighborComparator<PyNeighbor>&,
            moSolNeighborComparator<PyNeighbor>&
        >()[WC6]
    )
    .def("setMove",setMove<moSimpleHC<PyNeighbor>>)
    .def("setMoveBack",setMoveBack<moSimpleHC<PyNeighbor>>)
    ;



    class_<moFirstImprHC<PyNeighbor>, bases<moLocalSearch<PyNeighbor>>, boost::noncopyable>
    ("moFirstImprHC",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&,
            moNeighborComparator<PyNeighbor>&,
            moSolNeighborComparator<PyNeighbor>&
        >()[WC6]
    )
    .def("setMove",setMove<moFirstImprHC<PyNeighbor>>)
    .def("setMoveBack",setMoveBack<moFirstImprHC<PyNeighbor>>)
    ;



    class_<moRandomBestHC<PyNeighbor>, bases<moLocalSearch<PyNeighbor>>, boost::noncopyable>
    ("moRandomBestHC",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&
        >()[WC4]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&,
            moNeighborComparator<PyNeighbor>&,
            moSolNeighborComparator<PyNeighbor>&
        >()[WC6]
    )
    .def("setMove",setMove<moRandomBestHC<PyNeighbor>>)
    .def("setMoveBack",setMoveBack<moRandomBestHC<PyNeighbor>>)
    ;



    class_<moNeutralHC<PyNeighbor>, bases<moLocalSearch<PyNeighbor>>, boost::noncopyable>
    ("moNeutralHC",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            unsigned int
        >()[WC3]
    )
    .def(
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            unsigned int,
            moContinuator<PyNeighbor>&
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
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            unsigned int,
            moContinuator<PyNeighbor>&,
            moNeighborComparator<PyNeighbor>&,
            moSolNeighborComparator<PyNeighbor>&
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
    .def("setMove",setMove<moNeutralHC<PyNeighbor>>)
    .def("setMoveBack",setMoveBack<moNeutralHC<PyNeighbor>>)
    ;


    //Random : What for ???
    class_<moRandomSearch<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomSearch",
        init<
            eoInit<PyEOT>&,
            eoEvalFunc<PyEOT>&,
            unsigned
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3>>()
        ]
    )
    .def(
        init<
            eoInit<PyEOT>&,
            eoEvalFunc<PyEOT>&,
            unsigned,
            moContinuator<PyNeighbor>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,5>>>()
        ]
    )
    ;

    class_<moRandomWalk<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomWalk",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
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
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            moContinuator<PyNeighbor>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4,
            with_custodian_and_ward<1,5>>>>()
        ]
    )
    .def("setMove",setMove<moRandomWalk<PyNeighbor>>)
    ;

    class_<moRandomNeutralWalk<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomNeutralWalk",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4>>>()
    ]
    )
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&
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
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
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
    .def("setMove",setMove<moRandomNeutralWalk<PyNeighbor>>)
    ;



    class_<moMetropolisHasting<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moMetropolisHasting",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,3,
        with_custodian_and_ward<1,4>>>()
    ]
    )
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        moContinuator<PyNeighbor>&
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
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
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
    .def("setMove",setMove<moMetropolisHasting<PyNeighbor>>)
    ;

    class_<moSA<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moSA",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,optional<
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
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&
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
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&,
        moContinuator<PyNeighbor>&
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
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&,
        moSolNeighborComparator<PyNeighbor>&,
        moContinuator<PyNeighbor>&
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
    .def("setMove",setMove<moSA<PyNeighbor>>)
    ;


    class_<moTS<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moTS",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        unsigned int
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        moTabuList<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moContinuator<PyNeighbor>&,
        moTabuList<PyNeighbor>&,
        moAspiration<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&,
        moContinuator<PyNeighbor>&,
        moTabuList<PyNeighbor>&,
        moIntensification<PyNeighbor>&,
        moDiversification<PyNeighbor>&,
        moAspiration<PyNeighbor>&
    >())
    .def("setMove",setMove<moTS<PyNeighbor>>)
    ;


    class_<moILS<PyNeighbor,PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moILS",init<
        moLocalSearch<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        eoMonOp<PyEOT>&,
        unsigned int
    >())
    .def(init<
        moLocalSearch<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        eoMonOp<PyEOT>&,
        moContinuator<PyNeighbor>&
    >())
    .def(init<
        moLocalSearch<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moContinuator<PyNeighbor>&,
        moPerturbation<PyNeighbor>&,
        moAcceptanceCriterion<PyNeighbor>&
    >())
    ;


    class_<moVNS<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moVNS",init<
        moVariableNeighborhoodSelection<PyEOT>&,
        moAcceptanceCriterion<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moContinuator<PyNeighbor>&
    >())
    ;
}
