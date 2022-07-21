#include <pyeot.h>

#include <algo/moeoNSGA.h>
#include <algo/moeoNSGAII.h>
#include <algo/moeoEasyEA.h>
#include <algo/moeoMOGA.h>
#include <algo/moeoSPEA2.h>
#include <algo/moeoSEEA.h>
#include <algo/moeoUnifiedDominanceBasedLS.h>

#include <utils/def_abstract_functor.h>
// #include "def_abstract_functor.h"






template <typename SolutionType>
void moeo_expose_algos(std::string stype)
{
    using namespace boost::python;

    // why is noncopyable needed? (else compilation error, does it hurt?)
    // ==========================================================================
    class_<moeoNSGA<SolutionType>, bases<moeoEA<SolutionType>>, boost::noncopyable>
        (make_name("moeoNSGA",stype).c_str(), init<
          unsigned int,
          eoEvalFunc<SolutionType>&,
          eoQuadOp<SolutionType>&,
          double,
          eoMonOp<SolutionType>&,
          double,
          optional<double>
      >()
      [
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6
          >     >    >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&,
          optional<double>
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          >     >    >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&,
          optional<double>
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          >     >    >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&,
          optional<double>
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          >     >    >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&,
          optional<double>
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          >     >    >()
      ]
    )
    .def("__call__", &moeoNSGA<SolutionType>::operator ());

    // ==========================================================================
    class_<moeoNSGAII<SolutionType>, bases<moeoEA<SolutionType> >, boost::noncopyable>
        (make_name("moeoNSGAII",stype).c_str(),
      init<
          unsigned int,
          eoEvalFunc<SolutionType>&,
          eoQuadOp<SolutionType>&,
          double,
          eoMonOp<SolutionType>&,
          double
      >()
      [
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6
          > > >()
      ]
        )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &moeoNSGAII<SolutionType>::operator ())
    ;

    // ==========================================================================
    class_<moeoEasyEA<SolutionType>, bases<moeoEA<SolutionType> > >
        (make_name("moeoEasyEA",stype).c_str(),
      init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoBreed<SolutionType>&,
          moeoReplacement<SolutionType>&,
          moeoFitnessAssignment<SolutionType>&,
          moeoDiversityAssignment<SolutionType>&,
          bool
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 5,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 7
          > > > > > >()
      ]
        )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoBreed<SolutionType>&,
          moeoReplacement<SolutionType>&,
          moeoFitnessAssignment<SolutionType>&,
          moeoDiversityAssignment<SolutionType>&,
          bool
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 5,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 7
          > > > > > >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoSelect<SolutionType>&,
          eoTransform<SolutionType>&,
          moeoReplacement<SolutionType>&,
          moeoFitnessAssignment<SolutionType>&,
          moeoDiversityAssignment<SolutionType>&,
          bool
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 5,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 7,
          with_custodian_and_ward<1, 8
          > > > > > > >()
      ]
    )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoSelect<SolutionType>&,
          eoTransform<SolutionType>&,
          moeoReplacement<SolutionType>&,
          moeoFitnessAssignment<SolutionType>&,
          moeoDiversityAssignment<SolutionType>&,
          bool
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 5,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 7,
          with_custodian_and_ward<1, 8
          > > > > > > >()
      ]
    )
    .def("__call__", &moeoEasyEA<SolutionType>::operator ())
    ;


    // why is noncopyable needed? (else compilation error, does it hurt?)
    // ==========================================================================
    class_<moeoMOGA<SolutionType>, bases<moeoEA<SolutionType> >, boost::noncopyable>
        (make_name("moeoMOGA",stype).c_str(),
      init<
          unsigned int,
          eoEvalFunc<SolutionType>&,
          eoQuadOp<SolutionType>&,
          double,
          eoMonOp<SolutionType>&,
          double,
          double
      >()
      [
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6
          > > >()
      ]
        )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&,
          double
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
  )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoGenOp<SolutionType>&,
          double
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
  )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&,
          double
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
  )
    .def(init<
          eoContinue<SolutionType>&,
          eoPopEvalFunc<SolutionType>&,
          eoTransform<SolutionType>&,
          double
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &moeoMOGA<SolutionType>::operator ())
    ;

    // ==========================================================================
    class_<moeoSPEA2<SolutionType>, bases<moeoEA<SolutionType> >, boost::noncopyable>
        (make_name("moeoSPEA2",stype).c_str(),
      init<
          unsigned int,
          eoEvalFunc<SolutionType>&,
          eoQuadOp<SolutionType>&,
          double,
          eoMonOp<SolutionType>&,
          double,
          moeoArchive<SolutionType>&,
          optional<unsigned int,bool>
      >()
      [
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 8
          > > > >()
      ]
        )
    .def(init<
          eoContinue<SolutionType>&,
          eoEvalFunc<SolutionType>&,
          eoQuadOp<SolutionType>&,
          double,
          eoMonOp<SolutionType>&,
          double,
          moeoArchive<SolutionType>&,
          optional<unsigned int,bool>
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 8
          > > > > >()
      ])
    .def(init<
              eoContinue<SolutionType>&,
              eoEvalFunc<SolutionType>&,
              eoTransform<SolutionType>&,
              moeoArchive<SolutionType>&,
              optional<unsigned int,bool>
          >()
          [
              with_custodian_and_ward<1, 2,
              with_custodian_and_ward<1, 3,
              with_custodian_and_ward<1, 4,
              with_custodian_and_ward<1, 5
              > > > >()
          ])
    .def(init<
            eoContinue<SolutionType>&,
            eoPopEvalFunc<SolutionType>&,
            eoTransform<SolutionType>&,
            moeoArchive<SolutionType>&,
            optional<unsigned int,bool>
        >()
        [
            with_custodian_and_ward<1, 2,
            with_custodian_and_ward<1, 3,
            with_custodian_and_ward<1, 4,
            with_custodian_and_ward<1, 5
            > > > >()
        ])

    .def("__call__", &moeoSPEA2<SolutionType>::operator ())
    ;

    class_<moeoSEEA<SolutionType>, bases<moeoEA<SolutionType> >, boost::noncopyable>
        (make_name("moeoSEEA",stype).c_str(), init<
          unsigned int,
          eoEvalFunc<SolutionType> &,
          eoQuadOp<SolutionType> &,
          double,
          eoMonOp<SolutionType> &,
          double,
          moeoArchive<SolutionType> &
      >()
      [
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4,
          with_custodian_and_ward<1, 6,
          with_custodian_and_ward<1, 8
          > > > >()
      ]
    )
    .def(init<
            eoContinue<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            eoGenOp<SolutionType>&,
            moeoArchive<SolutionType>&
        >()
        [
            with_custodian_and_ward<1, 2,
            with_custodian_and_ward<1, 3,
            with_custodian_and_ward<1, 4,
            with_custodian_and_ward<1, 5
            > > > >()
        ])
    .def(init<
            eoContinue<SolutionType>&,
            eoPopEvalFunc<SolutionType>&,
            eoGenOp<SolutionType>&,
            moeoArchive<SolutionType>&
        >()
        [
            with_custodian_and_ward<1, 2,
            with_custodian_and_ward<1, 3,
            with_custodian_and_ward<1, 4,
            with_custodian_and_ward<1, 5
            > > > >()
        ])
    .def(init<
            eoContinue<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            eoTransform<SolutionType>&,
            moeoArchive<SolutionType>&
        >()
        [
            with_custodian_and_ward<1, 2,
            with_custodian_and_ward<1, 3,
            with_custodian_and_ward<1, 4,
            with_custodian_and_ward<1, 5
            > > > >()
        ])
    .def("__call__", &moeoSEEA<SolutionType>::operator ())
    ;


    //missing MOEO neighborhood
  //   class_<moeoUnifiedDominanceBasedLS<SolutionType>, boost::noncopyable>
  //       ("moeoUnifiedDominanceBasedLS", init<
  //         eoContinue<SolutionType>&,
  //         eoEvalFunc<SolutionType>&,
  //         moeoArchive<SolutionType>&,
  //         moeoPopNeighborhoodExplorer<SolutionType>&,
  //         moeoUnvisitedSelect<SolutionType>&
  //     >()
  //     [
  //         with_custodian_and_ward<1, 2,
  //         with_custodian_and_ward<1, 3,
  //         with_custodian_and_ward<1, 4,
  //         with_custodian_and_ward<1, 5,
  //         with_custodian_and_ward<1, 6
  //         > > > > >()
  //     ]
  // )
  //   .def("__call__", &moeoUnifiedDominanceBasedLS<SolutionType>::operator())
  //   ;






}

void moeo_algos()
{
    moeo_expose_algos<PyEOT>("");
    moeo_expose_algos<BinarySolution>("Bin");
    moeo_expose_algos<RealSolution>("Real");
}
