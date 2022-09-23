/*
    PyEO

    Copyright (C) 2003 Maarten Keijzer

    This program is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include <boost/python/copy_const_reference.hpp>
#include <boost/python/return_value_policy.hpp>

#include <eoSelectOne.h>
#include <eoDetTournamentSelect.h>
#include <eoRandomSelect.h>
#include <eoStochTournamentSelect.h>
#include <eoTruncatedSelectOne.h>
#include <eoSequentialSelect.h>
#include <eoSelectFromWorth.h>
#include <eoProportionalSelect.h>


#include <selection/moeoSelectOne.h>
#include <selection/moeoDetTournamentSelect.h>
#include <selection/moeoRouletteSelect.h>
#include <selection/moeoSelectFromPopAndArch.h>
#include <selection/moeoStochTournamentSelect.h>

#include <pyeot.h>
#include <utils/pickle.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

namespace bp = boost::python;


template<typename SolutionType>
class pySelectOne : public eoSelectOne<SolutionType>
{
public:
    pySelectOne() : eoSelectOne<SolutionType>(){};

    pySelectOne(boost::python::object _op) :
        eoSelectOne<SolutionType>(),
        select_op(_op)
    {};

    pySelectOne(boost::python::object _op,boost::python::object _setup) :
        eoSelectOne<SolutionType>(),
        select_op(_op),
        setup_op(_setup)
    {};


    const SolutionType& operator()(const eoPop<SolutionType>& _pop)
    {
        m_current = boost::python::call<SolutionType>(select_op.ptr(),_pop);
        return m_current;
    }

    void setup(const eoPop<SolutionType>& _pop)
    {
        if(setup_op.ptr() != Py_None){
            setup_op(_pop);
        }
    }

private:
    boost::python::object select_op;
    boost::python::object setup_op;

    SolutionType m_current;
};



template<typename SolutionType>
struct eoSelectOneWrap : eoSelectOne<SolutionType>,wrapper<eoSelectOne<SolutionType>>
{
    eoSelectOneWrap() : eoSelectOne<SolutionType>(){};

    const SolutionType& operator()(const eoPop<SolutionType>& _pop)
    {
        return this->get_override("operator()")(_pop);
    }

    void setup(const eoPop<SolutionType>& _pop)
    {
        if(override f = this->get_override("setup"))
        {
            f(_pop);
        }
        eoSelectOne<SolutionType>::setup(_pop);
    }
    void default_setup(const eoPop<SolutionType>& _pop)
    {
        this->eoSelectOne<SolutionType>::setup(_pop);
    }
};


template<typename SolutionType>
class eoSelectOneWrapper : public eoSelectOne<SolutionType>
{
public:
    PyObject* self;
    eoSelectOneWrapper(PyObject* p) : self(p) {}
    const SolutionType& operator()(const eoPop<SolutionType>& pop)
    {
	return boost::python::call_method<const SolutionType& >(self, "__call__", boost::ref(pop));
    }
};

template <class Select,class SolutionType>
void add_select(std::string name)
{
    class_<Select, bases<eoSelectOne<SolutionType> > >(name.c_str(), init<>() )
    .def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	// .def("__call__", &Select::operator(), return_internal_reference<>() )
	;
}

template <class Select,class SolutionType, class Init>
void add_select(std::string name, Init init)
{
    class_<Select, bases<eoSelectOne<SolutionType> > >(name.c_str(), init)
	.def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	// .def("__call__", &Select::operator(), return_internal_reference<>() )
	;
}

template <class Select,class SolutionType, class Init1, class Init2>
void add_select(std::string name, Init1 init1, Init2 init2)
{
    class_<Select, bases<eoSelectOne<SolutionType> > >(name.c_str(), init1)
	.def( init2 )
	// .def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	.def("__call__", &Select::operator(), return_internal_reference<>() )
	.def("setup", &Select::setup);
}

template<typename SolutionType>
void expose_selectOne(std::string name)
{
    class_<eoSelectOneWrap<SolutionType>,boost::noncopyable>
    (make_name("eoSelectOne",name).c_str(), init<>())
    .def("__call__",pure_virtual(&eoSelectOneWrap<SolutionType>::operator()),
        return_value_policy<copy_const_reference>()
        // return_internal_reference<>()
    )
    .def("setup",&eoSelectOne<SolutionType>::setup,&eoSelectOneWrap<SolutionType>::default_setup)
    ;

    // class_<pySelectOne<SolutionType>,bases<eoSelectOne<SolutionType>>
    class_<pySelectOne<SolutionType>,bases<eoSelectOne<SolutionType>> >
    (make_name("pySelectOne",name).c_str(), init<>())
    .def(init<boost::python::object>()[WC1])
    .def(init<boost::python::object,boost::python::object>()[WC2])
    .def("__call__",&pySelectOne<SolutionType>::operator(),
        return_value_policy<copy_const_reference>()
    )
    .def("setup",&pySelectOne<SolutionType>::setup)
    ;


    // class_<eoSelectOne<SolutionType>, eoSelectOneWrapper<SolutionType>, boost::noncopyable>(make_name("eoSelectOne",name).c_str(), init<>())
    // .def("__call__", &eoSelectOneWrapper<SolutionType>::operator(),
    // return_internal_reference<>() )
    // // return_value_policy<copy_const_reference>() ) //
    // .def("setup", &eoSelectOne<SolutionType>::setup);


    class_<eoDetTournamentSelect<SolutionType>, bases<eoSelectOne<SolutionType> > >(make_name("eoDetTournamentSelect",name).c_str(), "Tournament Selection.",
    init<>(
        args("self"),"default tournament size = 2"
    ))
    .def(
        init<unsigned>(
            args("_tSize"),"tournament size"
        ))
    .def("__call__", &eoDetTournamentSelect<SolutionType>::operator(), return_value_policy<copy_const_reference>() )
    .def("setup", &eoDetTournamentSelect<SolutionType>::setup)
    ;

    class_<eoStochTournamentSelect<SolutionType>,
        bases<eoSelectOne<SolutionType>>>
        (
            make_name("eoStochTournamentSelect",name).c_str(),
            init<optional<double>>()
        )
        .def("__call__",&eoStochTournamentSelect<SolutionType>::operator(), return_value_policy<copy_const_reference>()
        )
        .def("setup",&eoStochTournamentSelect<SolutionType>::setup)
        ;

    class_<eoTruncatedSelectOne<SolutionType>,
        bases<eoSelectOne<SolutionType>>>
        (
            make_name("eoTruncatedSelectOne",name).c_str(),
            init<eoSelectOne<SolutionType>&,double,optional<bool>>()[WC1]
        )
        .def(
            init<
                eoSelectOne<SolutionType>&,
                eoHowMany
            >()[WC1]
        )
        .def("__call__",&eoTruncatedSelectOne<SolutionType>::operator(), return_value_policy<copy_const_reference>()
        )
        .def("setup",&eoTruncatedSelectOne<SolutionType>::setup)
    ;

    // add_select<eoTruncatedSelectOne<SolutionType>,SolutionType>
    // (
    //     make_name("eoTruncatedSelectOne",name).c_str(),
    //     init<eoSelectOne<SolutionType>&, double, optional<bool>>()[WC1],
    //     init<eoSelectOne<SolutionType>&, eoHowMany >()[WC1]
    // );

    class_<eoProportionalSelect<SolutionType>,
        bases<eoSelectOne<SolutionType>>>
        (
            make_name("eoProportionalSelect",name).c_str(),
            init<>()
        )
        // .def(
        //     init<
        //         eoPop<SolutionType>&
        //     >()[WC1]
        // )
        .def("__call__",&eoProportionalSelect<SolutionType>::operator(), return_value_policy<copy_const_reference>()
        )
        .def("setup",&eoProportionalSelect<SolutionType>::setup)
    ;



    // add_select<eoProportionalSelect<SolutionType>,SolutionType>(
    //     make_name("eoProportionalSelect",name).c_str(),
    //     init<eoPop<SolutionType>&>()
    // );

    add_select<eoRandomSelect<SolutionType>,SolutionType>
    (make_name("eoRandomSelect",name).c_str());
    add_select<eoBestSelect<SolutionType>,SolutionType>
    (make_name("eoBestSelect",name).c_str());
    add_select<eoNoSelect<SolutionType>,SolutionType>
    (make_name("eoNoSelect",name).c_str());

    add_select<eoSequentialSelect<SolutionType>,SolutionType>
    (make_name("eoSequentialSelect",name).c_str(), init<>(), init<bool>());
    add_select<eoEliteSequentialSelect<SolutionType>,SolutionType>
    (make_name("eoEliteSequentialSelect",name).c_str());
}




void selectOne()
{
    /* the EO part ...*/

    pickle(class_<eoHowMany>("eoHowMany", init<>())
	   .def( init<double>() )
	   .def( init<double, bool>() )
	   .def( init<int>() )
	   .def("__call__", &eoHowMany::operator())
	   .def("__neg__", &eoHowMany::operator-)
	   );

    // class_<eoSelectOne<PyEOT>, eoSelectOneWrapper, boost::noncopyable>("eoSelectOne", init<>())
	// .def("__call__", &eoSelectOneWrapper::operator(), return_value_policy<copy_const_reference>() ) //return_internal_reference<>() )
	// .def("setup", &eoSelectOne<PyEOT>::setup);



    /* SelectOne derived classes */
    expose_selectOne<PyEOT>("");
    expose_selectOne<BinarySolution>("Bin");
    expose_selectOne<RealSolution>("Real");
}



class moeoSelectOneWrapper : public moeoSelectOne<PyEOT>
{
public:
    PyObject* self;
    moeoSelectOneWrapper(PyObject* p) : self(p) {}
    const PyEOT& operator()(const eoPop<PyEOT>& pop)
    {
	    return boost::python::call_method< const PyEOT& >(self, "__call__", boost::ref(pop));
    }
};


/*
selectors between EO and MOEO are different...
although it seems that EO selectors are just a special case of MOEO (comparators!!!)
*/
void PyEOTSelectOne()
{
    // class_<moeoSelectOne<PyEOT>,bases<eoSelectOne<PyEOT>>>
    // ("moeoSelectOne")
    // .def("__call__", pure_virtual(&moeoSelectOne::operator()), return_internal_reference<>() )
    // .def("setup", &moeoSelectOne<PyEOT>::setup);

    class_<moeoSelectOne<PyEOT>, moeoSelectOneWrapper, boost::noncopyable>("moeoSelectOne", init<>())
    .def("__call__", &moeoSelectOneWrapper::operator(), return_internal_reference<>() )
    .def("setup", &moeoSelectOne<PyEOT>::setup);

    class_<moeoDetTournamentSelect<PyEOT>,bases<moeoSelectOne<PyEOT> > >
    ("moeoDetTournamentSelect",
    init<
        moeoComparator<PyEOT>&,
        unsigned int
        >()
    )
    .def(init<
        unsigned int
        >()
    )
    .def("__call__",&moeoDetTournamentSelect<PyEOT>::operator(),return_internal_reference<>())
    .def("setup", &moeoDetTournamentSelect<PyEOT>::setup);
    ;

    class_<moeoStochTournamentSelect<PyEOT>,bases<moeoSelectOne<PyEOT> > >
    ("moeoStochTournamentSelect",
    init<
        moeoComparator<PyEOT>&,
        double
        >()
    )
    .def(init<
        double
        >()
    )
    .def("__call__",&moeoStochTournamentSelect<PyEOT>::operator(),return_internal_reference<>())
    .def("setup", &moeoStochTournamentSelect<PyEOT>::setup);
    ;

    //uses fitness!!
    class_<moeoRouletteSelect<PyEOT>,bases<moeoSelectOne<PyEOT> > >
    ("moeoRouletteSelect",
    init<
        unsigned int
        >()
    )
    .def("__call__",&moeoRouletteSelect<PyEOT>::operator(),return_internal_reference<>())
    .def("setup", &moeoRouletteSelect<PyEOT>::setup);
    ;

    class_<moeoSelectFromPopAndArch<PyEOT>,bases<moeoSelectOne<PyEOT> > >
    ("moeoSelectFromPopAndArch",
    init<
        moeoSelectOne<PyEOT>&,
        moeoSelectOne<PyEOT>&,
        moeoArchive<PyEOT>&,
        double
        >()
    )
    .def(init<
        moeoSelectOne<PyEOT>&,
        moeoArchive<PyEOT>&,
        double
        >()
    )
    .def("__call__",&moeoSelectFromPopAndArch<PyEOT>::operator(),return_internal_reference<>())
    .def("setup", &moeoSelectFromPopAndArch<PyEOT>::setup);
    ;
}
