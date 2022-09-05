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
#include <pso/pypot.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;

// #define DEF3(x, i1, i2) class_<x<PyEOT>, bases<eoContinue<PyEOT > > >(#x, init<i1, i2 >() ).def("__call__", &eoContinue<PyEOT>::operator())

void add_checkpoint();

template<typename SolutionType>
void expose_continuators(std::string name)
{
    /* Continuators */
    // def_abstract_functor<eoContinue<PyEOT> >("eoContinue");

    /* Counters, wrappers etc */

    // EVAL FUNC !!
    class_<eoEvalFuncCounter<SolutionType>, bases<eoEvalFunc<SolutionType> > >
        (make_name("eoEvalFuncCounter",name).c_str(),
         init< eoEvalFunc<SolutionType>&, optional<std::string>>()
         [
          with_custodian_and_ward<1, 2>()
         ]
        )
        .def("__call__", &eoEvalFuncCounter<SolutionType>::operator())
        ;

    // CONTINUATORS
    class_<eoGenContinue<SolutionType>, bases<eoContinue<SolutionType> > >//, boost::noncopyable >
        (make_name("eoGenContinue",name).c_str(), init<unsigned long>() )
        .def("__call__", &eoGenContinue<SolutionType>::operator())
        // .def("totalGenerations",&eoGenContinue<SolutionType>::totalGenerations)
        ;

    class_<eoCombinedContinue<SolutionType>, bases<eoContinue<SolutionType> > >
        (make_name("eoCombinedContinue",name).c_str(), init<eoContinue<SolutionType>&>()[WC1])
        .def( init<eoContinue<SolutionType>&, eoContinue<SolutionType>& >()[WC2] )
        .def("add", &eoCombinedContinue<SolutionType>::add, WC1)
        .def("__call__", &eoCombinedContinue<SolutionType>::operator())
        ;

    class_<eoEvalContinue<SolutionType>, bases<eoContinue<SolutionType> > >
        (make_name("eoEvalContinue",name).c_str(),
         init<eoEvalFuncCounter<SolutionType>&, unsigned long>()[WC1]
         )
        .def("__call__", &eoEvalContinue<SolutionType>::operator())
        ;

    class_<eoSecondsElapsedContinue<SolutionType>,bases<eoContinue<SolutionType>>>
        (make_name("eoSecondsElapsedContinue",name).c_str(),init<int>())
        .def("__call__", &eoSecondsElapsedContinue<SolutionType>::operator())
        ;

    // JG : python object as fitness type? extraction done inside eoFitContinue? if r/o ... relying on [] operator...
    class_<eoFitContinue<SolutionType>, bases<eoContinue<SolutionType > > >
    (make_name("eoFitContinue",name).c_str(), init<doubleFitness>())
    .def("__call__", &eoFitContinue<SolutionType>::operator())
    ;

    class_<eoSteadyFitContinue<SolutionType>, bases<eoContinue<SolutionType > > >
    (make_name("eoSteadyFitContinue",name).c_str(), init<unsigned long,unsigned long, optional<unsigned long>>())
    .def("__call__", &eoSteadyFitContinue<SolutionType>::operator())
    ;
}

void continuators()
{
    expose_continuators<PyEOT>("");
    expose_continuators<BinarySolution>("Bin");
    expose_continuators<RealSolution>("Real");
    expose_continuators<IntSolution>("Perm");
    expose_continuators<RealParticle>("RealPart");
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
