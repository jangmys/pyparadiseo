#include <eoEvalFunc.h>
#include <eoInit.h>
#include <eoTransform.h>
#include <eoSGATransform.h>
#include <eoPopEvalFunc.h>

#include <utils/def_abstract_functor.h>
#include <pyeot.h>

using namespace boost::python;


void
transform()
{
    // eoUF : eoPop<PyMOEO> ---> void
    def_abstract_functor<eoTransform<PyEOT> >("eoTransform");

    class_<eoSGATransform<PyEOT>, bases<eoTransform<PyEOT> > >
        ("eoSGATransform",
        init<
            eoQuadOp<PyEOT>&,
            double,
            eoMonOp<PyEOT>&,
            double
        >()
        )
        .def("__call__", &eoSGATransform<PyEOT>::operator ())
        ;
    //
    // def_abstract_functor<eoPopEvalFunc<PyMOEO> >("eoPopEvalFunc");
    //
    // class_<eoPopLoopEval<PyMOEO>, bases<eoPopEvalFunc<PyMOEO> > >
    // (
    //     "eoPopLoopEval",
    //     init<
    //         eoEvalFunc<PyMOEO>&
    //     >()
    // )
    // .def("__call__", &eoPopLoopEval<PyMOEO>::operator ());
}
