#include <boost/python.hpp>

#include <iostream>

#include <eoEvalFunc.h>
#include <pyeot.h>


template<class EOT>
class Sphere : public eoEvalFunc<EOT>
{
public:
    void operator() (EOT& _sol) {
        double sum = 0.0f;
        for (unsigned int i = 0; i < _sol.size(); i++)
            sum += _sol[i] * _sol[i];
        _sol.fitness(sum);
    }
};


BOOST_PYTHON_MODULE(_mod)
{
    using namespace boost::python;

    class_<Sphere<RealSolution>, bases<eoEvalFunc<RealSolution>>>
    ("Sphere",init<>())
    .def("__call__", &Sphere<RealSolution>::operator ())
    ;

}
