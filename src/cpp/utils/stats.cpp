#include <utils/def_abstract_functor.h>

#include <utils/eoStat.h>
#include <eoPop.h>

#include <pyeot.h>

using namespace boost::python;

template<typename SolutionType>
struct eoStatBaseWrapper :  eoStatBase<SolutionType>,wrapper<eoStatBase<SolutionType>>
{
public:
    eoStatBaseWrapper() : eoStatBase<SolutionType>(){}

    void operator()(const eoPop<SolutionType>& _pop)
    {
        (void)this->get_override("operator()")(_pop);
    }

    void lastCall(const eoPop<SolutionType>& _pop)
    {
        if (override f = this->get_override("lastCall"))
        {
            lastCall(_pop);
            return;
        }
        eoStatBase<SolutionType>::lastCall(_pop);
    }
    void default_lastCall(const eoPop<SolutionType>& _pop) { this->eoStatBase<SolutionType>::lastCall(_pop); }
};

template<typename SolutionType>
void expose_eostats(std::string name)
{
    class_<eoStatBaseWrapper<SolutionType>,boost::noncopyable>(make_name("eoStatBase",name).c_str())
    .def("__call__", pure_virtual(&eoStatBaseWrapper<SolutionType>::operator()))
    .def("lastCall", &eoStatBase<SolutionType>::lastCall, &eoStatBaseWrapper<SolutionType>::default_lastCall)
    ;
}

void
eo_statistics()
{
    expose_eostats<PyEOT>("");
    expose_eostats<BinarySolution>("Bin");
    expose_eostats<RealSolution>("Real");
}
