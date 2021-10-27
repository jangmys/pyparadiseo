#ifndef INDEX_ERROR_H_
#define INDEX_ERROR_H_

#include <exception>
#include <boost/python.hpp>

struct index_error : public std::exception
{
    index_error(std::string w) : what(w) {};
    virtual ~index_error() throw() {}
    std::string what;
};

inline void translate_index_error(index_error const& e)
{
    PyErr_SetString(PyExc_IndexError, e.what.c_str());
}

#endif
