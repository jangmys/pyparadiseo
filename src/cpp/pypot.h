#ifndef PYPOT_H_
#define PYPOT_H_

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <eoVectorParticle.h>
#include <eoRealParticle.h>

#include <core/MOEO.h>
#include <core/moeoObjectiveVectorTraits.h>
#include <core/moeoObjectiveVector.h>
#include <core/moeoRealObjectiveVector.h>

#include <fitness.h>
#include <pyeot.h>

#include "utils/to_std_vector.h"
#include "utils/index_error.h"

namespace p=boost::python;
namespace np=boost::python::numpy;

// typedef DoubleFitness<FitnessTraits> doubleFitness;
// typedef moeoRealObjectiveVector<moeoObjectiveVectorTraits> realObjVec;


class PyPOT : public eoVectorParticle<doubleFitness,double,double>
{
public:

};
//
// template<class T>
// class RealSolution : public PyEOT
// {
// public:
//     typedef T PyEOT;
//
//     RealSolution(unsigned int _size) : PyEOT(
//         np::zeros(p::make_tuple(_size),np::dtype::get_builtin<double>())
//     ),_size(_size)
//     {
//     }
//
//     int size(){
//         return _size;
//         // return encoding.shape(0);//_size;
//     }
//
//     double& operator[](int i){
//         np::ndarray arr = np::from_object(encoding,np::dtype::get_builtin<double>());
//
//         double* ptr = reinterpret_cast<double*>(arr.get_data());
//
//         return ptr[i];
//     }
//
// private:
//     int _size;
// };
//
//
// class BinarySolution : public PyEOT
// {
// public:
//     BinarySolution(unsigned int _size) : PyEOT(),_size(_size)
//     {
//         encoding = np::zeros(p::make_tuple(_size),np::dtype::get_builtin<bool>());
//     }
//
//     size_t size(){
//         return _size;
//     }
//
//     bool& operator[](int i){
//         np::ndarray arr = np::from_object(encoding,np::dtype::get_builtin<bool>());
//
//         bool* ptr = reinterpret_cast<bool*>(arr.get_data());
//
//         return ptr[i];
//     }
//
// private:
//     size_t _size;
// };
//
//


#endif
