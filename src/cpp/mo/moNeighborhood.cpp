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

#include <utils/def_abstract_functor.h>


using namespace boost::python;

template<typename SolutionType>
struct moNeighborhoodWrap : moNeighborhood<PyNeighbor<SolutionType>>,
    wrapper<moNeighborhood<PyNeighbor<SolutionType>>>
{
    public:
    moNeighborhoodWrap() : moNeighborhood<PyNeighbor<SolutionType>>(){}

    //pure virtuals
    bool hasNeighbor(SolutionType& _solution)
    {
        return this->get_override("hasNeighbor")(_solution);
    }

    void init(SolutionType& _solution, PyNeighbor<SolutionType>& _current)
    {
        this->get_override("init")(_solution,_current);
    }

    void next(SolutionType& _solution, PyNeighbor<SolutionType>& _current)
    {
        this->get_override("next")(_solution,_current);
    }

    bool cont(SolutionType& _solution)
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
        return moNeighborhood<PyNeighbor<SolutionType>>::isRandom();
    }
    bool default_isRandom() {
        return this->moNeighborhood<PyNeighbor<SolutionType>>::isRandom();
    }
};

template<typename SolutionType>
struct moIndexNeighborhoodWrap : moIndexNeighborhood<PyNeighbor<SolutionType>>,
    wrapper<moIndexNeighborhood<PyNeighbor<SolutionType>>>
{
    public:
    // moIndexNeighborhoodWrap() : moIndexNeighborhood<PyNeighbor>(){}

    moIndexNeighborhoodWrap(unsigned int _nhoodSize) : moIndexNeighborhood<PyNeighbor<SolutionType>>(_nhoodSize){}

    //pure virtuals
    bool hasNeighbor(SolutionType& _solution)
    {
        return this->get_override("hasNeighbor")(_solution);
    }

    void init(SolutionType& _solution, PyNeighbor<SolutionType>& _current)
    {
        this->get_override("init")(_solution,_current);
    }

    void next(SolutionType& _solution, PyNeighbor<SolutionType>& _current)
    {
        this->get_override("next")(_solution,_current);
    }

    bool cont(SolutionType& _solution)
    {
        return this->get_override("cont")(_solution);
    }
};


template<typename X>
bool isRandom(X& _nhood)
{
    return true;
}

template<typename SolutionType>
void expose_moNeighborhoods(std::string name)
{
    typedef PyNeighbor<SolutionType> NborT;

    //ABC
    class_<moNeighborhoodWrap<SolutionType>,boost::noncopyable>(
        make_name("moNeighborhood",name).c_str(),
        init<>())
    .def("has_neighbor", pure_virtual(&moNeighborhood<NborT>::hasNeighbor))
    .def("init", pure_virtual(&moNeighborhood<NborT>::init))
    .def("next", pure_virtual(&moNeighborhood<NborT>::next))
    .def("cont", pure_virtual(&moNeighborhood<NborT>::cont))
    .def("is_random",&moNeighborhood<NborT>::isRandom,&moNeighborhoodWrap<SolutionType>::default_isRandom)
    ;

    //ABC
    class_<moIndexNeighborhoodWrap<SolutionType>,bases<moNeighborhood<NborT>>,boost::noncopyable>(
        make_name("moIndexNeighborhood",name).c_str(),
        init<unsigned int>())
    .add_property("neighborhood_size",&moIndexNeighborhood<NborT>::getNeighborhoodSize,&moIndexNeighborhood<NborT>::setNeighborhoodSize)
    .def("has_neighbor", pure_virtual(&moIndexNeighborhood<NborT>::hasNeighbor))
    .def("init", pure_virtual(&moIndexNeighborhood<NborT>::init))
    .def("next", pure_virtual(&moIndexNeighborhood<NborT>::next))
    .def("cont", pure_virtual(&moIndexNeighborhood<NborT>::cont))
    ;

    class_<moOrderNeighborhood<NborT>,bases<moIndexNeighborhood<NborT>>>(
        make_name("moOrderNeighborhood",name).c_str(),
        init<>())
    .def(init<unsigned int>())
    .def("has_neighbor",&moOrderNeighborhood<NborT>::hasNeighbor)
    .def("init",&moOrderNeighborhood<NborT>::init)
    .def("next",&moOrderNeighborhood<NborT>::next)
    .def("cont",&moOrderNeighborhood<NborT>::cont)
    .add_property("position",&moOrderNeighborhood<NborT>::position,&moOrderNeighborhood<NborT>::setPosition)
    ;

    class_<moDummyNeighborhood<NborT>,bases<moNeighborhood<NborT>>>(
        make_name("moDummyNeighborhood",name).c_str(),
        init<>())
    .def("has_neighbor",&moDummyNeighborhood<NborT>::hasNeighbor)
    .def("init",&moDummyNeighborhood<NborT>::init)
    .def("next",&moDummyNeighborhood<NborT>::next)
    .def("cont",&moDummyNeighborhood<NborT>::cont)
    ;

    class_<moRndWithoutReplNeighborhood<NborT>,bases<moIndexNeighborhood<NborT>>>(
        make_name("moRndWithoutReplNeighborhood",name).c_str(),
        init<unsigned>())
    .def("has_neighbor",&moRndWithoutReplNeighborhood<NborT>::hasNeighbor)
    .def("init",&moRndWithoutReplNeighborhood<NborT>::init)
    .def("next",&moRndWithoutReplNeighborhood<NborT>::next)
    .def("cont",&moRndWithoutReplNeighborhood<NborT>::cont)
    .def("isRandom",isRandom<moRndWithoutReplNeighborhood<NborT>>)
    .def("position",&moRndWithoutReplNeighborhood<NborT>::position)
    ;

    class_<moRndWithReplNeighborhood<NborT>,bases<moIndexNeighborhood<NborT>>>(
        make_name("moRndWithReplNeighborhood",name).c_str(),
        init<unsigned,optional<unsigned int>>())
    .def("has_neighbor",&moRndWithReplNeighborhood<NborT>::hasNeighbor)
    .def("init",&moRndWithReplNeighborhood<NborT>::init)
    .def("next",&moRndWithReplNeighborhood<NborT>::next)
    .def("cont",&moRndWithReplNeighborhood<NborT>::cont)
    .def("isRandom",isRandom<moRndWithReplNeighborhood<NborT>>)
    ;

}

void moNeighborhoods()
{
    expose_moNeighborhoods<PyEOT>("");
    expose_moNeighborhoods<BinarySolution>("Bin");
}
