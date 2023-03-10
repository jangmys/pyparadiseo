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

#include <pyeot.h>
#include <utils/def_abstract_functor.h>
#include <eoMerge.h>

using namespace boost::python;

#define DEF(x) class_<x<PyEOT>, bases<eoMerge<PyEOT > > >(#x).def("__call__", &eoMerge<PyEOT>::operator())
#define DEF2(x, i1) class_<x<PyEOT>, bases<eoMerge<PyEOT > > >(#x, init<i1>() ).def("__call__", &eoMerge<PyEOT>::operator())
#define DEF3(x, i1, i2) class_<x<PyEOT>, bases<eoMerge<PyEOT > > >(#x, init<i1, i2 >() ).def("__call__", &eoMerge<PyEOT>::operator())

template<typename SolutionType>
void expose_mergers(std::string name)
{
    class_<eoElitism<SolutionType>, bases<eoMerge<SolutionType> > >(
        make_name("eoElitism",name).c_str(),
        init<double>()
    )
    .def( init<double, bool>() )
    .def("__call__", &eoMerge<SolutionType>::operator())
    ;

    class_<eoNoElitism<SolutionType>, bases<eoMerge<SolutionType> > >(
        make_name("eoNoElitism",name).c_str()
    )
    .def("__call__", &eoMerge<SolutionType>::operator())
    ;

    class_<eoPlus<SolutionType>, bases<eoMerge<SolutionType> > >(
        make_name("eoPlus",name).c_str()
    )
    .def("__call__", &eoMerge<SolutionType>::operator())
    ;
}



void mergers()
{
    expose_mergers<PyEOT>("");
    expose_mergers<BinarySolution>("Bin");
    expose_mergers<RealSolution>("Real");

    // DEF2(eoElitism, double)
    //     .def( init<double, bool>() );
    // DEF(eoNoElitism);
    // DEF(eoPlus);
}
