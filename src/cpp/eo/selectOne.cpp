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

class eoSelectOneWrapper : public eoSelectOne<PyEOT>
{
public:
    PyObject* self;
    eoSelectOneWrapper(PyObject* p) : self(p) {}
    const PyEOT& operator()(const eoPop<PyEOT>& pop)
    {
	return boost::python::call_method< const PyEOT& >(self, "__call__", boost::ref(pop));
    }
};

template <class Select>
void add_select(std::string name)
{
    class_<Select, bases<eoSelectOne<PyEOT> > >(name.c_str(), init<>() )
    .def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	// .def("__call__", &Select::operator(), return_internal_reference<>() )
	;
}

template <class Select, class Init>
void add_select(std::string name, Init init)
{
    class_<Select, bases<eoSelectOne<PyEOT> > >(name.c_str(), init)
	.def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	// .def("__call__", &Select::operator(), return_internal_reference<>() )
	;
}

template <class Select, class Init1, class Init2>
void add_select(std::string name, Init1 init1, Init2 init2)
{
    class_<Select, bases<eoSelectOne<PyEOT> > >(name.c_str(), init1)
	.def( init2 )
	.def("__call__", &Select::operator(), return_value_policy<copy_const_reference>() )
	// .def("__call__", &Select::operator(), return_internal_reference<>() )
	.def("setup", &Select::setup);
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

    class_<eoSelectOne<PyEOT>, eoSelectOneWrapper, boost::noncopyable>("eoSelectOne", init<>())
	.def("__call__", &eoSelectOneWrapper::operator(), return_value_policy<copy_const_reference>() ) //return_internal_reference<>() )
	.def("setup", &eoSelectOne<PyEOT>::setup);

    /* SelectOne derived classes */
    class_<eoDetTournamentSelect<PyEOT>, bases<eoSelectOne<PyEOT> > >("eoDetTournamentSelect", "Tournament Selection.",
    init<>(
        args("self"),"default tournament size = 2"
    ))
	.def(
        init<unsigned>(
            args("_tSize"),"tournament size"
        ))
	.def("__call__", &eoDetTournamentSelect<PyEOT>::operator(), return_value_policy<copy_const_reference>() )
	.def("setup", &eoDetTournamentSelect<PyEOT>::setup);

    add_select<eoStochTournamentSelect<PyEOT> >("eoStochTournamentSelect", init<>(), init<double>() );

    add_select<eoTruncatedSelectOne<PyEOT> >("eoTruncatedSelectOne",
					    init<eoSelectOne<PyEOT>&, double>()[WC1],
					    init<eoSelectOne<PyEOT>&, eoHowMany >()[WC1]
					    );

    // eoProportionalSelect is not feasible to implement at this point as fitness is not recognizable as a float
    // use eoDetTournament instead: with a t-size of 2 it is equivalent to eoProportional with linear scaling
    add_select<eoProportionalSelect<PyEOT> >("eoProportionalSelect", init<eoPop<PyEOT>&>() );

    add_select<eoRandomSelect<PyEOT> >("eoRandomSelect");
    add_select<eoBestSelect<PyEOT> >("eoBestSelect");
    add_select<eoNoSelect<PyEOT> >("eoNoSelect");

    add_select<eoSequentialSelect<PyEOT> >("eoSequentialSelect", init<>(), init<bool>());
    add_select<eoEliteSequentialSelect<PyEOT> >("eoEliteSequentialSelect");
    /*
     * eoSelectFromWorth.h:class eoSelectFromWorth : public eoSelectOne<EOT>
     */

    /* the EO part ...*/
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
