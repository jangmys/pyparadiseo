#define NPY_NO_DEPRECATED_API NPY_1_7_API_VERSION
// #define PY_ARRAY_UNIQUE_SYMBOL MyParadiseo_PyArray_API

#include <numpy/arrayobject.h>
#include <boost/python.hpp>
#include <boost/python/numpy.hpp>
#include <numpy/arrayscalars.h>
#include <boost/python/to_python_converter.hpp>

// // #include "iterable_converter.h"
//
//
//geeeeeeeeeeeeeeeez!!!!!!!!!!!
//https://stackoverflow.com/questions/47026900/pyarray-check-gives-segmentation-fault-with-cython-c
int init_numpy(){
    _import_array(); // PyError if not successful
    return 0;
}

const static int numpy_initialized =  init_numpy();
//
//
// //Scalar converters
// //=================
// //https://github.com/ukoethe/vigra/blob/master/vigranumpy/src/core/converters.cxx
template <typename ScalarType>
struct NumpyScalarConverter
{
    NumpyScalarConverter()
    {
        using namespace boost::python;
        converter::registry::push_back( &convertible, &construct, type_id<ScalarType>());
    }

    // Determine if obj_ptr is a supported numpy.number
    static void* convertible(PyObject* obj_ptr)
    {
        if (PyArray_IsScalar(obj_ptr, Float32) ||
            PyArray_IsScalar(obj_ptr, Float64) ||
            PyArray_IsScalar(obj_ptr, Int8)    ||
            PyArray_IsScalar(obj_ptr, Int16)   ||
            PyArray_IsScalar(obj_ptr, Int32)   ||
            PyArray_IsScalar(obj_ptr, Int64)   ||
            PyArray_IsScalar(obj_ptr, UInt8)   ||
            PyArray_IsScalar(obj_ptr, UInt16)  ||
            PyArray_IsScalar(obj_ptr, UInt32)  ||
            PyArray_IsScalar(obj_ptr, UInt64) ||
            PyArray_IsScalar(obj_ptr, Bool))
        {
            return obj_ptr;
        }
        return 0;
    }

    static void construct( PyObject* obj_ptr, boost::python::converter::rvalue_from_python_stage1_data* data)
    {
        using namespace boost::python;

        // Grab pointer to memory into which to construct the C++ scalar
        void* storage = ((converter::rvalue_from_python_storage<ScalarType>*) data)->storage.bytes;

        // in-place construct the new scalar value
        ScalarType * scalar = new (storage) ScalarType;

        if (PyArray_IsScalar(obj_ptr, Float32))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Float32);
        else if (PyArray_IsScalar(obj_ptr, Float64))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Float64);
        else if (PyArray_IsScalar(obj_ptr, Int8))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Int8);
        else if (PyArray_IsScalar(obj_ptr, Int16))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Int16);
        else if (PyArray_IsScalar(obj_ptr, Int32))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Int32);
        else if (PyArray_IsScalar(obj_ptr, Int64))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Int64);
        else if (PyArray_IsScalar(obj_ptr, UInt8))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, UInt8);
        else if (PyArray_IsScalar(obj_ptr, UInt16))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, UInt16);
        else if (PyArray_IsScalar(obj_ptr, UInt32))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, UInt32);
        else if (PyArray_IsScalar(obj_ptr, UInt64))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, UInt64);
        else if (PyArray_IsScalar(obj_ptr, Bool))
            (*scalar) = PyArrayScalar_VAL(obj_ptr, Bool);


        // Stash the memory chunk pointer for later use by boost.python
        data->convertible = storage;
    }
};


void registerConverters()
{
    // NumpyScalarConverter<signed char>();
    // NumpyScalarConverter<short>();
    NumpyScalarConverter<int>();
    NumpyScalarConverter<long>();
    NumpyScalarConverter<long long>();
    // NumpyScalarConverter<unsigned char>();
    // NumpyScalarConverter<unsigned short>();
    NumpyScalarConverter<unsigned int>();
    NumpyScalarConverter<unsigned long>();
    NumpyScalarConverter<unsigned long long>();
    NumpyScalarConverter<float>();
    NumpyScalarConverter<double>();
};
