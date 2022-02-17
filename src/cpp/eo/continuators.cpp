/*
    PyMOEO

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

#include <eoGenContinue.h>
#include <eoCombinedContinue.h>
#include <eoSecondsElapsedContinue.h>
#include <eoEvalContinue.h>
#include <eoFitContinue.h>
#include <eoSteadyFitContinue.h>
#include <utils/eoCheckPoint.h>
#include <utils/eoStat.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

#define DEF3(x, i1, i2) class_<x<PyEOT>, bases<eoContinue<PyEOT > > >(#x, init<i1, i2 >() ).def("__call__", &eoContinue<PyEOT>::operator())

void add_checkpoint();

void continuators()
{
    /* Continuators */
    def_abstract_functor<eoContinue<PyEOT> >("eoContinue");

    /* Counters, wrappers etc */
    class_<eoEvalFuncCounter<PyEOT>, bases<eoEvalFunc<PyEOT> > >
        ("eoEvalFuncCounter",
         init< eoEvalFunc<PyEOT>&, optional<std::string>>()
         [
          with_custodian_and_ward<1, 2>()
         ]
        )
        .def("__call__", &eoEvalFuncCounter<PyEOT>::operator())
        ;

    class_<eoGenContinue<PyEOT>, bases<eoContinue<PyEOT> >, boost::noncopyable >
        ("eoGenContinue", init<unsigned long>() )
        .def("__call__", &eoGenContinue<PyEOT>::operator())
        ;

    class_<eoCombinedContinue<PyEOT>, bases<eoContinue<PyEOT> > >
        ("eoCombinedContinue", init<eoContinue<PyEOT>&>()[WC1])
        .def( init<eoContinue<PyEOT>&, eoContinue<PyEOT>& >()[WC2] )
        .def("add", &eoCombinedContinue<PyEOT>::add, WC1)
        .def("__call__", &eoCombinedContinue<PyEOT>::operator())
        ;

    class_<eoEvalContinue<PyEOT>, bases<eoContinue<PyEOT> > >
        ("eoEvalContinue",
         init<eoEvalFuncCounter<PyEOT>&, unsigned long>()[WC1]
         )
        .def("__call__", &eoEvalContinue<PyEOT>::operator())
        ;

    class_<eoSecondsElapsedContinue<PyEOT>,bases<eoContinue<PyEOT>>>
        ("eoSecondsElapsedContinue",init<int>())
        .def("__call__", &eoSecondsElapsedContinue<PyEOT>::operator())
        ;

    // JG : python object as fitness type? extraction done inside eoFitContinue? if r/o ... relying on [] operator...
    // DEF2(eoFitContinue, object); // object is the fitness type
    //
    DEF3(eoSteadyFitContinue, unsigned long, unsigned long);

    //
    add_checkpoint();
}

void addContinue(eoCheckPoint<PyEOT>& c, eoContinue<PyEOT>& cc) { c.add(cc); }
void addMonitor(eoCheckPoint<PyEOT>& c, eoMonitor& m) { c.add(m);}
void addStat(eoCheckPoint<PyEOT>& c, eoStatBase<PyEOT>& s) { c.add(s);}
void addSortedStat(eoCheckPoint<PyEOT>& c, eoSortedStatBase<PyEOT>& s) { c.add(s);}

void add_checkpoint()
{
    class_<eoCheckPoint<PyEOT>, bases< eoContinue<PyEOT> > >
        ("eoCheckPoint",
         init<eoContinue<PyEOT>&> ()[with_custodian_and_ward<1,2>()]
         )
        .def("add", addContinue, with_custodian_and_ward<1,2>() )
        .def("add", addMonitor, with_custodian_and_ward<1,2>() )
        .def("add", addStat, with_custodian_and_ward<1,2>())
        .def("add", addSortedStat, with_custodian_and_ward<1,2>())
        .def("__call__", &eoCheckPoint<PyEOT>::operator())
        ;
}
