#ifndef UNIFORM_MUTATION_H
#define UNIFORM_MUTATION_H

#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

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
    PyUniformMutation(const double _epsilon, const double _p_change = 1.0) :
        homogeneous(true), bounds(eoDummyVectorNoBounds), epsilon(1, _epsilon),
        p_change(1, _p_change){ }

    /**
     * Constructor with bounds
     * @param _bounds an eoRealVectorBounds that contains the bounds
     * @param _epsilon the range for uniform mutation - a double to be scaled
     * @param _p_change the one probability to change all coordinates
     */
    PyUniformMutation(eoRealVectorBounds & _bounds,
      const double _epsilon, const double _p_change = 1.0) :
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

#endif
