/*
    PyEO

    Copyright (C) 2003 Maarten Keijzer
    2022 Jan Gmys, Inria, BONUS Team

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

#include <eoSelect.h>
#include <eoDetSelect.h>
#include <eoSelectMany.h>
#include <eoSelectNumber.h>
#include <eoSelectPerc.h>
#include <eoTruncSelect.h>
#include <eoTruncatedSelectMany.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

template<typename SolutionType>
class pySelect : public eoSelect<SolutionType>
{
public:
    pySelect(boost::python::object _op) :
        select_op(_op){}

    void operator()(const eoPop<SolutionType>& pop1,eoPop<SolutionType>& pop2)
    {
        boost::python::call<void>(select_op.ptr(),boost::ref(pop1),boost::ref(pop2));
    }

private:
    boost::python::object select_op;
};



template<typename SolutionType>
void expose_selectors(std::string name)
{
    class_<pySelect<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("pySelect",name).c_str(),
        init<boost::python::object>()[WC1]
    )
    .def("__call__",&pySelect<SolutionType>::operator())
    ;

    class_<eoDetSelect<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoDetSelect",name).c_str(),
    "deterministic select\n\n"
    "select many individuals deterministically\n",
    init<optional<double,bool>>(
            (arg("self"),arg("_rate"),arg("_interpret_as_rate")),
            "Construct a DetSelect.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )
    )
    .def("__call__", &eoDetSelect<SolutionType>::operator())
    ;

    class_<eoSelectMany<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoSelectMany",name).c_str(),
    "selects many individuals using eoSelectOne as it's mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor\n"
    "It will use an eoHowMany to determine the number of guys to select, and push them to the back of the destination population.",
    init<eoSelectOne<SolutionType>&,double,optional<bool>>(
            (arg("self"),arg("_selectOne"),arg("_rate"),arg("_as_rate")),
            "describe.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )[WC1]
    )
    .def( init<eoSelectOne<SolutionType>&,eoHowMany>()[WC1] )
    .def("__call__", &eoSelectMany<SolutionType>::operator())
    ;


    class_<eoSelectNumber<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoSelectNumber",name).c_str(),
    "eoSelectNumber selects many individuals using eoSelectOne as it's mechanism. Therefore eoSelectNumber needs an eoSelectOne in its ctor\n"
    "It will select a fixed number of individuals and pushes them to the back of the destination population.",
    init<eoSelectOne<SolutionType>&,optional<unsigned>>(
            (arg("self"),arg("_selectOne"),arg("_nb_to_select")),
            "describe.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )[WC1]
    )
    .def("__call__", &eoSelectNumber<SolutionType>::operator())
    ;


    class_<eoSelectPerc<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoSelectPerc",name).c_str(),
    "eoSelectPerc selects many individuals using eoSelectOne as it's mechanism. Therefore eoSelectPerc needs an eoSelectOne in its ctor\n"
    "It will select floor(rate*pop.size()) individuals and pushes them to the back of the destination population.",
    init<eoSelectOne<SolutionType>&,optional<float>>(
            (arg("self"),arg("_selectOne"),arg("_rate")),
            "describe.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )[WC1]
    )
    .def("__call__", &eoSelectPerc<SolutionType>::operator())
    ;


    class_<eoTruncSelect<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoTruncSelect",name).c_str(),
    "eoTruncSelect selects individuals after truncating the population using eoSelectOne as it's mechanism.\n"
     "Therefore eoSelectMany needs an eoSelectOne in its ctor. It will use an eoHowMnay to determine the number of guys to keep",
    init<eoSelectOne<SolutionType>&,eoHowMany>(
            (arg("self"),arg("_selectOne"),arg("_rate")),
            "describe.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )[WC1]
    )
    .def("__call__", &eoTruncSelect<SolutionType>::operator())
    ;


    class_<eoTruncatedSelectMany<SolutionType>,bases<eoSelect<SolutionType>>>(
        make_name("eoTruncatedSelectMany",name).c_str(),
    "eoTruncatedSelectMany selects many individuals using eoSelectOne as it's mechanism. Therefore eoSelectMany needs an eoSelectOne in its ctor\n"
    "It will use an eoHowMnay to determine the number of guys to select, and push them to the back of the destination population."
    "And it will only perform selection from the top guys in the population."
    "It is NOT a special case of eoSelectMany because it needs to SORT the population to discard the worst guys before doing the selection"
    "However, the same result can be obtained by embedding an eoTruncatedSelectOne into an eoSelectMany ...",
    init<eoSelectOne<SolutionType>&,double,double,optional<bool,bool>>(
            (arg("self"),arg("_selectOne"),arg("_rate")),
            "describe.\n\n"
            ":param _rate: a crossover\n"
            ":type _rate: double (optional)\n"
            ":param _interpret_as_rate: as-rate flag\n"
            ":type _interpret_as_rate: bool\n"
        )[WC1]
    )
    .def( init<eoSelectOne<SolutionType>&,eoHowMany,eoHowMany>()[WC1] )
    .def("__call__", &eoTruncatedSelectMany<SolutionType>::operator())
    ;
}


void selectors()
{
    expose_selectors<PyEOT>("");
    expose_selectors<BinarySolution>("Bin");
    expose_selectors<RealSolution>("Real");
}
