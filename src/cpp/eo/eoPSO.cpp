#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <eoEvalFunc.h>
#include <eoOpContainer.h>
#include <eoGenOp.h>

// #include <eoSGA.h>
// #include <eoEasyEA.h>
// #include <eoFastGA.h>

#include <boost/python.hpp>
#include <pyeot.h>

#include <pypot.h>

#include <eoVelocity.h>
#include <eoStandardVelocity.h>

#include <eoFlight.h>
#include <eoStandardFlight.h>


#include <eoPSO.h>
#include <eoEasyPSO.h>

#include <eoTopology.h>
#include <eoLinearTopology.h>

#include <eoSyncEasyPSO.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;




// struct eoPSOWrap : eoPSO<PyPOT>,wrapper<eoPSO<PyPOT>>
// {
// public:
//     void operator()(eoPop<PyPOT>& _pop)
//     {
//         this->get_override("operator()")(_pop);
//     }
// };
typedef VectorParticle<double> POT;

struct eoFlightWrap : eoFlight<POT>,wrapper<eoFlight<POT>>
{
public:
    void operator()(POT& _part)
    {
        this->get_override("operator()")(_part);
    }
};

struct eoVelocityWrap : eoVelocity<POT>,wrapper<eoVelocity<POT>>
{
public:
    void operator()(POT& _p,unsigned _i)
    {
        this->get_override("operator()")(_p,_i);
    }

    eoTopology<POT>& getTopology()
    {
        return this->get_override("getTopology")();
    }
};


struct eoTopologyWrap : eoTopology<POT>,wrapper<eoTopology<POT>>
{
    eoTopologyWrap() : eoTopology<POT>(){}


    void setup(const eoPop<POT>& _pop)
    {
        this->get_override("setup")(_pop);
    }

    void updateNeighborhood(POT& p, unsigned i)
    {
        this->get_override("updateNeighborhood")(p,i);
    }

    // void updateNeighborhood(eoPop<POT>& _pop)
    // {
    //     this->get_override("updateNeighborhood")(_pop);
    // }

    POT& best(unsigned i)
    {
        return this->get_override("best")(i);
    }

    POT& globalBest()
    {
        return this->get_override("globalBest")();
    }
};

// void testModifSolution(PyEOT& _sol)
// {
//     for(unsigned i=0;i<_sol.size();i++)
//     {
//         _sol[i] += 1.0;
//     }
// }

void eoParticleSwarm(){
    //ABC
    class_< eoFlightWrap,boost::noncopyable >
    ("eoFlight",init<>())
    .def("__call__", pure_virtual(&eoFlightWrap::operator()))
    ;

    class_< eoVelocityWrap,boost::noncopyable >
    ("eoVelocity",init<>())
    .def("__call__", pure_virtual(&eoVelocityWrap::operator()))
    .def("getTopology", pure_virtual(&eoVelocityWrap::getTopology),return_internal_reference<>())
    // .def("updateNeighborhood")
    ;

    class_< eoTopologyWrap,boost::noncopyable >
    ("eoTopology",init<>())
    .def("setup", pure_virtual(&eoTopologyWrap::setup))
    .def("updateNeighborhood", pure_virtual(&eoTopologyWrap::updateNeighborhood))
    // .def("updateNeighborhood", &eoTopologyWrap::updateNeighborhood))
    .def("best", pure_virtual(&eoTopologyWrap::best),return_internal_reference<>())
    .def("globalBest", pure_virtual(&eoTopologyWrap::globalBest),return_internal_reference<>())
    ;

    //virtual
    // class_<eoInitializer<POT>>("eoInitializer",
    //     init<
    //         eoPopEvalFunc<POT>&,
    //         eoVelocityInit<POT>&,
    //         eoParticleBestInit<POT>&,
    //         eoTopology<POT>&,
    //         eoPop<POT>&
    //     >()
    //     [
    //         with_custodian_and_ward<1,2,
    //         with_custodian_and_ward<1,3,
    //         with_custodian_and_ward<1,4,
    //         with_custodian_and_ward<1,5,
    //         with_custodian_and_ward<1,6
    //         >>>>>()
    //     ]
    // )
    // ;



    //===============================
    // CONCRETE CLASSES
    //===============================
    class_<eoStandardFlight<POT>,bases<eoFlight<POT>>>("eoStandardFlight",init<>())
    .def(init<eoRealVectorBounds&>()[WC1])
    .def(init<const unsigned,const double,const double>())
    .def("__call__",&eoStandardFlight<POT>::operator())
    ;

    class_<eoStandardVelocity<POT>,bases<eoVelocity<POT>>>("eoStandardVelocity",
    init<
        eoTopology<POT>&,
        const double,
        const double,
        const double,
        eoRealVectorBounds&
    >()
    [
        with_custodian_and_ward<1,2,
        with_custodian_and_ward<1,6
        >>()
    ]
    )
    .def(
        init<
            eoTopology<POT>&,
            const double,
            const double,
            const double,
            eoRng&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,6
            >>()
        ]
    )
    .def(
        init<
            eoTopology<POT>&,
            const double,
            const double,
            const double
        >()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def("__call__",&eoStandardVelocity<POT>::operator())
    .def("updateNeighborhood",&eoStandardVelocity<POT>::updateNeighborhood)
    .def("getTopology",&eoStandardVelocity<POT>::getTopology,return_internal_reference<>())
    ;


    class_<eoLinearTopology<POT>,bases<eoTopology<POT>>>("eoLinearTopology",init<unsigned>())
    .def("setup",&eoLinearTopology<POT>::setup)
    .def("updateNeighborhood",&eoLinearTopology<POT>::updateNeighborhood)
    .def("best",&eoLinearTopology<POT>::best,return_internal_reference<>())
    .def("globalBest",&eoLinearTopology<POT>::globalBest,return_internal_reference<>())
    ;


    class_<eoEasyPSO<POT>>("eoEasyPSO",
        init<
            eoContinue<POT>&,
            eoEvalFunc<POT>&,
            eoVelocity<POT>&
        >()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4
            >>>()
        ]
    )
    .def("__call__",&eoEasyPSO<POT>::operator())
    // .def(
    //     init<
    //     eoInitializerBase<POT>&,
    //     eoContinue<POT>&,
    //     eoEvalFunc<POT>&,
    //     eoVelocity<POT>&,
    //     eoFlight<POT>&
    //     >()
    //     [
    //     with_custodian_and_ward<1,2,
    //     with_custodian_and_ward<1,3,
    //     with_custodian_and_ward<1,4,
    //     with_custodian_and_ward<1,5,
    //     with_custodian_and_ward<1,6
    //     >>>>>()
    //     ]
    // )
    ;
}
