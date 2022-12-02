#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <fitness/moeoFitnessAssignment.h>
#include <fitness/moeoDominanceBasedFitnessAssignment.h>
#include <fitness/moeoIndicatorBasedFitnessAssignment.h>
#include <fitness/moeoCriterionBasedFitnessAssignment.h>
#include <fitness/moeoScalarFitnessAssignment.h>
#include <fitness/moeoSingleObjectivization.h>

#include <fitness/moeoDominanceDepthFitnessAssignment.h>
#include <fitness/moeoDominanceRankFitnessAssignment.h>
#include <fitness/moeoDominanceCountFitnessAssignment.h>
#include <fitness/moeoDominanceCountRankingFitnessAssignment.h>


using namespace boost::python;


//<fitness/moeoFitnessAssignment.h>
template<typename SolutionType>
struct moeoFitnessAssignmentWrap : moeoFitnessAssignment<SolutionType>,wrapper<moeoFitnessAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    //pure virtual
    void updateByDeleting(eoPop < SolutionType > & _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop); //(_pop);
    }
};

// <fitness/moeoDominanceBasedFitnessAssignment.h>
template<typename SolutionType>
struct moeoDominanceBasedFitnessAssignmentWrap : moeoDominanceBasedFitnessAssignment<SolutionType>,wrapper<moeoDominanceBasedFitnessAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop<SolutionType>& _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop); //(_pop);
    }
}
;

// <fitness/moeoIndicatorBasedFitnessAssignment.h>
template<typename SolutionType>
struct moeoIndicatorBasedFitnessAssignmentWrap : moeoIndicatorBasedFitnessAssignment<SolutionType>,wrapper<moeoIndicatorBasedFitnessAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop<SolutionType> & _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop);
    }
};

// <fitness/moeoCriterionBasedFitnessAssignment.h>
template<typename SolutionType>
struct moeoCriterionBasedFitnessAssignmentWrap : moeoCriterionBasedFitnessAssignment<SolutionType>,wrapper<moeoCriterionBasedFitnessAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop < SolutionType > & _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop);
    }
};

//
template<typename SolutionType>
struct moeoScalarFitnessAssignmentWrap : moeoScalarFitnessAssignment<SolutionType>,wrapper<moeoScalarFitnessAssignment<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop < SolutionType > & _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop);
    }
};

template<typename SolutionType>
struct moeoSingleObjectivizationWrap : moeoSingleObjectivization<SolutionType>,wrapper<moeoSingleObjectivization<SolutionType>>
{
public:
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;

    void updateByDeleting(eoPop < SolutionType > & _pop, const ObjectiveVector& _objVec)
    {
        this->get_override("updateByDeleting")(_pop,_objVec);
    }
    //pure virtual inherited from eoUF<eoPop<MOEOT>&, void> ....
    void operator()(eoPop<SolutionType>& _pop)
    {
        this->get_override("operator()")(_pop); //(_pop);
    }
    void operator()(SolutionType& _moeo)
    {
        this->get_override("operator()")(_moeo);
    }
    typename SolutionType::Fitness operator()(const ObjectiveVector & _obj)
    {
        return this->get_override("operator()")(_obj); //(_pop);
    }
};


template <typename SolutionType>
void expose_fitnessAssign(std::string stype)
{
    //---------------------ABSTRACT---------------------
    void (moeoFitnessAssignment<SolutionType>::*fx1)(eoPop<SolutionType>&, SolutionType&) = &moeoFitnessAssignment<SolutionType>::updateByDeleting;

    class_<moeoFitnessAssignmentWrap<SolutionType>,boost::noncopyable>(make_name("moeoFitnessAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoFitnessAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>(&moeoFitnessAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoFitnessAssignment<SolutionType>::operator()))
        .def("updateByDeleting", fx1)
    ;

    /** Dominance-based Fitness assignments
    */
    class_<moeoDominanceBasedFitnessAssignmentWrap<SolutionType>,bases<moeoFitnessAssignment<SolutionType>>,boost::noncopyable>(make_name("moeoDominanceBasedFitnessAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoDominanceBasedFitnessAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoDominanceBasedFitnessAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoDominanceBasedFitnessAssignment<SolutionType>::operator()))
    ;

    /** Indicator-based
    */
    class_<moeoIndicatorBasedFitnessAssignmentWrap<SolutionType>,bases<moeoFitnessAssignment<SolutionType>>,boost::noncopyable>(make_name("moeoIndicatorBasedFitnessAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoIndicatorBasedFitnessAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoIndicatorBasedFitnessAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoIndicatorBasedFitnessAssignmentWrap<SolutionType>::operator()))
    ;

    /** Criterion-based
    */
    class_<moeoCriterionBasedFitnessAssignmentWrap<SolutionType>,boost::noncopyable>(make_name("moeoCriterionBasedFitnessAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoCriterionBasedFitnessAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoCriterionBasedFitnessAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoCriterionBasedFitnessAssignmentWrap<SolutionType>::operator()))
    ;

    /** Scalar
    */
    class_<moeoScalarFitnessAssignmentWrap<SolutionType>,boost::noncopyable>(make_name("moeoScalarFitnessAssignment",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoScalarFitnessAssignment<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoScalarFitnessAssignment<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual(&moeoScalarFitnessAssignmentWrap<SolutionType>::operator()))
    ;

    /** Single-objectivization
    */
    class_<moeoSingleObjectivizationWrap<SolutionType>,bases<moeoFitnessAssignment<SolutionType>,eoEvalFunc<SolutionType>>,boost::noncopyable>
    (make_name("moeoSingleObjectivization",stype).c_str())
        .def("updateByDeleting", pure_virtual<void(moeoSingleObjectivization<SolutionType>::*)(eoPop<SolutionType>&,const typename SolutionType::ObjectiveVector&)>
            (&moeoSingleObjectivization<SolutionType>::updateByDeleting) )
        .def("__call__", pure_virtual< void (moeoSingleObjectivization<SolutionType>::*)(eoPop<SolutionType>&) >
            (&moeoSingleObjectivization<SolutionType>::operator()))
        .def("__call__", pure_virtual< void (moeoSingleObjectivizationWrap<SolutionType>::*)(SolutionType&)>
            (&moeoSingleObjectivizationWrap<SolutionType>::operator()))
        .def("__call__", pure_virtual< typename SolutionType::Fitness (moeoSingleObjectivizationWrap<SolutionType>::*)(const typename SolutionType::ObjectiveVector&)>
            (&moeoSingleObjectivizationWrap<SolutionType>::operator()))
    ;

    //------------------------CONCRETE------------------------
    class_<moeoDominanceDepthFitnessAssignment<SolutionType>, bases<moeoDominanceBasedFitnessAssignment<SolutionType> > >
    (make_name("moeoDominanceDepthFitnessAssignment",stype).c_str(),init<bool>())
    //missing another Ctor (need comparator)
    .def("__call__",&moeoDominanceDepthFitnessAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoDominanceDepthFitnessAssignment<SolutionType>::updateByDeleting)
    ;

    class_<moeoDominanceRankFitnessAssignment<SolutionType>, bases<moeoDominanceBasedFitnessAssignment<SolutionType> > >
    (make_name("moeoDominanceRankFitnessAssignment",stype).c_str(),init<double,bool>())
    .def("__call__",&moeoDominanceRankFitnessAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoDominanceRankFitnessAssignment<SolutionType>::updateByDeleting)
    ;

    class_<moeoDominanceCountFitnessAssignment<SolutionType>, bases<moeoDominanceBasedFitnessAssignment<SolutionType> > >
    (make_name("moeoDominanceCountFitnessAssignment",stype).c_str(),init<bool>())
    .def("__call__",&moeoDominanceCountFitnessAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoDominanceCountFitnessAssignment<SolutionType>::updateByDeleting)
    ;

    class_<moeoDominanceCountRankingFitnessAssignment<SolutionType>, bases<moeoDominanceBasedFitnessAssignment<SolutionType> > >
    (make_name("moeoDominanceCountRankingFitnessAssignment",stype).c_str(),init<bool>())
    .def("__call__",&moeoDominanceCountRankingFitnessAssignment<SolutionType>::operator())
    .def("updateByDeleting",&moeoDominanceCountRankingFitnessAssignment<SolutionType>::updateByDeleting)
    ;
}

void moeo_fitnessAssign()
{
    expose_fitnessAssign<PyEOT>("");
    expose_fitnessAssign<BinarySolution>("Bin");
    expose_fitnessAssign<RealSolution>("Real");
}
