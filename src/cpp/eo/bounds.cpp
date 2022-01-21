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



    bool (eoRealBaseVectorBounds::* isBounded1) (unsigned) = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* isBounded2) (void)     = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* hasNoBoundAtAll1) (unsigned) = &eoRealBaseVectorBounds::isBounded;
    bool (eoRealBaseVectorBounds::* hasNoBoundAtAll2) (void) = &eoRealBaseVectorBounds::isBounded;
    void (eoRealBaseVectorBounds::* foldsInBounds1) (unsigned, double&) = &eoRealBaseVectorBounds::foldsInBounds;
    void (eoRealBaseVectorBounds::* foldsInBounds2) (std::vector<double> &) = &eoRealBaseVectorBounds::foldsInBounds;
    void (eoRealBaseVectorBounds::* truncated1) (unsigned, double&) = &eoRealBaseVectorBounds::truncate;
    void (eoRealBaseVectorBounds::* truncated2) (std::vector<double> &) = &eoRealBaseVectorBounds::truncate;
    bool (eoRealBaseVectorBounds::* isInBounds1) (unsigned, double) = &eoRealBaseVectorBounds::isInBounds;
    bool (eoRealBaseVectorBounds::* isInBounds2) (const std::vector<double>) = &eoRealBaseVectorBounds::isInBounds;

    //TODO overload !
    // double (eoRealBaseVectorBounds::* uniform1) (unsigned,eoRng&) = &eoRealBaseVectorBounds::uniform;
    // void (eoRealBaseVectorBounds::* uniform2) (std:vector<double>&,eoRng&) = &eoRealBaseVectorBounds::uniform;


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
    .def("foldsInBounds",foldsInBounds1)
    .def("foldsInBounds",foldsInBounds2)
    .def("truncated",truncated1)
    .def("truncated",truncated2)
    .def("isInBounds",isInBounds1)
    .def("isInBounds",isInBounds2)
    .def("minimum",&eoRealBaseVectorBounds::minimum)
    .def("maximum",&eoRealBaseVectorBounds::maximum)
    .def("range",&eoRealBaseVectorBounds::range)
    .def("averageRange",&eoRealBaseVectorBounds::averageRange)
    // .def("uniform",uniform1)
    // .def("uniform",uniform2)
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
