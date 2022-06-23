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


class PyPOT : public PyEO
{
public:
    doubleFitness bestFitness;




};


template<typename PositionType>
class VectorParticle : public PyPOT
{
public:



    std::vector<PositionType> _position;
    std::vector<PositionType> _best_positions;
    std::vector<PositionType> _velocities;
};

#endif
