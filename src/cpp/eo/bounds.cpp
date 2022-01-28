#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/eoRealBounds.h>
#include <utils/eoRealVectorBounds.h>

namespace p = boost::python;
namespace np = boost::python::numpy;



struct eoRealBoundsWrap : eoRealBounds, p::wrapper<eoRealBounds> {
public:
    // pure virtuals
    bool
    isBounded() const
    {
        return this->get_override("isBounded")();
    }

    // pure virtual
    bool
    hasNoBoundAtAll() const
    {
        return this->get_override("hasNoBoundAtAll")();
    }

    // pure virtual
    bool
    isMinBounded() const
    {
        return this->get_override("hasNoBoundAtAll")();
    }

    // pure virtual
    bool
    isMaxBounded() const
    {
        return this->get_override("hasNoBoundAtAll")();
    }

    // pure virtual
    bool
    isInBounds(double d) const
    {
        return this->get_override("isInBounds")(d);
    }

    // pure virtual
    void
    foldsInBounds(double& d) const
    {
        this->get_override("foldsInBounds")(d);
    }

    // pure virtual
    void
    truncate(double& d) const
    {
        this->get_override("truncate")(d);
    }

    // pure virtual
    double
    minimum() const
    {
        return this->get_override("minimum")();
    }

    // pure virtual
    double
    maximum() const
    {
        return this->get_override("maximum")();
    }

    // pure virtual
    double
    range() const
    {
        return this->get_override("range")();
    }

    // pure virtual
    double
    uniform(eoRng& _rng) const
    {
        return this->get_override("uniform")(_rng);
    }

    eoRealBounds *
    dup() const
    {
        return this->get_override("dup")();
    }

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


//wrapper (needed because of immutable scalar arguments in python)
double foldsInBoundsWrap(eoRealBounds &real_bounds, double val) {
    real_bounds.foldsInBounds(val);
    return val;
};

double truncateWrap(eoRealBounds& real_bounds, double val) {
    real_bounds.truncate(val);
    return val;
}

//the same
double vec_foldsInBoundsWrap(eoRealVectorBounds &real_vec_bounds, unsigned i, double val) {
    real_vec_bounds[i]->foldsInBounds(val);
    return val;
};

//converter works only for rvalues
//copy code ... easier than converting ndarray to std::vector and back
void vec_foldsInBoundsWrap2(eoRealVectorBounds &real_vec_bounds, np::ndarray vec) {
    double* arr = reinterpret_cast<double*>(vec.get_data());

    for(unsigned i =0;i<real_vec_bounds.size();i++){
        real_vec_bounds[i]->foldsInBounds(arr[i]);
    }
};

double vec_truncateWrap(eoRealVectorBounds &real_vec_bounds, unsigned i, double val) {
    real_vec_bounds[i]->truncate(val);
    return val;
};

void vec_truncateWrap2(eoRealVectorBounds &real_vec_bounds, np::ndarray vec) {
    double* arr = reinterpret_cast<double*>(vec.get_data());

    for(unsigned i =0;i<real_vec_bounds.size();i++){
        real_vec_bounds[i]->truncate(arr[i]);
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
        .def("foldsInBounds",foldsInBoundsWrap)
        .def("truncate", truncateWrap)
        .def("minimum", &RealBounds::minimum)
        .def("maximum", &RealBounds::maximum)
        .def("range", &RealBounds::range)
        ;
}





#define DEF_FUN(x) .def("isBounded", &x::isBounded) \
    .def("hasNoBoundAtAll", &x::hasNoBoundAtAll) \
    .def("isMinBounded", &x::isMinBounded) \
    .def("isMaxBounded", &x::isMaxBounded) \
    .def("isInBounds", &x::isInBounds) \
    .def("foldsInBounds",foldsInBoundsWrap) \
    .def("truncate", truncateWrap) \
    .def("minimum", &x::minimum) \
    .def("maximum", &x::maximum) \
    .def("range", &x::range) \
    ;

#define DEF0(x) class_<x, bases<eoRealBounds>>(#x, init<>()) DEF_FUN(x)

#define DEF1(x,i1) class_<x, bases<eoRealBounds>>(#x, init<i1>()) DEF_FUN(x)

#define DEF2(x,i1,i2) class_<x, bases<eoRealBounds>>(#x, init<i1,i2>()) DEF_FUN(x)


void uniformWrap(eoRealBaseVectorBounds& _b, np::ndarray _v, eoRng & _rng = eo::rng)
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
BOOST_PYTHON_FUNCTION_OVERLOADS(uniform2_overload, uniformWrap, 2, 3)

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


    ////////// derived from RealBounds
    //////////////////////////////////////////////////
    add_real_bounds<eoRealNoBounds>("RealNoBounds",init<>());
    add_real_bounds<eoRealInterval>("RealInterval",init<double,double>());
    add_real_bounds<eoRealBelowBound>("RealBelowBound",init<double>());
    add_real_bounds<eoRealAboveBound>("RealAboveBound",init<double>());
    // DEF0(eoRealNoBounds)
    // DEF2(eoRealInterval,double,double)
    // DEF1(eoRealBelowBound,double)
    // DEF1(eoRealAboveBound,double)


    // expose do derive eoRealBaseVectorBounds
    class_<std::vector<eoRealBounds *> >("BoundsVector")
    .def(vector_indexing_suite<std::vector<eoRealBounds *> >())
    ;


    bool (eoRealBaseVectorBounds::* isBounded1) (unsigned) = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* isBounded2) (void)     = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* hasNoBoundAtAll1) (unsigned) = &eoRealBaseVectorBounds::hasNoBoundAtAll;
    bool (eoRealBaseVectorBounds::* hasNoBoundAtAll2) (void) = &eoRealBaseVectorBounds::hasNoBoundAtAll;
    bool (eoRealBaseVectorBounds::* isInBounds1) (unsigned, double) = &eoRealBaseVectorBounds::isInBounds;
    bool (eoRealBaseVectorBounds::* isInBounds2) (const std::vector<double>) = &eoRealBaseVectorBounds::isInBounds;




    class_<eoRealBaseVectorBounds, bases<std::vector<eoRealBounds *> > >("RealBaseVectorBounds", init<>())
    .def(init<
          unsigned,
          eoRealBounds&
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def("isBounded", isBounded1)
    .def("isBounded", isBounded2)
    .def("hasNoBoundAtAll", hasNoBoundAtAll1)
    .def("hasNoBoundAtAll", hasNoBoundAtAll2)
    .def("isMinBounded",&eoRealBaseVectorBounds::isMinBounded)
    .def("isMaxBounded",&eoRealBaseVectorBounds::isMaxBounded)
    .def("foldsInBounds",vec_foldsInBoundsWrap)
    .def("foldsInBounds",vec_foldsInBoundsWrap2)
    .def("truncate",vec_truncateWrap)
    .def("truncate",vec_truncateWrap2)
    .def("isInBounds",isInBounds1)
    .def("isInBounds",isInBounds2)
    .def("minimum",&eoRealBaseVectorBounds::minimum)
    .def("maximum",&eoRealBaseVectorBounds::maximum)
    .def("range",&eoRealBaseVectorBounds::range)
    .def("averageRange",&eoRealBaseVectorBounds::averageRange)
    .def("uniform", static_cast<double (eoRealBaseVectorBounds::*)(unsigned,eoRng&)>(&eoRealBaseVectorBounds::uniform),uniform1_overload(args("i","_rng"))
    )
    .def("uniform", uniformWrap, uniform2_overload())
    ;


    //ATTENTION : order of constructors matters!
    // https://stackoverflow.com/questions/8140155/boost-python-confused-about-similar-constructor
    class_<eoRealVectorBounds, bases<eoRealBaseVectorBounds> >("RealVectorBounds", init<>())
    .def(init<
          std::vector<double>,
          std::vector<double>
      >()
    )
    .def(init<
          unsigned,
          eoRealBounds&
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def(init<
          eoRealBounds&,
          eoRealBounds&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3> >()
      ]
    )
    .def(init<
          unsigned,
          double,
          double
      >()
    )
    ;

    class_<eoRealVectorNoBounds, bases<eoRealVectorBounds> >("RealVectorNoBounds", init<unsigned>())
    ;
} // bounds
