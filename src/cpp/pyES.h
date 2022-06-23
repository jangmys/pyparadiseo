#ifndef PYES_H_
#define PYES_H_

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

class PyEsSimple : public RealSolution
{
    PyEsSimple(unsigned int _size = 0):RealSolution(_size),stdev(0)
    {};

    PyEsSimple(const PyEsSimple& _es):RealSolution(_es),stdev(0)
    {};

    //    TODO add stdev to printOn...

    double stdev;
};

class PyEsStdev : public RealSolution
{
    PyEsStdev(unsigned int _size = 0):RealSolution(_size),stdevs(std::vector<double>(_size))
    {};

    PyEsStdev(const PyEsSimple& _es):RealSolution(_es),stdevs(_es.stdevs)
    {};

    //    TODO add stdev to printOn...

    std::vector<double> stdevs;
};

class PyEsFull : public RealSolution
{
    PyEsFull(unsigned int _size = 0):RealSolution(_size),stdevs(std::vector<double>(_size))
    {};

    PyEsFull(const PyEsSimple& _es):RealSolution(_es),stdevs(_es.stdevs)
    {};

    //    TODO add stdev to printOn...

    std::vector<double> stdevs;
    std::vector<double> correlations;
};





#endif
