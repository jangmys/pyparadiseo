#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/eoRealBounds.h>
#include <utils/eoRealVectorBounds.h>

namespace bp = boost::python;
namespace np = boost::python::numpy;

template <class T>
boost::python::str to_string(T& _p)
{
    std::ostringstream os;
    _p.printOn(os);
    return boost::python::str(os.str().c_str());
}

//wrap pure virtual method of signature "ret x() const"
#define DEF_CONST_PV(ret,x) ret x() const \
    { \
        return this->get_override(#x)();\
    }

//wrap pure virtual method of signature "ret x(param v) const"
#define DEF_CONST_PV2(ret,param,x) ret x(param _v) const \
    { \
        return this->get_override(#x)(_v);\
    }

//wrap pure virtual method of signature "void x(param v) const"
#define DEF_CONST_PV3(param,x) void x(param _v) const \
    { \
        this->get_override(#x)(_v);\
    }

//---------------base class for real 1-D bounds---------------
struct eoRealBoundsWrap : eoRealBounds, bp::wrapper<eoRealBounds> {
public:
    DEF_CONST_PV(bool,isBounded)
    DEF_CONST_PV(bool,hasNoBoundAtAll)
    DEF_CONST_PV(bool,isMinBounded)
    DEF_CONST_PV(bool,isMaxBounded)

    DEF_CONST_PV(double,minimum)
    DEF_CONST_PV(double,maximum)
    DEF_CONST_PV(double,range)
    DEF_CONST_PV(eoRealBounds*,dup)

    DEF_CONST_PV2(bool,double,isInBounds)
    DEF_CONST_PV2(double,eoRng&,uniform)

    DEF_CONST_PV3(double&,foldsInBounds)
    DEF_CONST_PV3(double&,truncate)

    virtual void
    printOn(std::ostream& _os) const
    {
        _os << "[-inf,+inf]";
    }

    virtual void
    readFrom(std::istream& _is)
    {
        (void) _is;
        throw eoException("Should not use eoRealBounds::readFrom");
    }
};

//wrapper for foldsInBounds
//needed because of immutable scalar arguments in python : c++ modifies T& val
template<class X,typename T>
T foldsInBoundsWrap(X& bounds, T val) {
    bounds.foldsInBounds(val);
    return val;
};

//...the same
template<class X,typename T>
T vec_foldsInBoundsWrap(X& vec_bounds, unsigned i, T val) {
    vec_bounds[i]->foldsInBounds(val);
    return val;
};

//converter works only for rvalues
//copy code ... easier than converting ndarray to std::vector and back
template<class X,typename T>
void vec_foldsInBoundsWrap2(X& vec_bounds, np::ndarray vec) {
    T* arr = reinterpret_cast<T*>(vec.get_data());

    for(unsigned i =0;i<vec_bounds.size();i++){
        vec_bounds[i]->foldsInBounds(arr[i]);
    }
};

//...same thing for truncate methods
template<class X,typename T>
T truncateWrap(X& bounds, T val) {
    bounds.truncate(val);
    return val;
}

template<class X,typename T>
T vec_truncateWrap(X& vec_bounds, unsigned i, T val) {
    vec_bounds[i]->truncate(val);
    return val;
};

template<class X,typename T>
void vec_truncateWrap2(X& vec_bounds, np::ndarray vec) {
    T* arr = reinterpret_cast<T*>(vec.get_data());

    for(unsigned i =0;i<vec_bounds.size();i++){
        vec_bounds[i]->truncate(arr[i]);
    }
};



//eoRealNoBounds
//eoRealInterval
//eoRealBelowBound
//eoRealAboveBound
template <class RealBounds, class Init>
void add_real_bounds(std::string name, Init init)
{
    using namespace boost::python;

    class_<RealBounds, bases<eoRealBounds>>(name.c_str(), init) \
        .def("isBounded", &RealBounds::isBounded)
        .def("hasNoBoundAtAll", &RealBounds::hasNoBoundAtAll)
        .def("isMinBounded", &RealBounds::isMinBounded)
        .def("isMaxBounded", &RealBounds::isMaxBounded)
        .def("isInBounds", &RealBounds::isInBounds)
        .def("foldsInBounds",foldsInBoundsWrap<eoRealBounds,double>)
        .def("truncate", truncateWrap<eoRealBounds,double>)
        .def("minimum", &RealBounds::minimum)
        .def("maximum", &RealBounds::maximum)
        .def("range", &RealBounds::range)
        .def("uniform", &RealBounds::uniform)
        .def("__str__", to_string<RealBounds>)
        ;
}

//-----------------for vector bounds-----------------
template<class X>
void uniformWrap(X& _b, np::ndarray _v, eoRng & _rng = eo::rng)
{
    double* vec = reinterpret_cast<double*>(_v.get_data());

    for (unsigned i=0; i<_b.size(); i++)
    {
        vec[i] = _b.uniform(i, _rng);
    }
}

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(uniform1_overload, uniform, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(uniform2_overload, uniformWrap<eoRealVectorBounds>, 2, 3)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(int_uniform1_overload, uniform, 1, 2)


void
bounds()
{
    using namespace boost::python;

    //-------------------RealBounds---------------------
    //Abstract Base Class for 1D bounds
    class_<eoRealBoundsWrap, boost::noncopyable>("RealBounds",
      init<>())
    .def("isBounded", pure_virtual(&eoRealBoundsWrap::isBounded))
    .def("hasNoBoundAtAll", pure_virtual(&eoRealBoundsWrap::hasNoBoundAtAll))
    .def("isMinBounded", pure_virtual(&eoRealBoundsWrap::isMinBounded))
    .def("isMaxBounded", pure_virtual(&eoRealBoundsWrap::isMaxBounded))
    .def("minimum", pure_virtual(&eoRealBoundsWrap::minimum))
    .def("maximum", pure_virtual(&eoRealBoundsWrap::maximum))
    .def("range", pure_virtual(&eoRealBoundsWrap::range))
    .def("isInBounds", pure_virtual(&eoRealBoundsWrap::isInBounds))
    .def("uniform", pure_virtual(&eoRealBoundsWrap::uniform))
    .def("foldsInBounds", pure_virtual(&eoRealBoundsWrap::foldsInBounds))
    .def("truncate", pure_virtual(&eoRealBoundsWrap::truncate))
    ;


    //----------------derived from RealBounds----------------

    add_real_bounds<eoRealNoBounds>("RealNoBounds",init<>());
    add_real_bounds<eoRealInterval>("RealInterval",init<double,double>());
    add_real_bounds<eoRealBelowBound>("RealBelowBound",init<double>());
    add_real_bounds<eoRealAboveBound>("RealAboveBound",init<double>());


    register_ptr_to_python< std::shared_ptr<eoRealBounds> >();

    //and with std
    // By default indexed elements have Python reference semantics and are returned by proxy. This can be disabled by supplying true in the NoProxy template parameter.
    // https://www.boost.org/doc/libs/1_80_0/libs/python/doc/html/reference/topics/indexing_support.html#topics.indexing_support.class_vector_indexing_suite
    class_<std::vector<eoRealBoundsPtr>>("_BdsVectorReal")
    .def(vector_indexing_suite<std::vector<eoRealBoundsPtr>,true>())
    ;

    //-------------------eoRealVectorBounds-------------------
    bool (eoRealVectorBounds::* isBounded_1) (unsigned) = &eoRealVectorBounds::isBounded;
    bool (eoRealVectorBounds::* isBounded_2) (void)     = &eoRealVectorBounds::isBounded;
    bool (eoRealVectorBounds::* hasNoBoundAtAll_1) (unsigned) = &eoRealVectorBounds::hasNoBoundAtAll;
    bool (eoRealVectorBounds::* hasNoBoundAtAll_2) (void) = &eoRealVectorBounds::hasNoBoundAtAll;

    class_<eoRealVectorBounds, bases<std::vector<eoRealBoundsPtr> > >("RealVectorBounds", init<>())
    .def(init<
          unsigned,
          eoRealBoundsPtr
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def(init<unsigned,double,double>())
    .def("isBounded", isBounded_1)
    .def("isBounded", isBounded_2)
    .def("hasNoBoundAtAll", hasNoBoundAtAll_1)
    .def("hasNoBoundAtAll", hasNoBoundAtAll_2)
    .def("isMinBounded",&eoRealVectorBounds::isMinBounded)
    .def("isMaxBounded",&eoRealVectorBounds::isMaxBounded)
    .def("foldsInBounds",vec_foldsInBoundsWrap<eoRealVectorBounds,double>)
    .def("foldsInBounds",vec_foldsInBoundsWrap2<eoRealVectorBounds,double>)
    .def("truncate",vec_truncateWrap<eoRealVectorBounds,double>)
    .def("truncate",vec_truncateWrap2<eoRealVectorBounds,double>)
    .def("minimum",&eoRealVectorBounds::minimum)
    .def("maximum",&eoRealVectorBounds::maximum)
    .def("range",&eoRealVectorBounds::range)
    .def("uniform", static_cast<double (eoRealVectorBounds::*)(unsigned,eoRng&)>(&eoRealVectorBounds::uniform),uniform1_overload(args("i","_rng"))
    )
    .def("uniform", uniformWrap<eoRealVectorBounds>, uniform2_overload())
    ;
} // bounds
