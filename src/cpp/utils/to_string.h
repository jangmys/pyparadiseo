#ifndef TO_STRING_H
#define TO_STRING_H

template <class T>
inline boost::python::str to_py_str(T& _p)
{
    std::ostringstream os;
    os << _p;
    return boost::python::str(os.str().c_str());
}

#endif
