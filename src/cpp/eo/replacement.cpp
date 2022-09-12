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

template<typename SolutionType>
class pyReplace : public eoReplacement<SolutionType>
{
public:
    pyReplace(boost::python::object _op) : replace_op(_op){}

    void operator()(eoPop<SolutionType>& pop1, eoPop<SolutionType>& pop2)
    {
        boost::python::call<void>(replace_op.ptr(),boost::ref(pop1),boost::ref(pop2));
    }

private:
    boost::python::object replace_op;
};

template<typename SolutionType>
void expose_replacement(std::string name)
{
    class_<pyReplace<SolutionType>,bases<eoReplacement<SolutionType>>>(
        make_name("pyReplace",name).c_str(),
        init<boost::python::object>()
    )
    .def("__call__", &pyReplace<SolutionType>::operator())
    ;


    // eoReplacement.h
    class_<eoGenerationalReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoGenerationalReplacement",name).c_str(),
        init<>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoWeakElitistReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoWeakElitistReplacement",name).c_str(),
        init< eoReplacement<SolutionType>& >()[WC1])
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    // eoMergeReduce.h
    class_<eoMergeReduce<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoMergeReduce",name).c_str(),
        init<eoMerge<SolutionType>&, eoReduce<SolutionType>&>() [WC2]
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoPlusReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoPlusReplacement",name).c_str(),
        init<>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoCommaReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoCommaReplacement",name).c_str(),
        init<>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoEPReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoEPReplacement",name).c_str(),
        init<unsigned>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    // eoReduceMerge.h
    class_<eoReduceMerge<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoReduceMerge",name).c_str(),
        init<eoReduce<SolutionType>&, eoMerge<SolutionType>&>() [WC2]
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoSSGAWorseReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoSSGAWorseReplacement",name).c_str(),
        init<>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;


    class_<eoSSGADetTournamentReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoSSGADetTournamentReplacement",name).c_str(),
        init<unsigned>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;

    class_<eoSSGAStochTournamentReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoSSGAStochTournamentReplacement",name).c_str(),
        init<double>()
    )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;


    // eoReduceMergeReduce.h
    class_<eoReduceMergeReduce<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoReduceMergeReduce",name).c_str(),
        init<eoHowMany, bool, eoHowMany, eoReduce<SolutionType>&,
        eoHowMany, eoReduce<SolutionType>&, eoReduce<SolutionType>&>()
        [
            with_custodian_and_ward<1,5,
            with_custodian_and_ward<1,7,
            with_custodian_and_ward<1,8>>>()
        ]
    )
	.def("__call__", &eoReplacement<SolutionType>::operator())
    ;


    //eoMGGReplacement
    class_<eoMGGReplacement<SolutionType>, bases<eoReplacement<SolutionType>>>(
        make_name("eoMGGReplacement",name).c_str())
    .def( init<eoHowMany>() )
    .def( init<eoHowMany, unsigned>() )
    .def("__call__", &eoReplacement<SolutionType>::operator())
    ;
}




//
template<typename SolutionType>
void moeoreplacement(std::string name)
{
    // def_abstract_functor<moeoReplacement<PyEOT> >("moeoReplacement");

    class_<moeoElitistReplacement<SolutionType>,bases<moeoReplacement<SolutionType> > >
    (make_name("moeoElitistReplacement",name).c_str(),
    init<
        moeoFitnessAssignment < SolutionType > &,
        moeoDiversityAssignment < SolutionType > &,
        moeoComparator < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &,
        moeoDiversityAssignment < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &,
        moeoComparator < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &
        >()
    )
    .def("__call__",&moeoElitistReplacement<SolutionType>::operator())
    ;


    class_<moeoGenerationalReplacement<SolutionType>,bases< moeoReplacement<SolutionType>, eoGenerationalReplacement<SolutionType> > >
    (make_name("moeoGenerationalReplacement",name).c_str(),init<>()
    )
    .def("__call__",&moeoGenerationalReplacement<SolutionType>::operator())
    ;


    class_<moeoEnvironmentalReplacement<SolutionType>,bases<moeoReplacement<SolutionType>>>
    (make_name("moeoEnvironmentalReplacement",name).c_str(),
    init<
        moeoFitnessAssignment < SolutionType > &,
        moeoDiversityAssignment < SolutionType > &,
        moeoComparator < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &,
        moeoDiversityAssignment < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &,
        moeoComparator < SolutionType > &
        >()
    )
    .def( init<
        moeoFitnessAssignment < SolutionType > &
        >()
    )
    .def("__call__",&moeoEnvironmentalReplacement<SolutionType>::operator())
    ;


}


void replacement()
{
    expose_replacement<PyEOT>("");
    expose_replacement<BinarySolution>("Bin");
    expose_replacement<RealSolution>("Real");

    moeoreplacement<PyEOT>("");
    moeoreplacement<BinarySolution>("Bin");
    moeoreplacement<RealSolution>("Real");

}
