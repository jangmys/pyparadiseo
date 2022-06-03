#include <eoEvalFunc.h>
#include <eoOpContainer.h>
#include <eoGenOp.h>

#include <eoSGA.h>
#include <eoEasyEA.h>
#include <eoFastGA.h>

#include <boost/python.hpp>
#include <pyeot.h>

#include <pypot.h>
#include <eoFlight.h>
#include <eoPSO.h>
#include <eoSyncEasyPSO.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;


template <class SolutionType>
void
export_algos2(std::string name)
{
    class_<eoSGA<SolutionType>, bases<eoAlgo<SolutionType> >, boost::noncopyable>
        (make_name("eoSGA",name).c_str(), "Simple genetic algorithm.",
      init<
          eoSelectOne<SolutionType>&,
          eoQuadOp<SolutionType>&,
          float,
          eoMonOp<SolutionType>&,
          float,
          eoEvalFunc<SolutionType>&,
          eoContinue<SolutionType>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 5,
          with_custodian_and_ward<1, 7,
          with_custodian_and_ward<1, 8>
          >
          >
          >
          >()
      ]
        )
    .def("__call__", &eoSGA<SolutionType>::operator ())
    ;
}



template<class SolutionType>
void export_algos()
{
    class_<eoEasyEA<SolutionType>, bases<eoAlgo<SolutionType> > >
    ("eoEasyEA","Evolutionary algorithm.",
    init<
    eoContinue<SolutionType>&,
    eoEvalFunc<SolutionType>&,
    eoBreed<SolutionType>&,
    eoReplacement<SolutionType>&
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5
    >
    >
    >
    >()
    ]
    )
    .def( init<
    eoContinue<SolutionType>&,
    eoEvalFunc<SolutionType>&,
    eoBreed<SolutionType>&,
    eoReplacement<SolutionType>&,
    unsigned
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5
    >
    >
    >
    >()
    ]
    )
    .def( init<
    eoContinue<SolutionType>&,
    eoPopEvalFunc<SolutionType>&,
    eoBreed<SolutionType>&,
    eoReplacement<SolutionType>&
    >()
        [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5
    >
    >
    >
    >()
    ]
    )
    .def( init<
    eoContinue<SolutionType>&,
    eoEvalFunc<SolutionType>&,
    eoBreed<SolutionType>&,
    eoMerge<SolutionType>&,
    eoReduce<SolutionType>&
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5,
    with_custodian_and_ward<1,6
    >
    >
    >
    >
    >()
    ]
    )
    .def( init<
    eoContinue<SolutionType>&,
    eoEvalFunc<SolutionType>&,
    eoSelect<SolutionType>&,
    eoTransform<SolutionType>&,
    eoReplacement<SolutionType>&
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5,
    with_custodian_and_ward<1,6
    >
    >
    >
    >
    >()
    ]
    )
    .def( init<
    eoContinue<SolutionType>&,
    eoEvalFunc<SolutionType>&,
    eoSelect<SolutionType>&,
    eoTransform<SolutionType>&,
    eoMerge<SolutionType>&,
    eoReduce<SolutionType>&
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5,
    with_custodian_and_ward<1,6,
    with_custodian_and_ward<1,7
    >
    >
    >
    >
    >
    >()
    ]
    )
    .def("__call__", &eoEasyEA<SolutionType>::operator())
    ;



    class_<eoFastGA<SolutionType>, bases<eoAlgo<SolutionType> > >
    ("eoFastGA","Fast genetic algorithm.",
    init<
        double,
        eoSelectOne<SolutionType>&,
        eoQuadOp<SolutionType>&,
        eoSelectOne<SolutionType>&,
        double,
        eoSelectOne<SolutionType>&,
        eoMonOp<SolutionType>&,
        eoPopEvalFunc<SolutionType>&,
        eoReplacement<SolutionType>&,
        eoContinue<SolutionType>&,
        double
    >()
    [
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,4,
    with_custodian_and_ward<1,5,
    with_custodian_and_ward<1,7,
    with_custodian_and_ward<1,8,
    with_custodian_and_ward<1,9,
    with_custodian_and_ward<1,10,
    with_custodian_and_ward<1,11
    > > > > > > > >()
    ]
    )
    .def("__call__", &eoFastGA<SolutionType>::operator())
    ;

    //////////////////////////////////
}


void eoAlgos()
{
    export_algos2<PyEOT>("");
    export_algos2<BinarySolution>("Bin");
    export_algos2<RealSolution>("Real");


    export_algos<PyEOT>();
}

//
//
// struct eoPSOWrap : eoPSO<PyPOT>,wrapper<eoPSO<PyPOT>>
// {
// public:
//     void operator()(eoPop<PyPOT>& _pop)
//     {
//         this->get_override("operator()")(_pop);
//     }
// };
//
// struct eoFlightWrap : eoFlight<PyPOT>,wrapper<eoFlight<PyPOT>>
// {
// public:
//     void operator()(PyPOT& _part)
//     {
//         this->get_override("operator()")(_part);
//     }
// };
//
//
// void eoParticleSwarm(){
//     def_abstract_functor<eoPSO<PyPOT>>("eoPSO","Abstract base class. Unary functor : Population -> void");
//
//     class_<eoSyncEasyPSO<PyPOT>,bases<eoPSO<PyPOT>>>
//     ("eoSyncEasyPSO",init<
//         eoInitializerBase<PyPOT> &,
//         eoContinue < PyPOT > &,
//         eoEvalFunc < PyPOT > &,
//         eoVelocity < PyPOT > &,
//         eoFlight < PyPOT > &
//     >())
//     ;
//
//
//
//
//     class_< eoFlightWrap >
//     ("eoFlight",init<>())
//     .def("__call__", pure_virtual(&eoFlightWrap::operator()))
//     ;
// }
