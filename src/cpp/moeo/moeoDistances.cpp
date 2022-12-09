#include <pyeot.h>
#include <utils/def_abstract_functor.h>

#include <eoPop.h>

#include <distance/moeoDistance.h>
#include <distance/moeoObjSpaceDistance.h>
// #include <distance/moeoNormalizedDistance.h>


using namespace boost::python;

template <typename SolutionType,typename DistType>
struct moeoDistanceWrap : moeoDistance<SolutionType,DistType>,wrapper<moeoDistance<SolutionType,DistType>>
{
    //pure virtual inherited from eoBF ....
    const DistType operator()(const SolutionType& _moeo1, const SolutionType& _moeo2)
    {
        return this->get_override("operator()")(_moeo1,_moeo2); //(_pop);
    }

    //virtual w/ default implementation
    void setup(const eoPop<SolutionType>& _pop)
    {
        if (override f = this->get_override("setup"))
        {
            setup(_pop);
            return;
        }
        moeoDistance<SolutionType,DistType>::setup(_pop);
    }
    void default_setup1(const eoPop<SolutionType>& _pop) { this->moeoDistance<SolutionType,DistType>::setup(_pop); }

    //virtual w/ default implementation
    void setup(double _min,double _max,unsigned int _obj)
    {
        if (override f = this->get_override("setup"))
        {
            setup(_min,_max,_obj);
            return;
        }
        moeoDistance<SolutionType,DistType>::setup(_min,_max,_obj);
    }
    void default_setup2(double _min,double _max,unsigned int _obj) { this->moeoDistance<SolutionType,DistType>::setup(_min,_max,_obj); }
};

template <typename SolutionType>
struct moeoObjSpaceDistanceWrap : moeoObjSpaceDistance<SolutionType>,wrapper<moeoObjSpaceDistance<SolutionType>>
{
    typedef typename SolutionType::ObjectiveVector ObjectiveVector;
    typedef typename SolutionType::Fitness Fitness;

    //pure virtual
    const Fitness operator()(const ObjectiveVector& _o1,const ObjectiveVector& _o2)
    {
        return this->get_override("operator()")(_o1,_o2);
    }

    //with default
    const Fitness operator()(const SolutionType& _o1,const SolutionType& _o2)
    {
        if (override f = this->get_override("operator()"))
        {
            return operator()(_o1,_o2);
        }
        return moeoObjSpaceDistance<SolutionType>::operator()(_o1,_o2);
    }
    void default_operator(const SolutionType& _o1,const SolutionType& _o2) {
        this->moeoObjSpaceDistance<SolutionType>::operator()(_o1,_o2);
    }
};

  //   /**
  //    * Nothing to do
  //    * @param _realInterval the eoRealInterval object
  //    * @param _obj the objective index
  //    */
  //   virtual void setup(eoRealInterval _realInterval, unsigned int _obj)
  //   {}
  //
  // };


template <typename SolutionType,typename DistType>
void expose_distances(std::string stype)
{
    //two setup...disambiguate
    void (moeoDistance<SolutionType,DistType>::*f1)(const eoPop<SolutionType>&) = &moeoDistance<SolutionType,DistType>::setup;
    void (moeoDistance<SolutionType,DistType>::*f2)(double,double,unsigned int) = &moeoDistance<SolutionType,DistType>::setup;

    class_<moeoDistanceWrap<SolutionType,DistType>, boost::noncopyable>(make_name("moeoDistance",stype).c_str())
    .def("setup", f1, &moeoDistanceWrap<SolutionType,DistType>::default_setup1)
    .def("setup", f2, &moeoDistanceWrap<SolutionType,DistType>::default_setup2)
    .def("__call__", pure_virtual(&moeoDistance<SolutionType,DistType>::operator()))
    ;


    //two operator()...disambiguate
    const typename SolutionType::Fitness (moeoObjSpaceDistance<SolutionType>::*fx1)(const typename SolutionType::ObjectiveVector&,const typename SolutionType::ObjectiveVector&) = &moeoObjSpaceDistance<SolutionType>::operator();
    const typename SolutionType::Fitness (moeoObjSpaceDistance<SolutionType>::*fx2)(const SolutionType&, const SolutionType&) = &moeoObjSpaceDistance<SolutionType>::operator();

    class_<moeoObjSpaceDistanceWrap<SolutionType>, boost::noncopyable>(make_name("moeoObjSpaceDistance",stype).c_str())
    .def("__call__", pure_virtual(fx1))
    .def("__call__", fx2)
    ;
};


void moeo_distances()
{
    expose_distances<PyEOT,double>("");
    expose_distances<BinarySolution,double>("Bin");
    expose_distances<RealSolution,double>("Real");
}
