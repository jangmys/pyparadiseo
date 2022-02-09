#include "pyeot.h"
#include "pymo.h"

#include <neighborhood/moNeighbor.h>
#include <neighborhood/moDummyNeighbor.h>

#include <neighborhood/moNeighborhood.h>
#include <neighborhood/moDummyNeighborhood.h>
#include <neighborhood/moIndexNeighborhood.h>
#include <neighborhood/moOrderNeighborhood.h>

#include <neighborhood/moRndWithoutReplNeighborhood.h>
#include <neighborhood/moRndWithReplNeighborhood.h>


using namespace boost::python;

struct moNeighborhoodWrap : moNeighborhood<PyNeighbor>,
    wrapper<moNeighborhood<PyNeighbor>>
{
    public:
    moNeighborhoodWrap() : moNeighborhood(){}

    //pure virtuals
    bool hasNeighbor(PyEOT& _solution)
    {
        return this->get_override("hasNeighbor")(_solution);
    }

    void init(PyEOT& _solution, PyNeighbor& _current)
    {
        this->get_override("init")(_solution,_current);
    }

    void next(PyEOT& _solution, PyNeighbor& _current)
    {
        this->get_override("next")(_solution,_current);
    }

    bool cont(PyEOT& _solution)
    {
        return this->get_override("cont")(_solution);
    }

    //virtual with default
    bool isRandom()
    {
        if (override f = this->get_override("isRandom"))
        {
            return f();
        }
        return moNeighborhood<PyNeighbor>::isRandom();
    }
    bool default_isRandom() {
        return this->moNeighborhood<PyNeighbor>::isRandom();
    }
};


struct moIndexNeighborhoodWrap : moIndexNeighborhood<PyNeighbor>,
    wrapper<moIndexNeighborhood<PyNeighbor>>
{
    public:
    // moIndexNeighborhoodWrap() : moIndexNeighborhood<PyNeighbor>(){}

    moIndexNeighborhoodWrap(unsigned int _nhoodSize) : moIndexNeighborhood<PyNeighbor>(_nhoodSize){}

    //pure virtuals
    bool hasNeighbor(PyEOT& _solution)
    {
        return this->get_override("hasNeighbor")(_solution);
    }

    void init(PyEOT& _solution, PyNeighbor& _current)
    {
        this->get_override("init")(_solution,_current);
    }

    void next(PyEOT& _solution, PyNeighbor& _current)
    {
        this->get_override("next")(_solution,_current);
    }

    bool cont(PyEOT& _solution)
    {
        return this->get_override("cont")(_solution);
    }
};


template<typename X>
bool isRandom(X& _nhood)
{
    return true;
}


void moNeighborhoods()
{
    //ABC
    class_<moNeighborhoodWrap,boost::noncopyable>("moNeighborhood",init<>())
    .def("has_neighbor", pure_virtual(&moNeighborhood<PyNeighbor>::hasNeighbor))
    .def("init", pure_virtual(&moNeighborhood<PyNeighbor>::init))
    .def("next", pure_virtual(&moNeighborhood<PyNeighbor>::next))
    .def("cont", pure_virtual(&moNeighborhood<PyNeighbor>::cont))
    .def("is_random",&moNeighborhood<PyNeighbor>::isRandom,&moNeighborhoodWrap::default_isRandom)
    ;

    //ABC
    class_<moIndexNeighborhoodWrap,bases<moNeighborhood<PyNeighbor>>,boost::noncopyable>("moIndexNeighborhood",init<unsigned int>())
    .add_property("neighborhood_size",&moIndexNeighborhood<PyNeighbor>::getNeighborhoodSize,&moIndexNeighborhood<PyNeighbor>::setNeighborhoodSize)
    .def("has_neighbor", pure_virtual(&moIndexNeighborhood<PyNeighbor>::hasNeighbor))
    .def("init", pure_virtual(&moIndexNeighborhood<PyNeighbor>::init))
    .def("next", pure_virtual(&moIndexNeighborhood<PyNeighbor>::next))
    .def("cont", pure_virtual(&moIndexNeighborhood<PyNeighbor>::cont))
    ;

    class_<moOrderNeighborhood<PyNeighbor>,bases<moIndexNeighborhood<PyNeighbor>>>
    ("moOrderNeighborhood",init<>())
    .def(init<unsigned int>())
    .def("has_neighbor",&moOrderNeighborhood<PyNeighbor>::hasNeighbor)
    .def("init",&moOrderNeighborhood<PyNeighbor>::init)
    .def("next",&moOrderNeighborhood<PyNeighbor>::next)
    .def("cont",&moOrderNeighborhood<PyNeighbor>::cont)
    .add_property("position",&moOrderNeighborhood<PyNeighbor>::position,&moOrderNeighborhood<PyNeighbor>::setPosition)
    ;

    class_<moDummyNeighborhood<PyNeighbor>,bases<moNeighborhood<PyNeighbor>>>
    ("moDummyNeighborhood",init<>())
    .def("has_neighbor",&moDummyNeighborhood<PyNeighbor>::hasNeighbor)
    .def("init",&moDummyNeighborhood<PyNeighbor>::init)
    .def("next",&moDummyNeighborhood<PyNeighbor>::next)
    .def("cont",&moDummyNeighborhood<PyNeighbor>::cont)
    ;

    class_<moRndWithoutReplNeighborhood<PyNeighbor>,bases<moIndexNeighborhood<PyNeighbor>>>
    ("moRndWithoutReplNeighborhood",init<unsigned>())
    .def("has_neighbor",&moRndWithoutReplNeighborhood<PyNeighbor>::hasNeighbor)
    .def("init",&moRndWithoutReplNeighborhood<PyNeighbor>::init)
    .def("next",&moRndWithoutReplNeighborhood<PyNeighbor>::next)
    .def("cont",&moRndWithoutReplNeighborhood<PyNeighbor>::cont)
    .def("isRandom",isRandom<moRndWithoutReplNeighborhood<PyNeighbor>>)
    .def("position",&moRndWithoutReplNeighborhood<PyNeighbor>::position)
    ;

    class_<moRndWithReplNeighborhood<PyNeighbor>,bases<moIndexNeighborhood<PyNeighbor>>>
    ("moRndWithReplNeighborhood",init<unsigned>())
    .def("has_neighbor",&moRndWithReplNeighborhood<PyNeighbor>::hasNeighbor)
    .def("init",&moRndWithReplNeighborhood<PyNeighbor>::init)
    .def("next",&moRndWithReplNeighborhood<PyNeighbor>::next)
    .def("cont",&moRndWithReplNeighborhood<PyNeighbor>::cont)
    .def("isRandom",isRandom<moRndWithReplNeighborhood<PyNeighbor>>)
    ;

}
