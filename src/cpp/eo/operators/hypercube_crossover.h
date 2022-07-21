#ifndef HYPERCUBE_CROSSOVER_H
#define HYPERCUBE_CROSSOVER_H

#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

/** eoHypercubeCrossover --> uniform choice in hypercube
                 == arithmetical with different values for each coordinate
\class eoArithmeticCrossover eoRealOp.h Tutorial/eoRealOp.h
 *
 * @ingroup Real
 * @ingroup Variators
*/
template<class EOT> class PyHypercubeCrossover: public eoQuadOp<EOT>
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
  PyHypercubeCrossover(const double& _alpha = 0.0):
    bounds(eoDummyVectorNoBounds), alpha(_alpha), range(1+2*_alpha)
  {
    if (_alpha < 0)
      throw eoParamException("BLX coefficient should be positive");
  }

  /**
   * Constructor with bounds
   * @param _bounds an eoRealVectorBounds that contains the bounds
   * @param _alpha the amount of exploration OUTSIDE the parents
   *               as in BLX-alpha notation (Eshelman and Schaffer)
   *               0 == contractive application
   *               Must be positive
   */
  PyHypercubeCrossover(eoRealVectorBounds & _bounds,
                        const double& _alpha = 0.0):
    bounds(_bounds), alpha(_alpha), range(1+2*_alpha)
  {
    if (_alpha < 0)
      throw eoParamException("BLX coefficient should be positive");
  }

  /// The class name.
  virtual std::string className() const { return "eoHypercubeCrossover"; }

  bool do_crossover(double* eo1,double* eo2,size_t _size)
  {
      bool hasChanged = false;
      unsigned i;
        double r1, r2, fact;
        if (alpha == 0.0)            // no check to perform
            for (i=0; i<_size; i++)
              {
                r1=*(eo1+i);
                r2=*(eo2+i);
                if (r1 != r2) {      // otherwise do nothing
                  fact = rng.uniform(range);       // in [0,1)
                  *(eo1+i) = fact * r1 + (1-fact) * r2;
                  *(eo2+i) = (1-fact) * r1 + fact * r2;
                  hasChanged = true; // forget (im)possible alpha=0
                }
              }
        else         // check the bounds
          // do not try to get a bound on the linear factor, but rather
          // on the object variables themselves
          for (i=0; i<_size; i++)
            {
              r1=*(eo1+i);
              r2=*(eo2+i);
              if (r1 != r2) {        // otherwise do nothing
                double rmin = std::min(r1, r2);
                double rmax = std::max(r1, r2);

                // compute min and max for object variables
                double objMin = -alpha * rmax + (1+alpha) * rmin;
                double objMax = -alpha * rmin + (1+alpha) * rmax;

                // first find the limits on the alpha's
                if (bounds.isMinBounded(i))
                  {
                    objMin = std::max(objMin, bounds.minimum(i));
                  }
                if (bounds.isMaxBounded(i))
                  {
                    objMax = std::min(objMax, bounds.maximum(i));
                  }
                // then draw variables
                double median = (objMin+objMax)/2.0; // uniform within bounds
                // double median = (rmin+rmax)/2.0;  // Bounce on bounds
                double valMin = objMin + (median-objMin)*rng.uniform();
                double valMax = median + (objMax-median)*rng.uniform();
                // don't always put large value in _eo1 - or what?
                if (rng.flip(0.5))
                  {
                    *(eo1+i) = valMin;
                    *(eo2+i) = valMax;
                  }
                else
                  {
                    *(eo1+i) = valMax;
                    *(eo2+i) = valMin;
                  }
                // seomthing has changed
                hasChanged = true; // forget (im)possible alpha=0
              }
            }

      return hasChanged;
  }

  /**
   * hypercube crossover - modifies both parents
   * @param _eo1 The first parent
   * @param _eo2 The first parent
   */
  bool operator()(EOT& _eo1, EOT& _eo2)
    {
        np::dtype dt = np::dtype::get_builtin<double>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(_eo1.encoding, dt);
        np::ndarray e2 = np::from_object(_eo2.encoding, dt);

        int size = e1.shape(0);

        double * e1_ptr = reinterpret_cast<double *>(e1.get_data());
        double * e2_ptr = reinterpret_cast<double *>(e2.get_data());

        bool has_changed = do_crossover(e1_ptr, e2_ptr, size);

        return has_changed; // shoudl test if fact was 0 or 1 :-)))
   }

protected:
  eoRealVectorBounds & bounds;
  double alpha;
  double range;                    // == 1+2*alphaMin
};

#endif
