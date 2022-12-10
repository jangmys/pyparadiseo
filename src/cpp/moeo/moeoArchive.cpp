#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <eoPop.h>

#include <archive/moeoArchive.h>
#include <archive/moeo2DMinHypervolumeArchive.h>
// #include <archive/moeoBoundedArchive.h>
// #include <archive/moeoEpsilonHyperboxArchive.h>
#include <archive/moeoUnboundedArchive.h>

using namespace boost::python;

template <typename SolutionType>
struct moeoArchiveWrap : public moeoArchive<SolutionType>,wrapper<moeoArchive<SolutionType>>
{
    using moeoArchive<SolutionType>::update;

    bool operator()(const SolutionType& _moeo)
    {
        return this->get_override("operator()")(_moeo);
    }
    bool operator()(const eoPop<SolutionType>& _pop)
    {
        return this->get_override("operator()")(_pop);
    }
};

template <typename SolutionType>
void expose_archive(std::string stype)
{
    // ---------------------------moeoArchive (ABC)---------------------------

    //two update...disambiguate (wrapped one, update is protected member)
    bool (moeoArchiveWrap<SolutionType>::*f1)(const SolutionType&) = &moeoArchiveWrap<SolutionType>::update;
    bool (moeoArchiveWrap<SolutionType>::*f2)(const eoPop<SolutionType>&) = &moeoArchiveWrap<SolutionType>::update;

    class_<moeoArchiveWrap<SolutionType>,boost::noncopyable>(make_name("moeoArchive",stype).c_str())
        .def("__call__",pure_virtual<bool(moeoArchive<SolutionType>::*)(const SolutionType&)>(&moeoArchive<SolutionType>::operator()))
        .def("__call__",pure_virtual<bool(moeoArchive<SolutionType>::*)(const eoPop<SolutionType>&)>(&moeoArchive<SolutionType>::operator()))
        .def("contains",&moeoArchive<SolutionType>::contains)
        .def("dominates",&moeoArchive<SolutionType>::dominates)
        .def("equals",&moeoArchive<SolutionType>::equals)
        .def("update",f1)
        .def("update",f2)
        ;


    // ---------------------------moeo2DMinHypervolumeArchive---------------------------
    bool(moeo2DMinHypervolumeArchive<SolutionType>::*call_ind)(const SolutionType&) = &moeo2DMinHypervolumeArchive<SolutionType>::operator();

    bool(moeo2DMinHypervolumeArchive<SolutionType>::*call_pop)(const eoPop<SolutionType>&) = &moeo2DMinHypervolumeArchive<SolutionType>::operator();

    bool(moeo2DMinHypervolumeArchive<SolutionType>::*ins)(const typename moeo2DMinHypervolumeArchive<SolutionType>::ObjectiveVector &) = &moeo2DMinHypervolumeArchive<SolutionType>::insert;

    class_<moeo2DMinHypervolumeArchive<SolutionType>>
        (make_name("moeo2DMinHypervolumeArchive",stype).c_str(), init<unsigned int, double>())
        .def("__call__",call_ind)
        .def("__call__",call_pop)
        .def("insert",ins)
        .def("__str__",&moeo2DMinHypervolumeArchive<SolutionType>::print)
        ;


    // --------------------------moeoUnboundedArchive--------------------------
    bool(moeoUnboundedArchive<SolutionType>::*unb_call_1)(const SolutionType&) = &moeoUnboundedArchive<SolutionType>::operator();

    bool(moeoUnboundedArchive<SolutionType>::*unb_call_2)(const eoPop<SolutionType>&) = &moeoUnboundedArchive<SolutionType>::operator();

    class_<moeoUnboundedArchive<SolutionType>,bases<moeoArchive<SolutionType>>>
            ("moeoUnboundedArchive", init<bool>())
        .def("__call__",unb_call_1)
        .def("__call__",unb_call_2)
        ;

        // bool insert(const ObjectiveVector & _objVec)

        //huh???
        // template < class MOEOT >
        // class moeo2DMinHypervolumeArchive : public std::set<MOEOT , comp < MOEOT > >

        //also abstract...
        // class_<moeoBoundedArchive<PyMOEO>,bases<moeoArchive<PyMOEO>>>
        // ("moeoBoundedArchive",init<bool>())
        // ;



        // class_<moeoEpsilonHyperboxArchive<PyMOEO>,bases<moeoArchive<PyMOEO>>>
        // ("moeoEpsilonHyperboxArchive",init<
        //     moeoObjectiveVectorComparator<PyMOEO::ObjectiveVector>&,
        //     std::vector<double>,
        //     bool
        //     >()
        // )
        // .def(init<std::vector<double>,bool>())
        // .def<bool(moeoEpsilonHyperboxArchive<PyMOEO>::*)(const PyMOEO&)>("__call__",&moeoEpsilonHyperboxArchive<PyMOEO>::operator())
        // .def<bool(moeoEpsilonHyperboxArchive<PyMOEO>::*)(const eoPop<PyMOEO>&)>("__call__",&moeoEpsilonHyperboxArchive<PyMOEO>::operator())
        // .def("getNadir",&moeoEpsilonHyperboxArchive<PyMOEO>::getNadir)
        // .def("getIdeal",&moeoEpsilonHyperboxArchive<PyMOEO>::getIdeal)
        // .def("filtre",&moeoEpsilonHyperboxArchive<PyMOEO>::filtre)
        // ;
}

void moeo_archive()
{
    expose_archive<PyEOT>("");
    expose_archive<BinarySolution>("Bin");
    expose_archive<RealSolution>("Real");
}
