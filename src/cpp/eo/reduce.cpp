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

#include <eoReduce.h>

#include "PyMOEO.h"

using namespace boost::python;

// unfortunately have to define it specially
class eoReduceWrapper : public eoReduce<PyMOEO>
{
public:
    PyObject* self;
    eoReduceWrapper(PyObject* s) : self(s) {}
    void operator()(eoPop<PyMOEO>& pop, unsigned i)
    {
	boost::python::call_method<void>(self, "__call__", pop, i );
    }
};

void reduce()
{
    // ref trick in def_abstract_functor does not work for unsigned int :-(
    class_<eoReduce<PyMOEO>, eoReduceWrapper, boost::noncopyable>("eoReduce", init<>())
	.def("__call__", &eoReduceWrapper::operator());

    class_<eoTruncate<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoTruncate", init<>() )
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
    class_<eoRandomReduce<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoRandomReduce")
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
    class_<eoEPReduce<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoEPReduce", init<unsigned>())
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
    class_<eoLinearTruncate<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoLinearTruncate")
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
    class_<eoDetTournamentTruncate<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoDetTournamentTruncate", init<unsigned>())
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
    class_<eoStochTournamentTruncate<PyMOEO>, bases<eoReduce<PyMOEO> > >("eoStochTournamentTruncate", init<double>())
	.def("__call__", &eoReduce<PyMOEO>::operator())
	;
}
