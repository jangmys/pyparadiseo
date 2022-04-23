#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include "operators/uniform_mutation.h"
#include "operators/det_uniform_mutation.h"
#include "operators/segment_crossover.h"
#include "operators/hypercube_crossover.h"
#include "operators/SBX_crossover.h"

#include <eoRealOp.h>
#include <pyeot.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

// almost copies from eoRealOp.h .... not nice.
// PROBLEM: can't instantiate classes from eoRealOp unless EOT inherits from std::vector<double> (needs operator[], size(), ...).

void
real_op()
{
    using namespace boost::python;

    /*
     * C++ genetic operators need `double& operator[](int key)` for PyEOT (and size()...)
     *
     * np pb for size(), but I couldn't get lvalue converters to work, so...
     *
     * Solution (for now) : re-define classes as above, assuming that PyEOT.encoding is an ndarray with dtype double.
     *
     * TODO
     */
    class_<PyUniformMutation<PyEOT>, bases<eoMonOp<PyEOT> >, boost::noncopyable>
        ("UniformMutation", init<
          const double,
          optional<const double>
      >()
    )
    .def(init<
          eoRealVectorBounds&,
          const double,
          optional<const double>
      >()
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def(init<
          eoRealVectorBounds&,
          const std::vector<double>&,
          const std::vector<double>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &PyUniformMutation<PyEOT>::operator ())
    ;


    class_<PyDetUniformMutation<PyEOT>, bases<eoMonOp<PyEOT> >, boost::noncopyable>
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
    .def("__call__", &PyDetUniformMutation<PyEOT>::operator ())
    ;


    class_<PySegmentCrossover<PyEOT>, bases<eoQuadOp<PyEOT> >, boost::noncopyable>
        ("SegmentCrossover", init<double>()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
          eoRealVectorBounds&,
          const double
      >()
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def("__call__", &PySegmentCrossover<PyEOT>::operator ())
    ;


    class_<PyHypercubeCrossover<PyEOT>, bases<eoQuadOp<PyEOT> >, boost::noncopyable>
        ("HypercubeCrossover", init<double&>()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
          eoRealVectorBounds&,
          const double&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3
          >>()
      ]
    )
    .def("__call__", &PyHypercubeCrossover<PyEOT>::operator ())
    ;

    class_<eoSBXCrossover<PyEOT>, bases<eoQuadOp<PyEOT> >, boost::noncopyable>
        ("SBXCrossover", init<double>()
    )
    .def(init<
          eoRealVectorBounds&,
          optional<const double>
      >()
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def("__call__", &eoSBXCrossover<PyEOT>::operator ())
    ;



} // real_op
