#include <boost/python.hpp>
#include <boost/python/suite/indexing/vector_indexing_suite.hpp>

#include <utils/eoRealBounds.h>
#include <utils/eoRealVectorBounds.h>

using namespace boost::python;

struct eoRealBoundsWrap : eoRealBounds, wrapper<eoRealBounds> {
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


void
bounds()
{
    class_<eoRealBoundsWrap, boost::noncopyable>("RealBounds",
      init<>())
    .def("isBounded", pure_virtual(&eoRealBoundsWrap::isBounded))
    .def("hasNoBoundAtAll", pure_virtual(&eoRealBoundsWrap::hasNoBoundAtAll))
    .def("isMinBounded", pure_virtual(&eoRealBoundsWrap::isMinBounded))
    .def("isMaxBounded", pure_virtual(&eoRealBoundsWrap::isMaxBounded))
    .def("isInBounds", pure_virtual(&eoRealBoundsWrap::isBounded))
    .def("truncate", pure_virtual(&eoRealBoundsWrap::truncate))
    .def("minimum", pure_virtual(&eoRealBoundsWrap::minimum))
    .def("maximum", pure_virtual(&eoRealBoundsWrap::maximum))
    ;

    // expose do derive eoRealBaseVectorBounds
    class_<std::vector<eoRealBounds *> >("BoundsVector")
    .def(vector_indexing_suite<std::vector<eoRealBounds *> >())
    ;

    bool (eoRealBaseVectorBounds::* fx1) (unsigned) = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* fx2) (void)     = &eoRealBaseVectorBounds::isBounded;

    class_<eoRealBaseVectorBounds, bases<std::vector<eoRealBounds *> > >("RealBaseVectorBounds", init<>())
    .def(init<
          unsigned,
          eoRealBounds&
      >()
      [
          with_custodian_and_ward<1, 3>()
      ]
    )
    .def("isBounded", fx1)
    .def("isBounded", fx2)
    ;

    class_<eoRealVectorBounds, bases<eoRealBaseVectorBounds> >("RealVectorBounds", init<>())
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
