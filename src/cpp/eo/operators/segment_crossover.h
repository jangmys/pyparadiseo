#ifndef SEGMENT_CROSSOVER_H
#define SEGMENT_CROSSOVER_H

#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

template <class EOT>
class PySegmentCrossover : public eoQuadOp<EOT>
{
public:

    /**
     * (Default) Constructor.
     * The bounds are initialized with the global object that says: no bounds.
     *
     * @param _alpha the amount of exploration OUTSIDE the parents
     *               as in BLX-alpha notation (Eshelman and Schaffer)
     *               0 == contractive application
     *               Must be positive
     */
    PySegmentCrossover(const double _alpha = 0.0)
        : bounds(eoDummyVectorNoBounds), alpha(_alpha), range(1 + 2 * _alpha){ }

    /**
     * Constructor with bounds
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _alpha the amount of exploration OUTSIDE the parents
     *               as in BLX-alpha notation (Eshelman and Schaffer)
     *               0 == contractive application
     *               Must be positive
     */
    PySegmentCrossover(eoRealVectorBounds & _bounds,
      const double                         _alpha = 0.0)
        : bounds(_bounds), alpha(_alpha), range(1 + 2 * _alpha){ }

    /// The class name.
    virtual std::string
    className() const { return "eoSegmentCrossover"; }


    void
    do_crossover(double * v1, double * v2, size_t len)
    {
        double alphaMin = -alpha;
        double alphaMax = 1 + alpha;
        double r1, r2, fact;

        if (alpha == 0.0) {                     // no check to perform
            fact = -alpha + rng.uniform(range); // in [-alpha,1+alpha)
        } else {                                // look for the bounds for fact
            for (unsigned i = 0; i < len; i++) {
                r1 = *(v1 + i);
                r2 = *(v2 + i);
                if (r1 != r2) { // otherwise you'll get NAN's
                    double rmin   = std::min(r1, r2);
                    double rmax   = std::max(r1, r2);
                    double length = rmax - rmin;
                    if (bounds.isMinBounded(i)) {
                        alphaMin = std::max(alphaMin, (bounds.minimum(i) - rmin) / length);
                        alphaMax = std::min(alphaMax, (rmax - bounds.minimum(i)) / length);
                    }
                    if (bounds.isMaxBounded(i)) {
                        alphaMax = std::min(alphaMax, (bounds.maximum(i) - rmin) / length);
                        alphaMin = std::max(alphaMin, (rmax - bounds.maximum(i)) / length);
                    }
                }
            }
            fact = alphaMin + (alphaMax - alphaMin) * rng.uniform();
        }

        for (unsigned i = 0; i < len; i++) {
            r1        = *(v1 + i);
            r2        = *(v2 + i);
            *(v1 + i) = fact * r1 + (1 - fact) * r2;
            *(v2 + i) = (1 - fact) * r1 + fact * r2;
        }
    } // do_crossover

    // Note: this works only with EOT's having
    // size() and double& operator[] (eoReal)
    //
    // ==> modify to work with numpy array!
    // make the operator work on std::vector<double> ?

    /**
     * segment crossover - modifies both parents
     * @param _eo1 The first parent
     * @param _eo2 The first parent
     */
    bool
    operator () (EOT& _eo1, EOT& _eo2)
    {
        np::dtype dt = np::dtype::get_builtin<double>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(_eo1.encoding, dt);
        np::ndarray e2 = np::from_object(_eo2.encoding, dt);

        int size = e1.shape(0);

        double * e1_ptr = reinterpret_cast<double *>(e1.get_data());
        double * e2_ptr = reinterpret_cast<double *>(e2.get_data());

        do_crossover(e1_ptr, e2_ptr, size);

        return true; // shoudl test if fact was 0 or 1 :-)))
    } // ()

protected:
    // eoRealVectorNoBounds eoDummyVectorNoBounds(0);

    eoRealVectorBounds & bounds;
    double alpha;
    double range; // == 1+2*alpha
};

#endif
