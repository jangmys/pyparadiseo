#ifndef EO_POLYNOMIAL_MUTATION_H
#define EO_POLYNOMIAL_MUTATION_H

// #include <algorithm>    // swap_ranges
#include <utils/eoRealVectorBounds.h>

/**
 * @ingroup Real
 * @ingroup Variators
 */
template<class EOT> class eoPolynomialMutation: public eoMonOp<EOT>
{
public:
        /****
         * (Default) Constructor.
         * The bounds are initialized with the global object that says: no bounds.
         */
        eoPolynomialMutation(const double mutation_proba = 1.0, const double _eta = 1.0) :
                bounds(eoDummyVectorNoBounds), prob(mutation_proba), eta(_eta), range(1) {
        }

        //////////////////////////////////////////////
        eoPolynomialMutation(eoRealVectorBounds & _bounds,
                            const double mutation_proba = 1.0,
                             const double _eta = 1.0) :
                bounds(_bounds), prob(mutation_proba), eta(_eta), range(1) {
        }

        ///////////////////////////////////////////////

        //////////////////////////////////////////////

        /// The class name.
        virtual std::string className() const {
                return "eoPolynomialMutation";
        }

        /*****************************************
        * Polynomial Mutation
        *****************************************/
        bool operator()(EOT& _eo)
        {
            bool ret = false;

            for (unsigned i=0; i<_eo.size(); i++)
            {
                auto u = rng.uniform(range);

                if(u<prob){
                    auto yl = bounds.minimum(i);
                    auto yu = bounds.maximum(i);

                    double mut_pow = 1.0d/(1.0d+(double)eta);

                    u = rng.uniform(range);

                    if(u<=0.5){
                        auto delta_l = std::pow(2.0*u,mut_pow)-1.0;
                        _eo[i] += delta_l*(_eo[i]-yl);
                    }else{
                        auto delta_r = 1.0 - std::pow(2.0*(1.0-u),mut_pow);
                        _eo[i] += delta_r*(yu-_eo[i]);
                    }

                    if(!(bounds.isInBounds(i,_eo[i])))
                        bounds.foldsInBounds(i,_eo[i]);

                    ret = true;
                }
            }
            return ret;
        }

protected:
        eoRealVectorBounds & bounds;
        double prob;
        double eta;
        double range;              // == 1
};

#endif
