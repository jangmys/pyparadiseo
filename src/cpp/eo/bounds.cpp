#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/eoRealBounds.h>
#include <utils/eoIntBounds.h>

#include <utils/eoRealVectorBounds.h>
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
        .def("foldsInBounds",static_cast<void (IntBounds::*)(double&) const>(&IntBounds::foldsInBounds))
        .def("foldsInBounds",foldsInBoundsWrap<eoIntBounds,long int>)
        .def("truncate",static_cast<void (IntBounds::*)(double&) const>(&IntBounds::truncate))
        .def("truncate", truncateWrap<eoIntBounds,long int>)
        .def("minimum", &IntBounds::minimum)
        .def("maximum", &IntBounds::maximum)
        .def("range", &IntBounds::range)
        .def("uniform", &IntBounds::uniform)
        .def("random", &IntBounds::random)
        .def("__str__", to_string<IntBounds>)
        ;

    register_ptr_to_python< boost::shared_ptr<IntBounds> >();
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



//overload constructor for bounds...?
// static std::shared_ptr<eoRealVectorBounds> makeClass()

// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(unif_overload, uniform, 0, 1)
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(uniform1_overload, uniform, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(uniform2_overload, uniformWrap<eoRealVectorBounds>, 2, 3)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(int_uniform1_overload, uniform, 1, 2)
BOOST_PYTHON_FUNCTION_OVERLOADS(int_uniform2_overload, uniformWrap<eoBaseVectorBounds>, 2, 3)


// BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(uniform1_overload, static_cast<double (eoRealBaseVectorBounds::*)(unsigned,eoRng&)>(&eoRealBaseVectorBounds::uniform), 1, 2)


void
bounds()
{
    using namespace boost::python;

    class_<eoRealBoundsWrap, boost::noncopyable>("RealBounds",
      init<>())
    .def("isBounded", pure_virtual(&eoRealBoundsWrap::isBounded))
    .def("hasNoBoundAtAll", pure_virtual(&eoRealBoundsWrap::hasNoBoundAtAll))
    .def("isMinBounded", pure_virtual(&eoRealBoundsWrap::isMinBounded))
    .def("isMaxBounded", pure_virtual(&eoRealBoundsWrap::isMaxBounded))
    .def("isInBounds", pure_virtual(&eoRealBoundsWrap::isInBounds))
    .def("foldsInBounds", pure_virtual(&eoRealBoundsWrap::foldsInBounds))
    .def("truncate", pure_virtual(&eoRealBoundsWrap::truncate))
    .def("minimum", pure_virtual(&eoRealBoundsWrap::minimum))
    .def("maximum", pure_virtual(&eoRealBoundsWrap::maximum))
    .def("range", pure_virtual(&eoRealBoundsWrap::range))
    .def("uniform", pure_virtual(&eoRealBoundsWrap::uniform))
    ;

    register_ptr_to_python< boost::shared_ptr<eoRealBounds> >();

    ////////// derived from RealBounds
    //////////////////////////////////////////////////
    add_real_bounds<eoRealNoBounds>("RealNoBounds",init<>());
    add_real_bounds<eoRealInterval>("RealInterval",init<double,double>());
    add_real_bounds<eoRealBelowBound>("RealBelowBound",init<double>());
    add_real_bounds<eoRealAboveBound>("RealAboveBound",init<double>());


    // expose do derive eoRealBaseVectorBounds
    class_<std::vector<eoRealBounds*> >("_BoundsVectorReal")
    .def(vector_indexing_suite<std::vector<eoRealBounds*> >())
    ;

    class_<std::vector<boost::shared_ptr<eoRealInterval>> >("_BdsVectorRealInterval")
    .def(vector_indexing_suite<std::vector<boost::shared_ptr<eoRealInterval>>,true>())
    ;

    class_<std::vector<eoRealBoundsPtr>>("_BdsVectorReal")
    .def(vector_indexing_suite<std::vector<eoRealBoundsPtr>,true>())
    ;


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



    // bool (eoRealBaseVectorBounds::* isBounded1) (unsigned) = &eoRealBaseVectorBounds::isBounded;
    // bool (eoRealBaseVectorBounds::* isBounded2) (void)     = &eoRealBaseVectorBounds::isBounded;
    // bool (eoRealBaseVectorBounds::* hasNoBoundAtAll1) (unsigned) = &eoRealBaseVectorBounds::hasNoBoundAtAll;
    // bool (eoRealBaseVectorBounds::* hasNoBoundAtAll2) (void) = &eoRealBaseVectorBounds::hasNoBoundAtAll;
    // bool (eoRealBaseVectorBounds::* isInBounds1) (unsigned, double) = &eoRealBaseVectorBounds::isInBounds;
    // bool (eoRealBaseVectorBounds::* isInBounds2) (const std::vector<double>) = &eoRealBaseVectorBounds::isInBounds;

    // class_<eoRealBaseVectorBounds, bases<std::vector<eoRealBounds*> > >("RealVectorBoundsBase", init<>())
    // .def(init<
    //       unsigned,
    //       eoRealBounds&
    //   >()
    //   [
    //       with_custodian_and_ward<1, 3>()
    //   ]
    // )
    // .def("isBounded", isBounded1)
    // .def("isBounded", isBounded2)
    // .def("hasNoBoundAtAll", hasNoBoundAtAll1)
    // .def("hasNoBoundAtAll", hasNoBoundAtAll2)
    // .def("isMinBounded",&eoRealBaseVectorBounds::isMinBounded)
    // .def("isMaxBounded",&eoRealBaseVectorBounds::isMaxBounded)
    // .def("foldsInBounds",vec_foldsInBoundsWrap<eoRealBaseVectorBounds,double>)
    // .def("foldsInBounds",vec_foldsInBoundsWrap2<eoRealBaseVectorBounds,double>)
    // .def("truncate",vec_truncateWrap<eoRealBaseVectorBounds,double>)
    // .def("truncate",vec_truncateWrap2<eoRealBaseVectorBounds,double>)
    // .def("isInBounds",isInBounds1)
    // .def("isInBounds",isInBounds2)
    // .def("minimum",&eoRealBaseVectorBounds::minimum)
    // .def("maximum",&eoRealBaseVectorBounds::maximum)
    // .def("range",&eoRealBaseVectorBounds::range)
    // .def("averageRange",&eoRealBaseVectorBounds::averageRange)
    // .def("uniform", static_cast<double (eoRealBaseVectorBounds::*)(unsigned,eoRng&)>(&eoRealBaseVectorBounds::uniform),uniform1_overload(args("i","_rng"))
    // )
    // .def("uniform", uniformWrap<eoRealBaseVectorBounds>, uniform2_overload())
    // ;


    //ATTENTION : order of constructors matters!
    // https://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor
    // class_<eoRealVectorBounds, bases<eoRealBaseVectorBounds> >("RealVectorBounds", init<>())
    // .def(init<
    //       std::vector<double>,
    //       std::vector<double>
    //   >()
    // )
    // .def(init<
    //       unsigned,
    //       eoRealBounds&
    //   >()
    //   [
    //       with_custodian_and_ward<1, 3>()
    //   ]
    // )
    // // .def(init<
    // //       eoRealBounds&,
    // //       eoRealBounds&
    // //   >()
    // //   [
    // //       with_custodian_and_ward<1, 2,
    // //       with_custodian_and_ward<1, 3> >()
    // //   ]
    // // )
    // .def(init<
    //       unsigned,
    //       double,
    //       double
    //   >()
    // )
    // ;

    // class_<eoRealVectorNoBounds, bases<eoRealVectorBounds> >("RealVectorNoBounds", init<unsigned>())
    // ;


    //-------------------IntBounds---------------------
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
    // .def("foldsInBounds", &eoIntBounds::foldsInBounds)
    // .def("truncate", &eoIntBounds::truncate)
    .def("foldsInBounds", pure_virtual(&eoIntBoundsWrap::foldsInBounds))
    .def("truncate", pure_virtual(&eoIntBoundsWrap::truncate))
    ;

    add_int_bounds<eoIntNoBounds>("IntNoBounds",init<>());
    add_int_bounds<eoIntInterval>("IntInterval",init<long int,long int>());
    add_int_bounds<eoIntBelowBound>("IntBelowBound",init<long int>());
    add_int_bounds<eoIntAboveBound>("IntAboveBound",init<long int>());


    class_<std::vector<eoIntBounds*> >("BoundsVectorInt")
    .def(vector_indexing_suite<std::vector<eoIntBounds*> >())
    ;

    bool (eoBaseVectorBounds::* isBoundedInt1) (unsigned) = &eoBaseVectorBounds::isBounded;
    bool (eoBaseVectorBounds::* isBoundedInt2) (void)     = &eoBaseVectorBounds::isBounded;
    bool (eoBaseVectorBounds::* hasNoBoundAtAllInt1) (unsigned) = &eoBaseVectorBounds::hasNoBoundAtAll;
    bool (eoBaseVectorBounds::* hasNoBoundAtAllInt2) (void) = &eoBaseVectorBounds::hasNoBoundAtAll;
    bool (eoBaseVectorBounds::* isInBoundsInt1) (unsigned, long int) = &eoBaseVectorBounds::isInBounds;
    bool (eoBaseVectorBounds::* isInBoundsInt2) (const std::vector<long int>) = &eoBaseVectorBounds::isInBounds;


    class_<eoBaseVectorBounds, bases<std::vector<eoIntBounds*> > >("IntBaseVectorBounds", init<>())
    .def(init<
          unsigned,
          eoIntBounds&
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def("isBounded", isBoundedInt1)
    .def("isBounded", isBoundedInt2)
    .def("hasNoBoundAtAll", hasNoBoundAtAllInt1)
    .def("hasNoBoundAtAll", hasNoBoundAtAllInt2)
    .def("isMinBounded",&eoBaseVectorBounds::isMinBounded)
    .def("isMaxBounded",&eoBaseVectorBounds::isMaxBounded)
    .def("foldsInBounds",vec_foldsInBoundsWrap<eoBaseVectorBounds,long int>)
    .def("foldsInBounds",vec_foldsInBoundsWrap2<eoBaseVectorBounds,long int>)
    .def("truncate",vec_truncateWrap<eoBaseVectorBounds,long int>)
    .def("truncate",vec_truncateWrap2<eoBaseVectorBounds,long int>)
    .def("isInBounds",isInBoundsInt1)
    .def("isInBounds",isInBoundsInt2)
    .def("minimum",&eoBaseVectorBounds::minimum)
    .def("maximum",&eoBaseVectorBounds::maximum)
    .def("range",&eoBaseVectorBounds::range)
    .def("averageRange",&eoBaseVectorBounds::averageRange)
    .def("uniform", static_cast<double (eoBaseVectorBounds::*)(unsigned,eoRng&)>(&eoBaseVectorBounds::uniform),int_uniform1_overload(args("i","_rng"))
    )
    .def("uniform", uniformWrap<eoBaseVectorBounds>, int_uniform2_overload())
    ;


    //ATTENTION : order of constructors matters!
    // https://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor
    //only purpose is to add some convenient ctors...
    // class_<eoIntVectorBounds, bases<eoBaseVectorBounds> >("IntVectorBounds", init<>())
    // .def(init<
    //       std::vector<long int>,
    //       std::vector<long int>
    //   >()
    // )
    // ;
    // .def(init<
    //       unsigned,
    //       eoIntBounds&
    //   >()
    //   [
    //       with_custodian_and_ward<1, 3>()
    //   ]
    // )
    // // .def(init<
    // //       eoRealBounds&,
    // //       eoRealBounds&
    // //   >()
    // //   [
    // //       with_custodian_and_ward<1, 2,
    // //       with_custodian_and_ward<1, 3> >()
    // //   ]
    // // )
    // .def(init<
    //       unsigned,
    //       long int,
    //       long int
    //   >()
    // )
    // ;
} // bounds
