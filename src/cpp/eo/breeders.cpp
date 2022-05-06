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

#include <eoBreed.h>
#include <eoGeneralBreeder.h>
#include <eoOneToOneBreeder.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

#define DEF3(x, i1, i2)				\
    class_<x<PyEOT>, bases<eoBreed<PyEOT> > >	\
    (#x,					\
     init<i1, i2 >()				\
     [						\
      with_custodian_and_ward<1,2,		\
      with_custodian_and_ward<1,3		\
      >						\
      >						\
     ()						\
     ]						\
     )						\
    .def("__call__", &eoBreed<PyEOT>::operator())

template<typename SolutionType>
void expose_breeders(std::string name)
{
    class_<eoSelectTransform<SolutionType>, bases<eoBreed<SolutionType> > >
    (make_name("eoSelectTransform",name).c_str(),
        init<
            eoSelect<SolutionType>&,
            eoTransform<SolutionType>&
        >()[
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3>>()
        ]
    )
    .def("__call__", &eoBreed<SolutionType>::operator())
    ;


    class_<eoGeneralBreeder<SolutionType>, bases<eoBreed<SolutionType> > >
    (make_name("eoGeneralBreeder",name).c_str(),
        init<
            eoSelectOne<SolutionType>&,
            eoGenOp<SolutionType>&
        >()[
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3>>()
        ]
    )
    .def(
        init<eoSelectOne<SolutionType>&, eoGenOp<SolutionType>&, double>()[WC2]
    )
    .def(
        init<eoSelectOne<SolutionType>&, eoGenOp<SolutionType>&, double, bool>()[WC2]
    )
    .def(
        init<eoSelectOne<SolutionType>&, eoGenOp<SolutionType>&, eoHowMany&>()
        [
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3,
            with_custodian_and_ward<1,4>>>()
        ]
    )
    .def("__call__", &eoBreed<SolutionType>::operator())
    ;


    class_<eoOneToOneBreeder<SolutionType>, bases<eoBreed<SolutionType> > >
    (make_name("eoOneToOneBreeder",name).c_str(),
        init<
            eoGenOp<SolutionType>&,
            eoEvalFunc<SolutionType>&
        >()[
            with_custodian_and_ward<1,2,
            with_custodian_and_ward<1,3>>()
        ]
    )
    .def(
        init<
            eoGenOp<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            double
        >()[WC2]
    )
    .def(
        init<
            eoGenOp<SolutionType>&,
            eoEvalFunc<SolutionType>&,
            double,
            eoHowMany
        >()[WC2]
    )
    .def("__call__", &eoBreed<SolutionType>::operator())
    ;
}

void breeders()
{
    expose_breeders<PyEOT>("");
    expose_breeders<BinarySolution>("Bin");
}
