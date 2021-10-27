#include <eoEvalFunc.h>
#include <eoOpContainer.h>
#include <eoGenOp.h>

#include <eoSGA.h>
#include <eoEasyEA.h>
#include <eoFastGA.h>
// #include <eoCellularEasyEA.h>

// #include <selection/moeoSelectOne.h>

#include <boost/python.hpp>
#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

void eoAlgos()
{
    def_abstract_functor<eoAlgo<PyEOT> >("eoAlgo");

    class_<eoSGA<PyEOT>, bases<eoAlgo<PyEOT> >, boost::noncopyable>
    ("eoSGA",
    init<
    eoSelectOne<PyEOT>&,
    eoQuadOp<PyEOT>&,
    float,
    eoMonOp<PyEOT>&,
    float,
    eoEvalFunc<PyEOT>&,
    eoContinue<PyEOT>&
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,3,
    with_custodian_and_ward<1,5,
    with_custodian_and_ward<1,7,
    with_custodian_and_ward<1,8>
    >
    >
    >
    >()
    ]
    )
    .def("__call__", &eoSGA<PyEOT>::operator())
    ;



    class_<eoEasyEA<PyEOT>, bases<eoAlgo<PyEOT> > >
    ("eoEasyEA",
    init<
    eoContinue<PyEOT>&,
    eoEvalFunc<PyEOT>&,
    eoBreed<PyEOT>&,
    eoReplacement<PyEOT>&
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
    eoContinue<PyEOT>&,
    eoEvalFunc<PyEOT>&,
    eoBreed<PyEOT>&,
    eoReplacement<PyEOT>&,
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
    eoContinue<PyEOT>&,
    eoPopEvalFunc<PyEOT>&,
    eoBreed<PyEOT>&,
    eoReplacement<PyEOT>&
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
    eoContinue<PyEOT>&,
    eoEvalFunc<PyEOT>&,
    eoBreed<PyEOT>&,
    eoMerge<PyEOT>&,
    eoReduce<PyEOT>&
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
    eoContinue<PyEOT>&,
    eoEvalFunc<PyEOT>&,
    eoSelect<PyEOT>&,
    eoTransform<PyEOT>&,
    eoReplacement<PyEOT>&
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
    eoContinue<PyEOT>&,
    eoEvalFunc<PyEOT>&,
    eoSelect<PyEOT>&,
    eoTransform<PyEOT>&,
    eoMerge<PyEOT>&,
    eoReduce<PyEOT>&
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
    .def("__call__", &eoEasyEA<PyEOT>::operator())
    ;



    class_<eoFastGA<PyEOT>, bases<eoAlgo<PyEOT> > >
    ("eoFastGA",
    init<
        double,
        eoSelectOne<PyEOT>&,
        eoQuadOp<PyEOT>&,
        eoSelectOne<PyEOT>&,
        double,
        eoSelectOne<PyEOT>&,
        eoMonOp<PyEOT>&,
        eoPopEvalFunc<PyEOT>&,
        eoReplacement<PyEOT>&,
        eoContinue<PyEOT>&,
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
    .def("__call__", &eoFastGA<PyEOT>::operator())
    ;

    //////////////////////////////////
}
