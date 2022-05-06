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
#include <eoReduce.h>

#include <utils/def_abstract_functor.h>

using namespace boost::python;

template<typename SolutionType>
void expose_reduce(std::string name)
{
    class_<eoTruncate<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoTruncate",name).c_str(),
        init<>()
    )
	.def("__call__", &eoReduce<SolutionType>::operator())
	;

    class_<eoRandomReduce<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoRandomReduce",name).c_str()
    )
	.def("__call__", &eoReduce<SolutionType>::operator())
	;

    class_<eoEPReduce<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoEPReduce",name).c_str(),
        init<unsigned>()
    )
	.def("__call__", &eoReduce<SolutionType>::operator())
	;

    class_<eoLinearTruncate<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoLinearTruncate",name).c_str()
    )
	.def("__call__", &eoReduce<SolutionType>::operator())
	;

    class_<eoDetTournamentTruncate<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoDetTournamentTruncate",name).c_str(),
        init<unsigned>()
    )
	.def("__call__", &eoReduce<SolutionType>::operator())
	;

    class_<eoStochTournamentTruncate<SolutionType>, bases<eoReduce<SolutionType> > >(
        make_name("eoStochTournamentTruncate",name).c_str(),
        init<double>()
    )
    .def("__call__", &eoReduce<SolutionType>::operator())
	;
}



void reduce()
{
    expose_reduce<PyEOT>("");
    expose_reduce<BinarySolution>("Bin");
    expose_reduce<RealSolution>("Real");

    // class_<eoTruncate<PyEOT>, bases<eoReduce<PyEOT> > >("eoTruncate", init<>() )
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;

    // class_<eoRandomReduce<PyEOT>, bases<eoReduce<PyEOT> > >("eoRandomReduce")
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;

    // class_<eoEPReduce<PyEOT>, bases<eoReduce<PyEOT> > >("eoEPReduce", init<unsigned>())
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;

    // class_<eoLinearTruncate<PyEOT>, bases<eoReduce<PyEOT> > >("eoLinearTruncate")
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;

    // class_<eoDetTournamentTruncate<PyEOT>, bases<eoReduce<PyEOT> > >("eoDetTournamentTruncate", init<unsigned>())
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;

    // class_<eoStochTournamentTruncate<PyEOT>, bases<eoReduce<PyEOT> > >("eoStochTournamentTruncate", init<double>())
	// .def("__call__", &eoReduce<PyEOT>::operator())
	// ;
}
