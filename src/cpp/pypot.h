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


template<typename PositionType>
class VectorParticle : public VectorSolution<PositionType>
{
public:
    typedef double ParticleVelocityType;

    VectorParticle(unsigned size = 0, double position = 0.0, double velocity = 0.0, double best_position = 0.0) : VectorSolution<double>(size),
            bestPositions(size,best_position),
            velocities(size,velocity),
            bestFitness(0.0)
    {};

    //needed on C++ (although bestFitness is not made private...)
    double best(){
        return bestFitness;
    }

    void best(const double& _best)
    {
        bestFitness = _best;
    }

    std::string repr() const
    {
        std::string s;

        s += "Particle(";
        s += VectorSolution<PositionType>::repr();
        s += ",";
        for(unsigned i=0;i<velocities.size();i++){
            s += boost::python::extract<const char*>(boost::python::str(velocities[i]));
            s += " ";
        }
        // s += boost::python::extract<const char*>(boost::python::str(encoding));
        s += ")";

        return s;
    }

    std::string to_string() const
    {
        std::string result;

        result += VectorSolution<PositionType>::to_string();
        result += "\t";

        result += "\n";
        result += "Best fitness: ";
        result += boost::python::extract<const char*>bestFitness;
        result += "\n";
        result += "Velocities: ";
        for(unsigned i=0;i<velocities.size();i++){
            result += boost::python::extract<const char*>(boost::python::str(velocities[i]));
            result += " ";
        }

        result += "\n";
        result += "BestPosition: ";
        result += "\n";
        for(unsigned i=0;i<bestPositions.size();i++){
            result += boost::python::extract<const char*>(boost::python::str(bestPositions[i]));
            result += " ";
        }

        return result;
    }


    // std::vector<PositionType> _position;
    std::vector<PositionType> bestPositions;
    std::vector<PositionType> velocities;

    doubleFitness bestFitness;
};

typedef VectorParticle<double> RealParticle;
typedef VectorParticle<int> IntParticle;

#endif
