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

/***
//
// #define WRAP(X) struct XWrap : X<PyNeighbor>,wrapper<X<PyNeighbor>>\
//     {\
//     XWrap(moNeighborhood<PyNeighbor>& _nhood,eoEvalFunc<PyEOT>& _eval,moEval<PyNeighbor>& _nborEval) : X<PyNeighbor>(_nhood,_eval,_nborEval){}\
//     XWrap(moNeighborhood<PyNeighbor>& _nhood,eoEvalFunc<PyEOT>& _eval,moEval<PyNeighbor>& _nborEval,moContinuator<PyNeighbor>& _cont) : X<PyNeighbor>(_nhood,_eval,_nborEval,_cont){}\
//     XWrap(moNeighborhood<PyNeighbor>& _nhood,eoEvalFunc<PyEOT>& _eval,moEval<PyNeighbor>& _nborEval,moContinuator<PyNeighbor>& _cont,moNeighborComparator<PyNeighbor>& _compN,moSolNeighborComparator<PyNeighbor>& _compSN) : X<PyNeighbor>(_nhood,_eval,_nborEval,_cont, _compN, _compSN){}\
//     void setMove(boost::python::object _obj){\
//         explorer.getSelectedNeighbor().setMove(_obj);\
//         explorer.getCurrentNeighbor().setMove(_obj);}\
//     void setMoveBack(boost::python::object _obj){\
//         explorer.getSelectedNeighbor().setMoveBack(_obj);\
//         explorer.getCurrentNeighbor().setMoveBack(_obj);}};
//
// WRAP(moSimpleHC)
*///


//
struct moSimpleHCWrap : moSimpleHC<PyNeighbor>,wrapper<moSimpleHC<PyNeighbor>>
{
    //just the C++ equivalent... (need to set a move after calling ctor)
    moSimpleHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval
    ) : moSimpleHC<PyNeighbor>(_nhood,_eval,_nborEval)
    {
    }

    moSimpleHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont
    ) : moSimpleHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    {
    }

    moSimpleHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont,
        moNeighborComparator<PyNeighbor>& _compN,
        moSolNeighborComparator<PyNeighbor>& _compSN
    ) : moSimpleHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont, _compN, _compSN)
    {
    }

    // //with a move
    // moSimpleHCWrap(
    //     moNeighborhood<PyNeighbor>& _nhood,
    //     eoEvalFunc<PyEOT>& _eval,
    //     moEval<PyNeighbor>& _nborEval,
    //     boost::python::object _move
    // ) : moSimpleHC<PyNeighbor>(_nhood,_eval,_nborEval)
    // {
    //     setMove(_move);
    // }
    //
    // moSimpleHCWrap(
    //     moNeighborhood<PyNeighbor>& _nhood,
    //     eoEvalFunc<PyEOT>& _eval,
    //     moEval<PyNeighbor>& _nborEval,
    //     moContinuator<PyNeighbor>& _cont,
    //     boost::python::object _move
    // ) : moSimpleHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    // {
    //     setMove(_move);
    //     // explorer.getSelectedNeighbor().setMove(_move);
    //     // explorer.getCurrentNeighbor().setMove(_move);
    // }

    void setMove(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMove(_obj);
        explorer.getCurrentNeighbor().setMove(_obj);
    }
    void setMoveBack(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMoveBack(_obj);
        explorer.getCurrentNeighbor().setMoveBack(_obj);
    }
};


struct moFirstImprHCWrap : moFirstImprHC<PyNeighbor>,wrapper<moFirstImprHC<PyNeighbor>>
{
    //just the C++ equivalent... (need to set a move after calling ctor)
    moFirstImprHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval
    ) : moFirstImprHC<PyNeighbor>(_nhood,_eval,_nborEval)
    {
    }

    moFirstImprHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont
    ) : moFirstImprHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    {
    }

    moFirstImprHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont,
        moNeighborComparator<PyNeighbor>& _compN,
        moSolNeighborComparator<PyNeighbor>& _compSN
    ) : moFirstImprHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont,_compN,_compSN)
    {
    }

    //with a move (Python object)
    moFirstImprHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        boost::python::object _move
    ) : moFirstImprHC<PyNeighbor>(_nhood,_eval,_nborEval)
    {
        setMove(_move);
    }

    moFirstImprHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont,
        boost::python::object _move
    ) : moFirstImprHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    {
        setMove(_move);
    }

    void setMove(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMove(_obj);
        explorer.getCurrentNeighbor().setMove(_obj);
    }
    void setMoveBack(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMoveBack(_obj);
        explorer.getCurrentNeighbor().setMoveBack(_obj);
    }
};


struct moRandomBestHCWrap : moRandomBestHC<PyNeighbor>,wrapper<moRandomBestHC<PyNeighbor>>
{
    //just the C++ equivalent... (need to set a move after calling ctor)
    moRandomBestHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval
    ) : moRandomBestHC<PyNeighbor>(_nhood,_eval,_nborEval)
    {
    }
    moRandomBestHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont
    ) : moRandomBestHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    {
    }
    moRandomBestHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont,
        moNeighborComparator<PyNeighbor>& _compN,
        moSolNeighborComparator<PyNeighbor>& _compSN
    ) : moRandomBestHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont,_compN,_compSN)
    {
    }
    //with a move (Python object)
    moRandomBestHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        boost::python::object _move
    ) : moRandomBestHC<PyNeighbor>(_nhood,_eval,_nborEval)
    {
        setMove(_move);
    }
    moRandomBestHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        moContinuator<PyNeighbor>& _cont,
        boost::python::object _move
    ) : moRandomBestHC<PyNeighbor>(_nhood,_eval,_nborEval,_cont)
    {
        setMove(_move);
    }
    void setMove(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMove(_obj);
        explorer.getCurrentNeighbor().setMove(_obj);
    }
    void setMoveBack(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMoveBack(_obj);
        explorer.getCurrentNeighbor().setMoveBack(_obj);
    }
};

struct moNeutralHCWrap : moNeutralHC<PyNeighbor>,wrapper<moNeutralHC<PyNeighbor>>
{
    //just the C++ equivalent... (need to set a move after calling ctor)
    moNeutralHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        unsigned int _nbStep
    ) : moNeutralHC<PyNeighbor>(_nhood,_eval,_nborEval,_nbStep)
    {
    }
    moNeutralHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        unsigned int _nbStep,
        moContinuator<PyNeighbor>& _cont
    ) : moNeutralHC<PyNeighbor>(_nhood,_eval,_nborEval,_nbStep,_cont)
    {
    }
    moNeutralHCWrap(
        moNeighborhood<PyNeighbor>& _nhood,
        eoEvalFunc<PyEOT>& _eval,
        moEval<PyNeighbor>& _nborEval,
        unsigned int _nbStep,
        moContinuator<PyNeighbor>& _cont,
        moNeighborComparator<PyNeighbor>& _compN,
        moSolNeighborComparator<PyNeighbor>& _compSN
    ) : moNeutralHC<PyNeighbor>(_nhood,_eval,_nborEval,_nbStep,_cont,_compN,_compSN)
    {
    }
    void setMove(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMove(_obj);
        explorer.getCurrentNeighbor().setMove(_obj);
    }
    void setMoveBack(boost::python::object _obj)
    {
        explorer.getSelectedNeighbor().setMoveBack(_obj);
        explorer.getCurrentNeighbor().setMoveBack(_obj);
    }
};


#define DEF(X,Y) class_<X, bases<moLocalSearch<PyNeighbor>>,    boost::noncopyable>(#Y,init<moNeighborhood<PyNeighbor>&,eoEvalFunc<PyEOT>&,moEval<PyNeighbor>&>()[WC3])\
        .def(init<moNeighborhood<PyNeighbor>&,eoEvalFunc<PyEOT>&,moEval<PyNeighbor>&,moContinuator<PyNeighbor>&>()[WC4])\
        .def(init<moNeighborhood<PyNeighbor>&,eoEvalFunc<PyEOT>&,moEval<PyNeighbor>&,moContinuator<PyNeighbor>&,moNeighborComparator<PyNeighbor>&,moSolNeighborComparator<PyNeighbor>&>()[WC6])\
        .def("setMove",&X::setMove)\
        .def("setMoveBack",&X::setMoveBack)


template<typename X>
void setMove(X& _ls, boost::python::object _obj)
{
    _ls.getNeighborhoodExplorer().getSelectedNeighbor().setMove(_obj);
    _ls.getNeighborhoodExplorer().getCurrentNeighbor().setMove(_obj);
}

// template void setMove<moRandomWalk<PyNeighbor>>();


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

    DEF(moSimpleHCWrap,moSimpleHC);
    DEF(moFirstImprHCWrap,moFirstImprHC);
    DEF(moRandomBestHCWrap,moRandomBestHC);

    class_<moNeutralHCWrap, bases<moLocalSearch<PyNeighbor>>, boost::noncopyable>
    ("moNeutralHC",
        init<
            moNeighborhood<PyNeighbor>&,
            eoEvalFunc<PyEOT>&,
            moEval<PyNeighbor>&,
            unsigned int
        >()
        [WC3]
    )
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        moContinuator<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        moContinuator<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >()
    )
    .def("setMove",&moNeutralHCWrap::setMove)
    ;

    //Random
    class_<moRandomSearch<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomSearch",init<
        eoInit<PyEOT>&,
        eoEvalFunc<PyEOT>&,
        unsigned
    >())
    .def(init<
        eoInit<PyEOT>&,
        eoEvalFunc<PyEOT>&,
        unsigned,
        moContinuator<PyNeighbor>&
    >())
    ;

    class_<moRandomWalk<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomWalk",init<
    moNeighborhood<PyNeighbor>&,
    eoEvalFunc<PyEOT>&,
    moEval<PyNeighbor>&,
    unsigned
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moContinuator<PyNeighbor>&
    >())
    .def("setMove",setMove<moRandomWalk<PyNeighbor>>)
    ;

    class_<moRandomNeutralWalk<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moRandomNeutralWalk",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >())
    .def("setMove",setMove<moRandomNeutralWalk<PyNeighbor>>)
    ;



    class_<moMetropolisHasting<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moMetropolisHasting",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned int,
        moContinuator<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        unsigned,
        moContinuator<PyNeighbor>&,
        moNeighborComparator<PyNeighbor>&,
        moSolNeighborComparator<PyNeighbor>&
    >())
    ;

    class_<moSA<PyNeighbor>,bases<moLocalSearch<PyNeighbor>>,boost::noncopyable>
    ("moSA",init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        double,
        double,
        unsigned,
        double
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&,
        moContinuator<PyNeighbor>&
    >())
    .def(init<
        moNeighborhood<PyNeighbor>&,
        eoEvalFunc<PyEOT>&,
        moEval<PyNeighbor>&,
        moCoolingSchedule<PyEOT>&,
        moSolNeighborComparator<PyNeighbor>&,
        moContinuator<PyNeighbor>&
    >())
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
