#include <pyeot.h>
#include <pymo.h>
#include <utils/def_abstract_functor.h>

#include <continuator/moStatBase.h>
#include <continuator/moStat.h>
#include <continuator/moFitnessStat.h>

using namespace boost::python;

template<typename SolutionType>
struct moStatBaseWrap : moStatBase<SolutionType>,wrapper<moStatBase<SolutionType>>
{
public:
    void operator()(SolutionType& _ind)
    {
        this->get_override("operator()")(_ind);
    }
};

template<typename SolutionType,typename T>
struct moStatWrap : moStat<SolutionType,T>,wrapper<moStat<SolutionType,T>>
{
public:
    moStatWrap(T _value, std::string _description = "") : moStat<SolutionType,T>(_value,_description){};

    void operator()(SolutionType& _ind)
    {
        this->get_override("operator()")(_ind);
    }
};

template<typename SolutionType,typename T>
void expose_moStat(std::string basename,std::string name)
{
    class_<moStatWrap<SolutionType,T>,bases<moStatBase<SolutionType>>,boost::noncopyable>
    (make_name(basename.c_str(),name).c_str(),init<T,optional< std::string>>())
    .def("__call__", pure_virtual(&moStatWrap<SolutionType,T>::operator()))
    .def("lastCall", &moStatBase<SolutionType>::lastCall)
    .def("init", &moStatBase<SolutionType>::init)
    ;
}


template<typename SolutionType>
void expose_moStats(std::string name)
{
    class_<moStatBaseWrap<SolutionType>,boost::noncopyable>
    (make_name("moStatBase",name).c_str())
    .def("__call__", pure_virtual(&moStatBaseWrap<SolutionType>::operator()))
    .def("lastCall", &moStatBase<SolutionType>::lastCall)
    .def("init", &moStatBase<SolutionType>::init)
    ;

    register_ptr_to_python< std::shared_ptr<moStatBase<SolutionType>> >();

    expose_moStat<SolutionType,double>("moRealStat",name);
    expose_moStat<SolutionType,int>("moIntStat",name);
    expose_moStat<SolutionType,boost::python::object>("moPyStat",name);
    expose_moStat<SolutionType,typename SolutionType::Fitness>("moFitStat",name);

    class_<moFitnessStat<SolutionType>,bases<moStat<SolutionType,typename SolutionType::Fitness>>>(make_name("moFitnessStat",name).c_str())
    .def("__call__",&moFitnessStat<SolutionType>::operator())
    .def("init",&moFitnessStat<SolutionType>::init)
    ;
}


void moStats()
{
    expose_moStats<PyEOT>("");
    expose_moStats<BinarySolution>("Bin");
    expose_moStats<RealSolution>("Real");
    expose_moStats<IntSolution>("Perm");
}
