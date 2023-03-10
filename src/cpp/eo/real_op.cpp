#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include "operators/uniform_mutation.h"
#include "operators/det_uniform_mutation.h"
#include "operators/segment_crossover.h"
#include "operators/hypercube_crossover.h"
#include "operators/SBX_crossover.h"
#include "operators/eoPolynomialMutation.h"
#include "operators/eoNPtsCrossover.h"

#include <es/eoRealOp.h>
#include <pyeot.h>
#include <pso/pypot.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <utils/def_abstract_functor.h>

namespace p  = boost::python;
namespace np = boost::python::numpy;

void
real_op()
{
    using namespace boost::python;

    class_<eoUniformMutation<RealSolution>, bases<eoMonOp<RealSolution> >, boost::noncopyable>
        ("UniformMutation", init<
          const double,
          optional<const double>
      >((arg("epsilon"),arg("p_change")))
        )
    .def(init<
          eoRealVectorBounds&,
          const double,
          optional<const double>
      >((arg("bounds"),arg("epsilon"),arg("p_change")))
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def(init<
          eoRealVectorBounds&,
          const std::vector<double>&,
          const std::vector<double>&
      >((arg("bounds"),arg("epsilons"),arg("p_changes")))
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &eoUniformMutation<RealSolution>::operator ())
    ;


    class_<eoDetUniformMutation<RealSolution>, bases<eoMonOp<RealSolution> >, boost::noncopyable>
        ("DetUniformMutation", init<
          const double&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3
          > >()
      ]
        )
    .def(init<
          eoRealVectorBounds&,
          const double&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoRealVectorBounds&,
          const std::vector<double>&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &eoDetUniformMutation<RealSolution>::operator ())
    ;


    class_<eoPolynomialMutation<RealSolution>, bases<eoMonOp<RealSolution> >, boost::noncopyable>
            ("PolynomialMutation", init<
              optional<const double, const double>
          >((arg("p_change"),arg("eta")))
            )
        .def(init<
              eoRealVectorBounds&,
              optional<const double,const double>
          >((arg("bounds"),arg("p_change"),arg("eta")))
          [
              with_custodian_and_ward<1, 2>()
          ]
        )
        // .def(init<
        //       eoRealVectorBounds&,
        //       const std::vector<double>&,
        //       const std::vector<double>&
        //   >((arg("bounds"),arg("epsilons"),arg("p_changes")))
        //   [
        //       with_custodian_and_ward<1, 2,
        //       with_custodian_and_ward<1, 3,
        //       with_custodian_and_ward<1, 4
        //       > > >()
        //   ]
        // )
        .def("__call__", &eoPolynomialMutation<RealSolution>::operator ())
        ;



    class_<eoTwoPtCrossover<RealSolution>, bases<eoQuadOp<RealSolution> >, boost::noncopyable>
            ("TwoPtCrossover", init<>())
    .def("__call__", &eoTwoPtCrossover<RealSolution>::operator ())
    ;





    class_<eoSegmentCrossover<RealSolution>, bases<eoQuadOp<RealSolution> >, boost::noncopyable>
        ("SegmentCrossover", init<optional<double>>()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
          eoRealVectorBounds&,
          optional<const double>
      >()
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def("__call__", &eoSegmentCrossover<RealSolution>::operator ())
    ;


    class_<eoHypercubeCrossover<RealSolution>, bases<eoQuadOp<RealSolution> >, boost::noncopyable>
        ("HypercubeCrossover", init<optional<double>>()
        [
        with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
        eoRealVectorBounds&,
        optional<const double>
        >()
        [
        with_custodian_and_ward<1, 2,
        with_custodian_and_ward<1, 3
        >>()
        ]
    )
    .def("__call__", &eoHypercubeCrossover<RealSolution>::operator ())
    ;


    class_<eoSBXCrossover<RealSolution>, bases<eoQuadOp<RealSolution> >, boost::noncopyable>
    ("SBXCrossover", init<optional<const double>>())
    .def(init<
        eoRealVectorBounds&,
        optional<const double>
        >()
        [
        with_custodian_and_ward<1, 2>()
        ]
    )
    .def("__call__", &eoSBXCrossover<RealSolution>::operator ())
    ;


    class_<eoRealUXover<RealSolution>, bases<eoQuadOp<RealSolution> >, boost::noncopyable>
    ("RealUXover", init<optional<const float>>())
    .def("__call__", &eoRealUXover<RealSolution>::operator ())
    ;


} // real_op
