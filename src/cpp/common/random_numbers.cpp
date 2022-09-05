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

#include <utils/eoRNG.h>
#include <utils/eoRndGenerators.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

using namespace boost::python;

#include <sstream>
#include <boost/python/detail/api_placeholder.hpp>


// initialize static constants
const uint32_t eoRng::K(0x9908B0DFU);
const int eoRng::M(397);
const int eoRng::N(624);

namespace eo
{
    eoRng rng(time(0));
}

eoRng& get_rng() { return rng; }
// double normal(eoRng& rng) { return rng.normal(); }

std::string rng_to_string(const eoRng& _rng)
{
    std::ostringstream os;
    _rng.printOn(os);
    os << std::ends;
    return os.str();
}


void rng_from_string(eoRng& _rng, std::string s)
{
    std::istringstream is(s);
    _rng.readFrom(is);
}

struct RNG_pickle_suite : boost::python::pickle_suite
{
    static
    boost::python::tuple getstate(const eoRng& _rng)
    {
        return boost::python::make_tuple(str(rng_to_string(_rng)));
    }
    static
    void setstate(eoRng& _rng, boost::python::tuple pickled)
    {
        std::string state = extract<std::string>(pickled[0]);
        rng_from_string(_rng, state);
    }
};

int spin(eoRng& _rng, numpy::ndarray values, double total = 0.0)
{
    if (total == 0.0)
        {
            unsigned sz = len(values);
            for (unsigned i = 0; i < sz; ++i)
                {
                    total += extract<double>(values[i]); //extract?
                }
        }

    double chance = _rng.uniform() * total;

    int i = 0;
    while (chance >= 0.0)
        chance -= extract<double>(values[i++]);

    return --i;
}



BOOST_PYTHON_FUNCTION_OVERLOADS(spin_overload, spin, 2, 3)

BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(flip_overload, eoRng::flip, 0, 1)
//there is uniform(double m=1.0) and uniform(double min, double max)
//deal with overloads and default args...
BOOST_PYTHON_MEMBER_FUNCTION_OVERLOADS(unif_zeroM, eoRng::uniform, 0, 1)

double (eoRng::*normal1)(double) = &eoRng::normal;
double (eoRng::*normal2)(double,double) = &eoRng::normal;



//exposing eoRndGenerator and derived
template<typename T>
struct eoRndGeneratorWrap : eoRndGenerator<T>,wrapper<eoRndGenerator<T>>
{
    T operator()(void)
    {
        return this->get_override("operator()")();
    }
};





void random_numbers()
{
    class_<eoRng, boost::noncopyable>("eoRng", init<uint32_t>())
        .def("reseed", &eoRng::reseed)
        .def("uniform", static_cast<double (eoRng::*)(double)>(&eoRng::uniform),unif_zeroM())
        .def("uniform", static_cast<double (eoRng::*)(double,double)>(&eoRng::uniform))
        .def("random", &eoRng::random)
        .def("flip", &eoRng::flip, flip_overload())
        .def("binomial",&eoRng::binomial)
        .def("powerlaw",&eoRng::powerlaw)
        .def("normal", normal1)
        .def("normal", normal2)
        .def("negexp", &eoRng::negexp)
        .def("rand", &eoRng::rand)
        .def("rand_max", &eoRng::rand_max)
        .def("roulette_wheel", spin, spin_overload())
        .def("clearCache",&eoRng::clearCache)
        .def("to_string", rng_to_string)
        .def("from_string", rng_from_string)
        .def_pickle(RNG_pickle_suite())
        ;

    def("rng", get_rng, return_value_policy<reference_existing_object>());





    class_< eoRndGeneratorWrap<double>,boost::noncopyable >
    ("_eoRndGeneratorBaseDouble", init<>())
    .def("__call__",pure_virtual(&eoRndGenerator<double>::operator()))
    ;

    class_<eoUniformGenerator<double>, bases<eoRndGenerator<double>> >
    ("RealUniformGenerator",init<double>())
    .def(init<double,double>())
    .def(init<double,double,eoRng&>()
        [with_custodian_and_ward<1,4>()]
    )
    .def("__call__",&eoUniformGenerator<double>::operator())
    ;

    class_< eoRndGeneratorWrap<int>,boost::noncopyable >
    ("_eoRndGeneratorBaseInt", init<>())
    .def("__call__",pure_virtual(&eoRndGenerator<int>::operator()))
    ;

    class_<eoUniformGenerator<int>, boost::noncopyable>
    ("IntUniformGenerator",init<int>())
    .def(init<int,int>())
    .def(init<int,int,eoRng&>()
        [with_custodian_and_ward<1,4>()]
    )
    .def("__call__",&eoUniformGenerator<int>::operator())
    ;

}
