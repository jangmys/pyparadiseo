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

#include <eoReplacement.h>
#include <eoMergeReduce.h>
#include <eoReduceMerge.h>
#include <eoReduceMergeReduce.h>
#include <eoMGGReplacement.h>

#include <replacement/moeoReplacement.h>
#include <replacement/moeoElitistReplacement.h>
#include <replacement/moeoGenerationalReplacement.h>
#include <replacement/moeoEnvironmentalReplacement.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

#define DEF(x) class_<x<PyEOT>, bases<eoReplacement<PyEOT > > >(#x).def("__call__", &eoReplacement<PyEOT>::operator())
#define DEF2(x, i1) class_<x<PyEOT>, bases<eoReplacement<PyEOT > > >(#x, init<i1>() ).def("__call__", &eoReplacement<PyEOT>::operator())
#define DEF3(x, i1, i2) class_<x<PyEOT>, bases<eoReplacement<PyEOT > > >	\
    (#x,								\
     init<i1, i2 >() [WC2])						\
    .def("__call__", &eoReplacement<PyEOT>::operator())

void replacement()
{
    def_abstract_functor<eoReplacement<PyEOT> >("eoReplacement");

    // eoReplacement.h
    DEF(eoGenerationalReplacement);

    class_<eoWeakElitistReplacement<PyEOT>, bases<eoReplacement<PyEOT> > >
	("eoWeakElitistReplacement",
	 init< eoReplacement<PyEOT>& >()[WC1]);

    // eoMergeReduce.h
    DEF3(eoMergeReduce, eoMerge<PyEOT>&, eoReduce<PyEOT>& );
    DEF(eoPlusReplacement);
    DEF(eoCommaReplacement);
    DEF2(eoEPReplacement, unsigned);

    // eoReduceMerge.h
    DEF3(eoReduceMerge, eoReduce<PyEOT>&, eoMerge<PyEOT>& );
    DEF(eoSSGAWorseReplacement);
    DEF2(eoSSGADetTournamentReplacement, unsigned);
    DEF2(eoSSGAStochTournamentReplacement, double);

    // eoReduceMergeReduce.h
    //class_<eoReduceMergeReduce<PyEOT>, bases<eoReplacement<PyEOT> > >("eoReplacement",
    //	    init<eoHowMany, bool, eoHowMany, eoReduce<PyEOT>&,
    //		 eoHowMany, eoReduce<PyEOT>&, eoReduce<PyEOT>&>())
    //	.def("__call__", &eoReplacement<PyEOT>::operator());

    //eoMGGReplacement
    DEF(eoMGGReplacement)
	.def( init<eoHowMany>() )
	.def( init<eoHowMany, unsigned>() );
}



void moeoreplacement()
{
    def_abstract_functor<moeoReplacement<PyEOT> >("moeoReplacement");

    class_<moeoElitistReplacement<PyEOT>,bases<moeoReplacement<PyEOT> > >
    ("moeoElitistReplacement",
    init<
        moeoFitnessAssignment < PyEOT > &,
        moeoDiversityAssignment < PyEOT > &,
        moeoComparator < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &,
        moeoDiversityAssignment < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &,
        moeoComparator < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &
        >()
    )
    .def("__call__",&moeoElitistReplacement<PyEOT>::operator())
    ;

    class_<moeoGenerationalReplacement<PyEOT>,bases< moeoReplacement<PyEOT>, eoGenerationalReplacement<PyEOT> > >
    ("moeoGenerationalReplacement",init<>()
    )
    .def("__call__",&moeoGenerationalReplacement<PyEOT>::operator())
    ;

    class_<moeoEnvironmentalReplacement<PyEOT>,bases<moeoReplacement<PyEOT>>>
    ("moeoEnvironmentalReplacement",
    init<
        moeoFitnessAssignment < PyEOT > &,
        moeoDiversityAssignment < PyEOT > &,
        moeoComparator < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &,
        moeoDiversityAssignment < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &,
        moeoComparator < PyEOT > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < PyEOT > &
        >()
    )
    .def("__call__",&moeoEnvironmentalReplacement<PyEOT>::operator())
    ;


}
