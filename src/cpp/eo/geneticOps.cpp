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

template<typename SolutionType>
class GenOpWrapper : public eoGenOp<SolutionType>
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

    void apply(eoPopulator<SolutionType>& populator )
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


template<typename SolutionType>
class MonOpWrapper : public eoMonOp<SolutionType>
{
public:
    PyObject* self;
    MonOpWrapper(PyObject* p) : self(p) {}
    bool operator()(SolutionType& _eo)
    {
        return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ));
    }
};

template<typename SolutionType>
class BinOpWrapper : public eoBinOp<SolutionType>
{
public:
    PyObject* self;
    BinOpWrapper(PyObject* p) : self(p) {}
    bool operator()(SolutionType& _eo, const SolutionType& _eo2)
    { return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ), boost::ref(_eo2)); }
};

template<typename SolutionType>
class QuadOpWrapper : public eoQuadOp<SolutionType>
{
public:
    PyObject* self;
    QuadOpWrapper(PyObject* p) : self(p) {}
    bool operator()(SolutionType& _eo, SolutionType& _eo2)
    { return boost::python::call_method<bool>(self, "__call__", boost::ref( _eo ), boost::ref(_eo2)); }
};



template<typename SolutionType>
class PyMonOp : public eoMonOp<SolutionType>
{
public:
    PyMonOp() : eoMonOp<SolutionType>(){};

    PyMonOp(boost::python::object _op) :
        eoMonOp<SolutionType>(),
        mon_op(_op)
    { };

    boost::python::object get_op() const {
        return mon_op;
    }

    void
    set_op(boost::python::object obj)
    {
        // std::cout << "Setting generator\n";
        mon_op = obj;
    }

    bool
    operator () (SolutionType& _eo)
    {
        if (mon_op.ptr() != Py_None) {
            _eo.invalidate();
            // std::cout << "*** mut" << std::endl;
            // return boost::python::call<bool>(mon_op.ptr(),_eo.encoding);
            return mon_op(_eo.encoding);
        } else  {
            std::cout << "no MonOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object mon_op;
};


template<typename SolutionType>
class PyBinOp : public eoBinOp<SolutionType>
{
public:
    PyBinOp() : eoBinOp<SolutionType>(){};

    PyBinOp(boost::python::object _op) :
        eoBinOp<SolutionType>(),
        bin_op(_op)
    { };

    boost::python::object get_op() const {
        return bin_op;
    }

    void
    set_op(boost::python::object obj)
    {
        bin_op = obj;
    }

    bool
    operator () (SolutionType& _eo, const SolutionType& _eo2)
    {
        if (bin_op.ptr() != Py_None) {
            _eo.invalidate();
            bin_op(_eo.encoding,_eo2.encoding);
            return true;
            // return boost::python::call<bool>(bin_op.ptr(),_eo.encoding,_eo2.encoding);
        } else  {
            std::cout << "no BinOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object bin_op;
};

template<typename SolutionType>
class PyQuadOp : public eoQuadOp<SolutionType>
{
public:
    PyQuadOp() : eoQuadOp<SolutionType>(){};

    PyQuadOp(boost::python::object _op) :
        eoQuadOp<SolutionType>(),
        quad_op(_op)
    { };

    boost::python::object get_op() const {
        return quad_op;
    }

    void
    set_op(boost::python::object obj)
    {
        quad_op = obj;
    }

    bool
    operator () (SolutionType& _eo, SolutionType& _eo2)
    {
        if (quad_op.ptr() != Py_None) {
            _eo.invalidate();
            _eo2.invalidate();
            // std::cout << "*** mut" << std::endl;
            quad_op(_eo.encoding,_eo2.encoding);
            return true; //let's assume the quad_op modifies the eo's ;-)
            // return boost::python::call<bool>(quad_op.ptr(),_eo.encoding,_eo2.encoding);
        } else  {
            std::cout << "no QuadOp defined : do nothing";
            return false;
        }
    }

private:
    boost::python::object quad_op;
};

template<typename SolutionType>
void expose_gen_ops(std::string name)
{
    //base classes
    class_<eoMonOp<SolutionType>, MonOpWrapper<SolutionType>, bases<eoOp>, boost::noncopyable>(make_name("eoMonOp",name).c_str(), init<>())
        .def("__call__", &MonOpWrapper<SolutionType>::operator(), "an example docstring")
        ;

    class_<eoBinOp<SolutionType>, BinOpWrapper<SolutionType>, bases<eoOp>, boost::noncopyable>(make_name("eoBinOp",name).c_str(), init<>())
        .def("__call__", &BinOpWrapper<SolutionType>::operator());

    class_<eoQuadOp<SolutionType>, QuadOpWrapper<SolutionType>, bases<eoOp>, boost::noncopyable>(make_name("eoQuadOp",name).c_str(), init<>())
        .def("__call__", &QuadOpWrapper<SolutionType>::operator());

    class_<eoGenOp<SolutionType>, GenOpWrapper<SolutionType>, bases<eoOp>, boost::noncopyable>(make_name("eoGenOp",name).c_str(), init<>())
        .def("max_production", &GenOpWrapper<SolutionType>::max_production)
        .def("className", &GenOpWrapper<SolutionType>::className)
        .def("apply", &GenOpWrapper<SolutionType>::apply)
        .def("__call__", &eoGenOp<SolutionType>::operator())
    ;

    //wrappers with python callback
    class_<PyMonOp<SolutionType>,bases<eoMonOp<SolutionType>>>
        (make_name("pyMonOp",name).c_str(),init<optional<boost::python::object>>())
        .def("__call__", &PyMonOp<SolutionType>::operator ())
        .add_property("op", &PyMonOp<SolutionType>::get_op, &PyMonOp<SolutionType>::set_op)
    ;

    class_<PyBinOp<SolutionType>,bases<eoBinOp<SolutionType>>>
        (make_name("pyBinOp",name).c_str(),init<optional<boost::python::object>>())
        .def("__call__", &PyBinOp<SolutionType>::operator ())
        .add_property("op", &PyBinOp<SolutionType>::get_op, &PyBinOp<SolutionType>::set_op)
    ;

    class_<PyQuadOp<SolutionType>,bases<eoQuadOp<SolutionType>>>
        (make_name("pyQuadOp",name).c_str(),init<optional<boost::python::object>>())
        .def("__call__", &PyQuadOp<SolutionType>::operator ())
        .add_property("op", &PyQuadOp<SolutionType>::get_op, &PyQuadOp<SolutionType>::set_op)
    ;
}

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


    // enum_<eoOp<PyEOT>::OpType>("OpType")
    //     .value("unary", eoOp<PyEOT>::unary)
    //     .value("binary", eoOp<PyEOT>::binary)
    //     .value("quadratic", eoOp<PyEOT>::quadratic)
    //     .value("general", eoOp<PyEOT>::general)
    //     ;

    enum_<eoOp::OpType>("OpType")
        .value("unary", eoOp::unary)
        .value("binary", eoOp::binary)
        .value("quadratic", eoOp::quadratic)
        .value("general", eoOp::general)
        ;

    class_<eoOp>("eoOp", init<eoOp::OpType>())
        .def("getType", &eoOp::getType);

    expose_gen_ops<PyEOT>("");
    expose_gen_ops<BinarySolution>("Bin");
    expose_gen_ops<RealSolution>("Real");
    expose_gen_ops<IntSolution>("Perm");

    // class_<eoOp<PyEOT> >("eoOp", init<eoOp<PyEOT>::OpType>())
    //     .def("getType", &eoOp<PyEOT>::getType);

    // class_<eoMonOp<PyEOT>, MonOpWrapper<PyEOT>, bases<eoOp>, boost::noncopyable>("eoMonOp", init<>())
    //     .def("__call__", &MonOpWrapper<PyEOT>::operator(), "an example docstring");
    // class_<eoBinOp<PyEOT>, BinOpWrapper<PyEOT>, bases<eoOp>, boost::noncopyable>("eoBinOp", init<>())
    //     .def("__call__", &BinOpWrapper<PyEOT>::operator());
    // class_<eoQuadOp<PyEOT>, QuadOpWrapper<PyEOT>, bases<eoOp>, boost::noncopyable>("eoQuadOp", init<>())
    //     .def("__call__", &QuadOpWrapper<PyEOT>::operator());


    // class_<PyMonOp<PyEOT>,bases<eoMonOp<PyEOT>>>
    //     ("pyMonOp",init<optional<boost::python::object>>())
    //     // .def(init<>())
    //     .def("__call__", &PyMonOp<PyEOT>::operator ())
    //     .add_property("op", &PyMonOp<PyEOT>::get_op, &PyMonOp<PyEOT>::set_op)
    // ;

    // class_<PyBinOp,bases<eoBinOp<PyEOT>>,boost::noncopyable>
    //     ("pyBinOp",init<>())
    //     .def(init<boost::python::object>())
    //     .def("setOp", &PyBinOp::setOp)
    //     .def("__call__", &PyBinOp::operator ())
    // ;

    // class_<PyQuadOp,bases<eoQuadOp<PyEOT>>,boost::noncopyable>
    //     ("pyQuadOp",init<>())
    //     .def(init<boost::python::object>())
    //     .def("setOp", &PyQuadOp::setOp)
    //     .def("__call__", &PyQuadOp::operator ())
    // ;

    // class_<eoGenOp<PyEOT>, GenOpWrapper, bases<eoOp>, boost::noncopyable>("eoGenOp", init<>())
    //     .def("max_production", &GenOpWrapper::max_production)
    //     .def("className", &GenOpWrapper::className)
    //     .def("apply", &GenOpWrapper::apply)
    //     .def("__call__", &eoGenOp<PyEOT>::operator())
    //     ;



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
