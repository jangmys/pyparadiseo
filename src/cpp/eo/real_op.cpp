#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <eoRealOp.h>
#include <pyeot.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

// almost copies from eoRealOp.h .... not nice.
// PROBLEM: can't instantiate classes from eoRealOp unless EOT inherits from std::vector<double> (needs operator[], size(), ...).


template <class EOT> class PyUniformMutation : public eoMonOp<EOT>
{
public:

    /**
     * Constructor without bounds == unbounded variables :-)
     * not very clean, but who's doing unbounded optimization anyway?
     * and it's there mostly for backward compatibility
     *
     * @param _epsilon the range for uniform nutation
     * @param _p_change the probability to change a given coordinate
     */
    PyUniformMutation(const double& _epsilon, const double& _p_change = 1.0) :
        homogeneous(true), bounds(eoDummyVectorNoBounds), epsilon(1, _epsilon),
        p_change(1, _p_change){ }

    /**
     * Constructor with bounds
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _epsilon the range for uniform mutation - a double to be scaled
     * @param _p_change the one probability to change all coordinates
     */
    PyUniformMutation(eoRealVectorBounds & _bounds,
      const double& _epsilon, const double& _p_change = 1.0) :
        homogeneous(false), bounds(_bounds), epsilon(_bounds.size(), _epsilon),
        p_change(_bounds.size(), _p_change)
    {
        // scale to the range - if any
        for (unsigned i = 0; i < bounds.size(); i++)
            if (bounds.isBounded(i))
                epsilon[i] *= _epsilon * bounds.range(i);
    }

    /**
     * Constructor with bounds
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _epsilon the VECTOR of ranges for uniform mutation
     * @param _p_change the VECTOR of probabilities for each coordinates
     */
    PyUniformMutation(eoRealVectorBounds & _bounds,
      const std::vector<double>          & _epsilon,
      const std::vector<double>          & _p_change) :
        homogeneous(false), bounds(_bounds), epsilon(_epsilon),
        p_change(_p_change){ }

    /// The class name.
    virtual std::string
    className() const { return "eoUniformMutation"; }

    bool
    do_uniform_mutate(double * _ind, size_t _size)
    {
        bool hasChanged = false;

        if (homogeneous) { // implies no bounds object
            for (unsigned lieu = 0; lieu < _size; lieu++) {
                if (rng.flip(p_change[0])) {
                    _ind[lieu] += 2 * epsilon[0] * rng.uniform() - epsilon[0];
                    hasChanged  = true;
                }
            }
        } else {
            // sanity check ?
            if (_size != bounds.size())
                throw eoException("Invalid size of indi in eoUniformMutation");

            for (unsigned lieu = 0; lieu < _size; lieu++)
                if (rng.flip(p_change[lieu])) {
                    // check the bounds
                    double emin = *(_ind + lieu) - epsilon[lieu];
                    double emax = *(_ind + lieu) + epsilon[lieu];
                    if (bounds.isMinBounded(lieu))
                        emin = std::max(bounds.minimum(lieu), emin);
                    if (bounds.isMaxBounded(lieu))
                        emax = std::min(bounds.maximum(lieu), emax);
                    *(_ind + lieu) = emin + (emax - emin) * rng.uniform();
                    hasChanged     = true;
                }
        }
        return hasChanged;
    }

    /**
     * Do it!
     * @param _eo The indi undergoing the mutation
     */
    bool
    operator () (EOT& _eo)
    {
        np::dtype dt = np::dtype::get_builtin<double>();

        // get ndarray from object
        np::ndarray ind = np::from_object(_eo.encoding, dt);
        int size        = ind.shape(0);

        double * ind_ptr = reinterpret_cast<double *>(ind.get_data());

        bool did_change = do_uniform_mutate(ind_ptr, size);

        return did_change;
    }

private:
    bool homogeneous; // == no bounds passed in the ctor
    eoRealVectorBounds & bounds;
    std::vector<double> epsilon;  // the ranges for mutation
    std::vector<double> p_change; // the proba that each variable is modified
};


template <class EOT> class PyDetUniformMutation : public eoMonOp<EOT>
{
public:

    /**
     * (Default) Constructor for homogeneous genotype
     * it's there mostly for backward compatibility
     *
     * @param _epsilon the range for uniform nutation
     * @param _no number of coordinate to modify
     */
    PyDetUniformMutation(const double& _epsilon, const unsigned& _no = 1) :
        homogeneous(true), bounds(eoDummyVectorNoBounds),
        epsilon(1, _epsilon), no(_no){ }

    /**
     * Constructor with bounds
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _epsilon the range for uniform nutation (to be scaled if necessary)
     * @param _no number of coordinate to modify
     */
    PyDetUniformMutation(eoRealVectorBounds & _bounds,
      const double& _epsilon, const unsigned& _no = 1) :
        homogeneous(false), bounds(_bounds),
        epsilon(_bounds.size(), _epsilon), no(_no)
    {
        // scale to the range - if any
        for (unsigned i = 0; i < bounds.size(); i++)
            if (bounds.isBounded(i))
                epsilon[i] *= _epsilon * bounds.range(i);
    }

    /**
     * Constructor with bounds and full std::vector of epsilon
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _epsilon the VECTOR of ranges for uniform mutation
     * @param _no number of coordinates to modify
     */
    PyDetUniformMutation(eoRealVectorBounds & _bounds,
      const std::vector<double>             & _epsilon,
      const unsigned                        & _no = 1) :
        homogeneous(false), bounds(_bounds), epsilon(_epsilon), no(_no)
    {
        // scale to the range - if any
        for (unsigned i = 0; i < bounds.size(); i++)
            if (bounds.isBounded(i))
                epsilon[i] *= _epsilon[i] * bounds.range(i);
    }

    /// The class name.
    virtual std::string
    className() const { return "eoDetUniformMutation"; }


    bool
    do_det_uniform_mutate(double * ind, size_t _size)
    {
        if (homogeneous) {
            for (unsigned i = 0; i < no; i++) {
                unsigned lieu = rng.random(_size);
                // actually, we should test that we don't re-modify same variable!
                *(ind + lieu) = 2 * epsilon[0] * rng.uniform() - epsilon[0];
            }
        } else {
            // sanity check ?
            if (_size != bounds.size())
                throw eoException("Invalid size of indi in eoDetUniformMutation");
            for (unsigned i = 0; i < no; i++) {
                unsigned lieu = rng.random(_size);
                // actually, we should test that we don't re-modify same variable!

                // check the bounds
                double emin = *(ind + lieu) - epsilon[lieu];
                double emax = *(ind + lieu) + epsilon[lieu];
                if (bounds.isMinBounded(lieu))
                    emin = std::max(bounds.minimum(lieu), emin);
                if (bounds.isMaxBounded(lieu))
                    emax = std::min(bounds.maximum(lieu), emax);
                *(ind + lieu) = emin + (emax - emin) * rng.uniform();
            }
        }
        return true;
    }

    /**
     * Do it!
     * @param _eo The indi undergoing the mutation
     */
    bool
    operator () (EOT& _eo)
    {
        np::dtype dt = np::dtype::get_builtin<double>();

        // get ndarray from object
        np::ndarray ind = np::from_object(_eo.encoding, dt);
        int size        = ind.shape(0);

        double * ind_ptr = reinterpret_cast<double *>(ind.get_data());

        bool did_change = do_det_uniform_mutate(ind_ptr, size);

        return did_change;
    }

private:
    bool homogeneous; //  == no bounds passed in the ctor
    eoRealVectorBounds & bounds;
    std::vector<double> epsilon; // the ranges of mutation
    unsigned no;
};


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







void
real_op()
{
    using namespace boost::python;

    /*
     * C++ genetic operators need `double& operator[](int key)` for PyEOT (and size()...)
     *
     * np pb for size(), but I couldn't get lvalue converters to work, so...
     *
     * Solution (for now) : re-define classes as above, assuming that PyEOT.encoding is an ndarray with dtype double.
     *
     * TODO
     */
    class_<PyUniformMutation<PyEOT>, bases<eoMonOp<PyEOT> >, boost::noncopyable>
        ("UniformMutation", init<
          const double&,
          const double&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3
          > >()
      ]
        )
    .def(init<
          eoRealVectorBounds&,
          const double&,
          const double&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoRealVectorBounds&,
          const std::vector<double>&,
          const std::vector<double>&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &PyUniformMutation<PyEOT>::operator ())
    ;


    class_<PyDetUniformMutation<PyEOT>, bases<eoMonOp<PyEOT> >, boost::noncopyable>
        ("DetUniformMutation", init<
          const double&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3
          > >()
      ]
        )
    .def(init<
          eoRealVectorBounds&,
          const double&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def(init<
          eoRealVectorBounds&,
          const std::vector<double>&,
          const unsigned&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3,
          with_custodian_and_ward<1, 4
          > > >()
      ]
    )
    .def("__call__", &PyDetUniformMutation<PyEOT>::operator ())
    ;


    class_<PySegmentCrossover<PyEOT>, bases<eoQuadOp<PyEOT> >, boost::noncopyable>
        ("SegmentCrossover", init<double>()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
          eoRealVectorBounds&,
          const double
      >()
      [
          with_custodian_and_ward<1, 2>()
      ]
    )
    .def("__call__", &PySegmentCrossover<PyEOT>::operator ())
    ;


    class_<PyHypercubeCrossover<PyEOT>, bases<eoQuadOp<PyEOT> >, boost::noncopyable>
        ("HypercubeCrossover", init<double&>()
        [
            with_custodian_and_ward<1,2>()
        ]
    )
    .def(init<
          eoRealVectorBounds&,
          const double&
      >()
      [
          with_custodian_and_ward<1, 2,
          with_custodian_and_ward<1, 3
          >>()
      ]
    )
    .def("__call__", &PyHypercubeCrossover<PyEOT>::operator ())
    ;





} // real_op
