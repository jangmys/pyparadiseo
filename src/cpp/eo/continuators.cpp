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
#include <eoEvalContinue.h>
#include <eoFitContinue.h>
#include <eoSteadyFitContinue.h>
#include <utils/eoCheckPoint.h>
#include <utils/eoStat.h>

#include "PyMOEO.h"
#include "def_abstract_functor.h"

using namespace boost::python;

#define DEF(x) class_<x<PyMOEO>, bases<eoContinue<PyMOEO > > >(#x).def("__call__", &eoContinue<PyMOEO>::operator())
#define DEF2(x, i1) class_<x<PyMOEO>, bases<eoContinue<PyMOEO > > >(#x, init<i1>() ).def("__call__", &eoContinue<PyMOEO>::operator())
#define DEF3(x, i1, i2) class_<x<PyMOEO>, bases<eoContinue<PyMOEO > > >(#x, init<i1, i2 >() ).def("__call__", &eoContinue<PyMOEO>::operator())

void add_checkpoint();

void continuators()
{
    /* Continuators */
    def_abstract_functor<eoContinue<PyMOEO> >("eoContinue");

    /* Counters, wrappers etc */
    class_<eoEvalFuncCounter<PyMOEO>, bases<eoEvalFunc<PyMOEO> > >
        ("eoEvalFuncCounter",
         init< eoEvalFunc<PyMOEO>&, std::string>()
         [
          with_custodian_and_ward<1, 2>()
         ]
        )
        .def("__call__", &eoEvalFuncCounter<PyMOEO>::operator())
        ;

    class_<eoGenContinue<PyMOEO>, bases<eoContinue<PyMOEO> >, boost::noncopyable >
        ("eoGenContinue", init<unsigned long>() )
        .def("__call__", &eoGenContinue<PyMOEO>::operator())
        ;

    class_<eoCombinedContinue<PyMOEO>, bases<eoContinue<PyMOEO> > >
        ("eoCombinedContinue", init<eoContinue<PyMOEO>&>()[WC1])
        .def( init<eoContinue<PyMOEO>&, eoContinue<PyMOEO>& >()[WC2] )
        .def("add", &eoCombinedContinue<PyMOEO>::add, WC1)
        .def("__call__", &eoCombinedContinue<PyMOEO>::operator())
        ;

    class_<eoEvalContinue<PyMOEO>, bases<eoContinue<PyMOEO> > >
        ("eoEvalContinue",
         init<eoEvalFuncCounter<PyMOEO>&, unsigned long>()[WC1]
         )
        .def("__call__", &eoEvalContinue<PyMOEO>::operator())
        ;

    // JG : python object as fitness type? extraction done inside eoFitContinue? if r/o ... relying on [] operator...
    // DEF2(eoFitContinue, object); // object is the fitness type
    //
    DEF3(eoSteadyFitContinue, unsigned long, unsigned long);
    //
    add_checkpoint();
}

void addContinue(eoCheckPoint<PyMOEO>& c, eoContinue<PyMOEO>& cc) { c.add(cc); }
void addMonitor(eoCheckPoint<PyMOEO>& c, eoMonitor& m) { c.add(m);}
void addStat(eoCheckPoint<PyMOEO>& c, eoStatBase<PyMOEO>& s) { c.add(s);}
void addSortedStat(eoCheckPoint<PyMOEO>& c, eoSortedStatBase<PyMOEO>& s) { c.add(s);}

void add_checkpoint()
{
    class_<eoCheckPoint<PyMOEO>, bases< eoContinue<PyMOEO> > >
        ("eoCheckPoint",
         init<eoContinue<PyMOEO>&> ()[with_custodian_and_ward<1,2>()]
         )
        .def("add", addContinue, with_custodian_and_ward<1,2>() )
        .def("add", addMonitor, with_custodian_and_ward<1,2>() )
        .def("add", addStat, with_custodian_and_ward<1,2>())
        .def("add", addSortedStat, with_custodian_and_ward<1,2>())
        .def("__call__", &eoCheckPoint<PyMOEO>::operator())
        ;
}
