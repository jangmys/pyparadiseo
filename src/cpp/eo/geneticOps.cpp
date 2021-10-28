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

#include <eoGenOp.h>
#include <eoSGAGenOp.h>
#include <eoOp.h>
#include <eoCloneOps.h>
#include <eoPopulator.h>
#include <eoOpContainer.h>

#include <pyeot.h>
#include <utils/def_abstract_functor.h>

using namespace boost::python;

class GenOpWrapper : public eoGenOp<PyEOT>
{
public:

    PyObject* self;
    GenOpWrapper(PyObject* p) : self(p) {}
    unsigned max_production(void)
    {
        return call_method<unsigned>(self,"max_production");
    }
    std::string className() const
    {
        return "GenOpDerivative"; // never saw the use of className anyway
    }

    void apply(eoPopulator<PyEOT>& populator )
    {
        boost::python::call_method<void>(self,"apply", boost::ref( populator ) );
    }
};

class PopulatorWrapper : public eoPopulator<PyEOT>
{
public:
    PyObject* self;
    PopulatorWrapper(PyObject* p, const eoPop<PyEOT>& src, eoPop<PyEOT>& dest)
        : eoPopulator<PyEOT>(src, dest), self(p)
    {
        //throw std::runtime_error("abstract base class");
    }

    const PyEOT& select()
    {
        return call_method<const PyEOT&>(self,"select");
    }
};

class MonOpWrapper : public eoMonOp<PyEOT>
{
public:
    PyObject* self;
    MonOpWrapper(PyObject* p) : self(p) {}
    bool operator()(PyEOT& _eo)
    {
        return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ));
    }
};

class BinOpWrapper : public eoBinOp<PyEOT>
{
public:
    PyObject* self;
    BinOpWrapper(PyObject* p) : self(p) {}
    bool operator()(PyEOT& _eo, const PyEOT& _eo2)
    { return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ), boost::ref(_eo2)); }
};

class QuadOpWrapper : public eoQuadOp<PyEOT>
{
public:
    PyObject* self;
    QuadOpWrapper(PyObject* p) : self(p) {}
    bool operator()(PyEOT& _eo, PyEOT& _eo2)
    { return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ), boost::ref(_eo2)); }
};


class PyMonOp : public eoMonOp<PyEOT>
{
public:
    PyMonOp() : eoMonOp<PyEOT>(){};

    PyMonOp(boost::python::object _op) :
        eoMonOp<PyEOT>(),
        mon_op(_op)
    { };

    void
    setOp(boost::python::object obj)
    {
        // std::cout << "Setting generator\n";
        mon_op = obj;
    }

    bool
    operator () (PyEOT& _eo)
    {
        if (mon_op.ptr() != Py_None) {
            _eo.invalidate();
            // std::cout << "*** mut" << std::endl;
            return boost::python::call<bool>(mon_op.ptr(),_eo.encoding);
        } else  {
            std::cout << "no MonOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object mon_op;
};


class PyBinOp : public eoBinOp<PyEOT>
{
public:
    PyBinOp() : eoBinOp<PyEOT>(){};

    PyBinOp(boost::python::object _op) :
        eoBinOp<PyEOT>(),
        bin_op(_op)
    { };

    void
    setOp(boost::python::object obj)
    {
        // std::cout << "Setting generator\n";
        bin_op = obj;
    }

    bool
    operator () (PyEOT& _eo, const PyEOT& _eo2)
    {
        if (bin_op.ptr() != Py_None) {
            _eo.invalidate();
            return boost::python::call<bool>(bin_op.ptr(),_eo.encoding,_eo2.encoding);
        } else  {
            std::cout << "no BinOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object bin_op;
};


class PyQuadOp : public eoQuadOp<PyEOT>
{
public:
    PyQuadOp() : eoQuadOp<PyEOT>(){};

    PyQuadOp(boost::python::object _op) :
        eoQuadOp<PyEOT>(),
        quad_op(_op)
    { };

    void
    setOp(boost::python::object obj)
    {
        // std::cout << "Setting generator\n";
        quad_op = obj;
    }

    bool
    operator () (PyEOT& _eo, PyEOT& _eo2)
    {
        if (quad_op.ptr() != Py_None) {
            _eo.invalidate();
            _eo2.invalidate();
            // std::cout << "*** mut" << std::endl;
            return boost::python::call<bool>(quad_op.ptr(),_eo.encoding,_eo2.encoding);
        } else  {
            std::cout << "no QuadOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object quad_op;
};








void geneticOps()
{
    class_<eoPopulator<PyEOT>, PopulatorWrapper, boost::noncopyable>
        ("eoPopulator", init<const eoPop<PyEOT>&, eoPop<PyEOT>&>() )
        .def("select", &PopulatorWrapper::select,  return_internal_reference<>() )
        .def("get", &eoPopulator<PyEOT>::operator*, return_internal_reference<>() )
        .def("next", &eoPopulator<PyEOT>::operator++, return_internal_reference<>() )
        .def("insert", &eoPopulator<PyEOT>::insert)
        .def("reserve", &eoPopulator<PyEOT>::reserve)
        .def("source", &eoPopulator<PyEOT>::source, return_internal_reference<>() )
        .def("offspring", &eoPopulator<PyEOT>::offspring, return_internal_reference<>() )
        .def("tellp", &eoPopulator<PyEOT>::tellp)
        .def("seekp", &eoPopulator<PyEOT>::seekp)
        .def("exhausted", &eoPopulator<PyEOT>::exhausted)
        ;

    class_<eoSeqPopulator<PyEOT>, bases<eoPopulator<PyEOT> > >
        ("eoSeqPopulator", init<const eoPop<PyEOT>&, eoPop<PyEOT>&>() )
        .def("select", &eoSeqPopulator<PyEOT>::select, return_internal_reference<>() )
        ;

    class_<eoSelectivePopulator<PyEOT>, bases<eoPopulator<PyEOT> > >
        ("eoSelectivePopulator", init<const eoPop<PyEOT>&, eoPop<PyEOT>&, eoSelectOne<PyEOT>& >() )
        .def("select", &eoSelectivePopulator<PyEOT>::select, return_internal_reference<>() )
        ;

    enum_<eoOp<PyEOT>::OpType>("OpType")
        .value("unary", eoOp<PyEOT>::unary)
        .value("binary", eoOp<PyEOT>::binary)
        .value("quadratic", eoOp<PyEOT>::quadratic)
        .value("general", eoOp<PyEOT>::general)
        ;

    class_<eoOp<PyEOT> >("eoOp", init<eoOp<PyEOT>::OpType>())
        .def("getType", &eoOp<PyEOT>::getType);

    class_<eoMonOp<PyEOT>, MonOpWrapper, bases<eoOp<PyEOT> >, boost::noncopyable>("eoMonOp", init<>())
        .def("__call__", &MonOpWrapper::operator(), "an example docstring");
    class_<eoBinOp<PyEOT>, BinOpWrapper, bases<eoOp<PyEOT> >, boost::noncopyable>("eoBinOp", init<>())
        .def("__call__", &BinOpWrapper::operator());
    class_<eoQuadOp<PyEOT>, QuadOpWrapper, bases<eoOp<PyEOT> >, boost::noncopyable>("eoQuadOp", init<>())
        .def("__call__", &QuadOpWrapper::operator());


    class_<PyMonOp,bases<eoMonOp<PyEOT>>,boost::noncopyable>
        ("pyMonOp",init<>())
        .def(init<boost::python::object>())
        .def("setOp", &PyMonOp::setOp)
        .def("__call__", &PyMonOp::operator ())
    ;

    class_<PyBinOp,bases<eoBinOp<PyEOT>>,boost::noncopyable>
        ("pyBinOp",init<>())
        .def(init<boost::python::object>())
        .def("setOp", &PyBinOp::setOp)
        .def("__call__", &PyBinOp::operator ())
    ;

    class_<PyQuadOp,bases<eoQuadOp<PyEOT>>,boost::noncopyable>
        ("pyQuadOp",init<>())
        .def(init<boost::python::object>())
        .def("setOp", &PyQuadOp::setOp)
        .def("__call__", &PyQuadOp::operator ())
    ;

    class_<eoGenOp<PyEOT>, GenOpWrapper, bases<eoOp<PyEOT> >, boost::noncopyable>("eoGenOp", init<>())
        .def("max_production", &GenOpWrapper::max_production)
        .def("className", &GenOpWrapper::className)
        .def("apply", &GenOpWrapper::apply)
        .def("__call__", &eoGenOp<PyEOT>::operator())
        ;



    class_<eoSequentialOp<PyEOT>, bases<eoGenOp<PyEOT> >, boost::noncopyable>("eoSequentialOp", init<>())
        .def("add", &eoSequentialOp<PyEOT>::add, WC1)
        .def("apply", &eoSequentialOp<PyEOT>::apply)
        ;

    class_<eoProportionalOp<PyEOT>, bases<eoGenOp<PyEOT> >, boost::noncopyable>("eoProportionalOp", init<>())
        .def("add", &eoProportionalOp<PyEOT>::add, WC1)
        .def("apply", &eoProportionalOp<PyEOT>::apply)
        ;

    // /* Cloning */
    class_<eoMonCloneOp<PyEOT>, bases<eoMonOp<PyEOT> > >("eoMonCloneOp").def("__call__", &eoMonCloneOp<PyEOT>::operator());
    class_<eoBinCloneOp<PyEOT>, bases<eoBinOp<PyEOT> > >("eoBinCloneOp").def("__call__", &eoBinCloneOp<PyEOT>::operator());
    class_<eoQuadCloneOp<PyEOT>, bases<eoQuadOp<PyEOT> > >("eoQuadCloneOp").def("__call__", &eoQuadCloneOp<PyEOT>::operator());

    class_<eoSGAGenOp<PyEOT>, bases<eoGenOp<PyEOT>>,boost::noncopyable>("eoSGAGenOp",
    init<
    eoQuadOp<PyEOT>&,
    double,
    eoMonOp<PyEOT>&,
    double
    >()
    [
    with_custodian_and_ward<1,2,
    with_custodian_and_ward<1,4
    >>()
    ]
    )
    .def("apply",&eoSGAGenOp<PyEOT>::apply)
    ;
}