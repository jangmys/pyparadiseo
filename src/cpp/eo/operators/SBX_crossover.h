#ifndef SBX_CROSSOVER_H
#define SBX_CROSSOVER_H

#include <eoOp.h>
#include <utils/eoRealVectorBounds.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;


/**
* @ingroup Real
* @ingroup Variators
*/
template<class EOT> class eoSBXCrossover: public eoQuadOp<EOT>
{
 public:
  /****
   * (Default) Constructor.
   * The bounds are initialized with the global object that says: no bounds.
   *
   *
   */
    eoSBXCrossover(const double _eta = 1.0) :
    bounds(eoDummyVectorNoBounds), eta(_eta), range(1) {}


  //////////////////////////////////////////////

  /**
   * Constructor with bounds
   * @param _bounds an eoRealVectorBounds that contains the bounds
   * @param _eta the amount of exploration OUTSIDE the parents
   *               as in BLX-alpha notation (Eshelman and Schaffer)
   *               0 == contractive application
   *               Must be positive
   */
     eoSBXCrossover(eoRealVectorBounds & _bounds,
                     const double _eta = 1.0) :
     bounds(_bounds), eta(_eta), range(1) {}

  ///////////////////////////////////////////////

  //////////////////////////////////////////////

  /**
   * Constructor from a parser. Will read from the argument parser
   * eoRealVectorBounds that contains the bounds
   * eta, the SBX parameter
   */
    //  eoSBXCrossover(eoParser & _parser) :
    // // First, decide whether the objective variables are bounded
    // // Warning, must be the same keywords than other possible objectBounds elsewhere
    //    bounds (_parser.getORcreateParam(eoDummyVectorNoBounds, "objectBounds", "Bounds for variables", 'B', "Variation Operators").value()) ,
    // // then get eta value
    //    eta (_parser.getORcreateParam(1.0, "eta", "SBX eta parameter", '\0', "Variation Operators").value()) ,
    //    range(1) {}


  /// The class name.
  virtual std::string className() const { return "eoSBXCrossover"; }

  bool do_crossover(double* _eo1,double* _eo2,size_t _size)
  {
      unsigned i;
      double r1, r2, beta;

      for (i=0; i<_size; i++){
          double u = rng.uniform(range);

          if ( u <= 0.5 )
            beta = exp( (1/(eta+1))*log(2*u));
          else
            beta = exp((1/(eta+1))*log(1/(2*(1-u))));

            r1=*(_eo1+i);
            r2=*(_eo2+i);;
            *(_eo1+i) =0.5*((1+beta)*r1+(1-beta)*r2);
            *(_eo2+i) =0.5*((1-beta)*r1+(1+beta)*r2);

              if(!(bounds.isInBounds(i,*(_eo1+i))))
                bounds.foldsInBounds(i,*(_eo1+i));
              if(!(bounds.isInBounds(i,*(_eo2+i))))
                bounds.foldsInBounds(i,*(_eo2+i));
           }
        return true;
  }

  /*****************************************
   * SBX crossover - modifies both parents *
   * @param _eo1 The first parent          *
   * @param _eo2 The first parent          *
   *****************************************/
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

        return has_changed;
    }


protected:
  eoRealVectorBounds & bounds;
  double eta;
  double range;                    // == 1
};


#endif
