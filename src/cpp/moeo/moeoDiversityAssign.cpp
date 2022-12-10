#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <diversity/moeoDiversityAssignment.h>
#include <diversity/moeoCrowdingDiversityAssignment.h>
#include <diversity/moeoFrontByFrontCrowdingDiversityAssignment.h>

#include <diversity/moeoSharingDiversityAssignment.h>
#include <diversity/moeoFrontByFrontSharingDiversityAssignment.h>

#include <diversity/moeoNearestNeighborDiversityAssignment.h>


using namespace boost::python;

template<typename SolutionType>
struct moeoDiversityAssignmentWrap : moeoDiversityAssignment<SolutionType>,wrapper<moeoDiversityAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop < SolutionType > & _pop, const ObjectiveVector & _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);//(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop);
    }
};


template <typename SolutionType>
void expose_diversity_assign(std::string stype)
{
    void (moeoDiversityAssignment<SolutionType>::*fx1)(eoPop<SolutionType>&, SolutionType&) = &moeoDiversityAssignment<SolutionType>::updateByDeleting;

    class_<moeoDiversityAssignmentWrap<SolutionType>,boost::noncopyable>(make_name("moeoDiversityAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoDiversityAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoDiversityAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoDiversityAssignmentWrap<SolutionType>::operator()))
        .def("updateByDeleting", fx1)
    ;



    class_<moeoCrowdingDiversityAssignment<SolutionType>, bases<moeoDiversityAssignment<SolutionType> > >
    (make_name("moeoCrowdingDiversityAssignment",stype).c_str(),init<>())
    .def("__call__",&moeoCrowdingDiversityAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoCrowdingDiversityAssignment<SolutionType>::updateByDeleting)
    .def("inf",&moeoCrowdingDiversityAssignment<SolutionType>::inf)
    .def("tiny",&moeoCrowdingDiversityAssignment<SolutionType>::tiny)
    ;

    class_<moeoFrontByFrontCrowdingDiversityAssignment<SolutionType>,bases<moeoCrowdingDiversityAssignment<SolutionType>>>
    (make_name("moeoFrontByFrontCrowdingDiversityAssignment",stype).c_str(),init<>())
    .def("updateByDeleting",&moeoFrontByFrontCrowdingDiversityAssignment<SolutionType>::updateByDeleting)
    .def("__call__",&moeoFrontByFrontCrowdingDiversityAssignment<SolutionType>::operator())
    ;

    class_<moeoSharingDiversityAssignment<SolutionType>, bases<moeoDiversityAssignment<SolutionType> > >
    ("moeoSharingDiversityAssignment",init<
        moeoDistance<SolutionType,typename SolutionType::Fitness>&,
        double,
        double
    >()
    [WC1]
    )
    .def(init<double,double>())
    .def("__call__",&moeoSharingDiversityAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoSharingDiversityAssignment<SolutionType>::updateByDeleting)
    ;

    class_<moeoFrontByFrontSharingDiversityAssignment<SolutionType>,bases<moeoSharingDiversityAssignment<SolutionType>>>
    ("moeoFrontByFrontSharingDiversityAssignment",init<
        moeoDistance<SolutionType,typename SolutionType::Fitness>&,
        double,
        double
    >()[WC1]
    )
    .def(init<double,double>())
    .def("updateByDeleting",&moeoFrontByFrontSharingDiversityAssignment<SolutionType>::updateByDeleting)
    .def("__call__",&moeoFrontByFrontSharingDiversityAssignment<SolutionType>::operator())
    ;


}


void moeo_diversityAssign()
{
    expose_diversity_assign<PyEOT>("");
    expose_diversity_assign<BinarySolution>("Bin");
    expose_diversity_assign<RealSolution>("Real");
}
