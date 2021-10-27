#ifndef TO_STD_VECTOR_H
#define TO_STD_VECTOR_H

#include <boost/python.hpp>
#include <boost/python/stl_iterator.hpp>

//convert an iterable python object to a std::vector<T>
template< typename T >
inline std::vector< T > to_std_vector( const boost::python::object& iterable )
{
    return std::vector< T >( boost::python::stl_input_iterator< T >( iterable ),
                             boost::python::stl_input_iterator< T >( ) );
}

#endif
