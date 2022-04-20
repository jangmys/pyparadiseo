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

#include <eoFlight.h>
#include <eoStandardFlight.h>

#include <eoPSO.h>
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

struct eoFlightWrap : eoFlight<PyEOT>,wrapper<eoFlight<PyEOT>>
{
public:
    void operator()(PyEOT& _part)
    {
        this->get_override("operator()")(_part);
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
    // class_<RealSolution<PyEOT>,bases<PyEOT>>("RealSolution",init<unsigned int>());
    //
    // class_<BinarySolution,bases<PyEOT>>("BinarySolution",init<unsigned int>());
    //
    // def("testSolution", testModifSolution);

    // class_<eoVectorParticle<double,double,double>>("VectorParticle",init<optional<unsigned,double,double,double>>())
    // ;
    //
    // class_<eoRealParticle<doubleFitness>,bases< eoVectorParticle<double,double,double>
    //     > >("RealParticle",init<optional<unsigned,double,double,double>>())
    // ;

    class_< eoFlightWrap,boost::noncopyable >
    ("eoFlight",init<>())
    .def("__call__", pure_virtual(&eoFlightWrap::operator()))
    ;
    //
    //
    //
    // def_abstract_functor<eoPSO<PyPOT>>("eoPSO","Abstract base class. Unary functor : Population -> void");


    // class_<eoSyncEasyPSO<PyPOT>,bases<eoPSO<PyPOT>>>
    // ("eoSyncEasyPSO",init<
    //     eoInitializerBase<PyPOT> &,
    //     eoContinue < PyPOT > &,
    //     eoEvalFunc < PyPOT > &,
    //     eoVelocity < PyPOT > &,
    //     eoFlight < PyPOT > &
    // >())
    // ;
    //
    //


    // class_<eoStandardFlight<PyPOT>,bases<eoFlight<PyPOT>>>
    // ("eoStandardFlight",init<>())
    // ;
}
