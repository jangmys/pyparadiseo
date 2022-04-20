#ifndef DET_UNIFORM_MUTATION_H
#define DET_UNIFORM_MUTATION_H

#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

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
        np::ndarray ind = p::extract<np::ndarray>(_eo.get_encoding())();
        // np::ndarray ind = np::from_object(_eo.get_encoding(), dt);
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

#endif
