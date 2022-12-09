#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/eoIntBounds.h>
#include <utils/eoIntVectorBounds.h>

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

//---------------base class for integer 1-D bounds---------------
struct eoIntBoundsWrap : eoIntBounds, bp::wrapper<eoIntBounds> {
public:
    DEF_CONST_PV(bool,isBounded)
    DEF_CONST_PV(bool,hasNoBoundAtAll)
    DEF_CONST_PV(bool,isMinBounded)
    DEF_CONST_PV(bool,isMaxBounded)

    DEF_CONST_PV(long int,minimum)
    DEF_CONST_PV(long int,maximum)
    DEF_CONST_PV(long int,range)
    DEF_CONST_PV(eoIntBounds*,dup)

    DEF_CONST_PV2(bool,double,isInBounds)
    DEF_CONST_PV2(double,eoRng&,uniform)
    DEF_CONST_PV2(long int,eoRng&,random)

    DEF_CONST_PV3(int&,foldsInBounds)
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
        throw eoException("Should not use eoIntBounds::readFrom");
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

//eoIntNoBounds
//eoIntInterval
//eoIntBelowBound
//eoIntAboveBound
template <class IntBounds, class Init>
void add_int_bounds(std::string name, Init init)
{
    using namespace boost::python;

    class_<IntBounds, bases<eoIntBounds>>(name.c_str(), init) \
        .def("isBounded", &IntBounds::isBounded)
        .def("hasNoBoundAtAll", &IntBounds::hasNoBoundAtAll)
        .def("isMinBounded", &IntBounds::isMinBounded)
        .def("isMaxBounded", &IntBounds::isMaxBounded)
        .def("isInBounds", &IntBounds::isInBounds)
        .def("foldsInBounds",static_cast<void (IntBounds::*)(int&) const>(&IntBounds::foldsInBounds))
        .def("foldsInBounds",foldsInBoundsWrap<eoIntBounds,int>)
        .def("truncate",static_cast<void (IntBounds::*)(double&) const>(&IntBounds::truncate))
        .def("truncate", truncateWrap<eoIntBounds,long int>)
        .def("minimum", &IntBounds::minimum)
        .def("maximum", &IntBounds::maximum)
        .def("range", &IntBounds::range)
        .def("uniform", &IntBounds::uniform)
        .def("random", &IntBounds::random)
        .def("__str__", to_string<IntBounds>)
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

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(int_uniform1_overload, uniform, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(int_uniform2_overload, uniformWrap<eoIntVectorBounds>, 2, 3)

void
int_bounds()
{
    using namespace boost::python;

    //-------------------IntBounds---------------------
    //Abstract Base Class for 1D bounds
    class_<eoIntBoundsWrap, boost::noncopyable>("IntBounds",
      init<>())
    .def("isBounded", pure_virtual(&eoIntBoundsWrap::isBounded))
    .def("hasNoBoundAtAll", pure_virtual(&eoIntBoundsWrap::hasNoBoundAtAll))
    .def("isMinBounded", pure_virtual(&eoIntBoundsWrap::isMinBounded))
    .def("isMaxBounded", pure_virtual(&eoIntBoundsWrap::isMaxBounded))
    .def("minimum", pure_virtual(&eoIntBoundsWrap::minimum))
    .def("maximum", pure_virtual(&eoIntBoundsWrap::maximum))
    .def("range", pure_virtual(&eoIntBoundsWrap::range))
    .def("isInBounds", pure_virtual(&eoIntBoundsWrap::isInBounds))
    .def("uniform", pure_virtual(&eoIntBoundsWrap::uniform))
    .def("random", pure_virtual(&eoIntBoundsWrap::random))
    .def("foldsInBounds", pure_virtual(&eoIntBoundsWrap::foldsInBounds))
    .def("truncate", pure_virtual(&eoIntBoundsWrap::truncate))
    ;

    register_ptr_to_python< boost::shared_ptr<eoIntBounds> >();

    add_int_bounds<eoIntNoBounds>("IntNoBounds",init<>());
    add_int_bounds<eoIntInterval>("IntInterval",init<long int,long int>());
    add_int_bounds<eoIntBelowBound>("IntBelowBound",init<long int>());
    add_int_bounds<eoIntAboveBound>("IntAboveBound",init<long int>());


    // class_<std::vector<eoIntBounds*> >("_BoundsVectorInt")
    // .def(vector_indexing_suite<std::vector<eoIntBounds*> >())
    // ;

    // class_<std::vector<boost::shared_ptr<eoIntInterval>> >("_BdsVectorIntInterval")
    // .def(vector_indexing_suite<std::vector<boost::shared_ptr<eoIntInterval>>,true>())
    // ;

    class_<std::vector<eoIntBoundsPtr>>("_BdsVectorInt")
    .def(vector_indexing_suite<std::vector<eoIntBoundsPtr>,true>())
    ;


    bool (eoIntVectorBounds::* isBoundedInt1) (unsigned) = &eoIntVectorBounds::isBounded;
    bool (eoIntVectorBounds::* isBoundedInt2) (void)     = &eoIntVectorBounds::isBounded;
    bool (eoIntVectorBounds::* hasNoBoundAtAllInt1) (unsigned) = &eoIntVectorBounds::hasNoBoundAtAll;
    bool (eoIntVectorBounds::* hasNoBoundAtAllInt2) (void) = &eoIntVectorBounds::hasNoBoundAtAll;
    bool (eoIntVectorBounds::* isInBoundsInt1) (unsigned, long int) = &eoIntVectorBounds::isInBounds;
    bool (eoIntVectorBounds::* isInBoundsInt2) (const std::vector<long int>) = &eoIntVectorBounds::isInBounds;


    class_<eoIntVectorBounds, bases<std::vector<eoIntBoundsPtr> > >("IntVectorBounds", init<>())
    .def(init<
          unsigned,
          eoIntBoundsPtr
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def("isBounded", isBoundedInt1)
    .def("isBounded", isBoundedInt2)
    .def("hasNoBoundAtAll", hasNoBoundAtAllInt1)
    .def("hasNoBoundAtAll", hasNoBoundAtAllInt2)
    .def("isMinBounded",&eoIntVectorBounds::isMinBounded)
    .def("isMaxBounded",&eoIntVectorBounds::isMaxBounded)
    .def("foldsInBounds",vec_foldsInBoundsWrap<eoIntVectorBounds,long int>)
    .def("foldsInBounds",vec_foldsInBoundsWrap2<eoIntVectorBounds,long int>)
    .def("truncate",vec_truncateWrap<eoIntVectorBounds,long int>)
    .def("truncate",vec_truncateWrap2<eoIntVectorBounds,long int>)
    .def("isInBounds",isInBoundsInt1)
    .def("isInBounds",isInBoundsInt2)
    .def("minimum",&eoIntVectorBounds::minimum)
    .def("maximum",&eoIntVectorBounds::maximum)
    .def("range",&eoIntVectorBounds::range)
    .def("averageRange",&eoIntVectorBounds::averageRange)
    .def("uniform", static_cast<double (eoIntVectorBounds::*)(unsigned,eoRng&)>(&eoIntVectorBounds::uniform),int_uniform1_overload(args("i","_rng"))
    )
    .def("uniform", uniformWrap<eoIntVectorBounds>, int_uniform2_overload())
    ;
} // bounds
