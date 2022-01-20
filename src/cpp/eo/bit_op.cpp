#include <eoOp.h>
// #include <utils/eoRealVectorBounds.h>
// #include <eoBinOp.h>
#include <pyeot.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

namespace p  = boost::python;
namespace np = boost::python::numpy;

// almost copies from eoBitOp.h .... not nice.
// PROBLEM: can't instantiate classes from eoRealOp unless EOT inherits from std::vector<double> (needs operator[], size(), ...).

template <class Chrom> class PyOneBitFlip : public eoMonOp<Chrom>
{
public:
    /// The class name.
    // virtual std::string className() const { return "eoOneBitFlip"; }

    /**
     * Change one bit.
     * @param chrom The cromosome which one bit is going to be changed.
     */
    bool
    operator () (Chrom& chrom)
    {
        np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom.encoding, dt);
        int size       = e1.shape(0);

        int * e1_ptr = reinterpret_cast<int *>(e1.get_data());

        unsigned i = eo::rng.random(size);
        *(e1_ptr+i) = !(*(e1_ptr+i));
        return true;
    }
};

template<class Chrom> class PyDetBitFlip: public eoMonOp<Chrom>
{
 public:
  /**
   * (Default) Constructor.
   * @param _num_bit The number of bits to change
   * default is one - equivalent to eoOneBitFlip then
   */
  PyDetBitFlip(const unsigned _num_bit = 1): num_bit(_num_bit) {}

  /// The class name.
  // virtual std::string className() const { return "eoDetBitFlip"; }

  /**
   * Change num_bit bits.
   * @param chrom The cromosome which one bit is going to be changed.
   */
  bool operator()(Chrom& chrom)
    {
        np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom.encoding, dt);
        int size       = e1.shape(0);

        int * e1_ptr = reinterpret_cast<int *>(e1.get_data());

          // for duplicate checking see eoDetSingleBitFlip
          for (unsigned k=0; k<num_bit; k++)
            {
              unsigned i = eo::rng.random(size);
              *(e1_ptr+i) = !(*(e1_ptr+i));
              // chrom[i] = !chrom[i];
            }
          return true;
    }
 private:
  unsigned num_bit;
};



/** Changes exactly k bits with checking for duplicate
\ingroup bitstring
*/
template<class Chrom>
class PyDetSingleBitFlip: public eoMonOp<Chrom>
{
 public:
  /**
   * (Default) Constructor.
   * @param _num_bit The number of bits to change
   * default is one - equivalent to eoOneBitFlip then
   */
  PyDetSingleBitFlip(const unsigned _num_bit = 1): num_bit(_num_bit) {}

  /// The class name.
  // virtual std::string className() const { return "eoDetSingleBitFlip"; }

  /**
   * Change num_bit bits.
   * @param chrom The cromosome which one bit is going to be changed.
   */
  bool operator()(Chrom& chrom)
  {
    np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e = np::from_object(chrom.encoding, dt);
        unsigned _size       = e.shape(0);

        int * e_ptr = reinterpret_cast<int *>(e.get_data());

      // All possible indices
      std::vector< unsigned > indices;
      indices.reserve(_size);
      for(unsigned i=0; i<_size; ++i) {
          indices.push_back(i);
      }

      // Shuffle indices
      for(unsigned i=0; i<indices.size(); ++i) {
          unsigned other = eo::rng.random(indices.size());
          std::swap(indices[i], indices[other]);
      }

      // Flip at first indices
      assert(num_bit <= _size);
      for(unsigned i=0; i<num_bit; ++i) {
          *(e_ptr+indices[i]) = !(*(e_ptr+indices[i]));
        // chrom[indices[i]] = !chrom[indices[i]];
      }

      if(num_bit > 0) {
          return true;
      } else {
          return false;
      }
  }

  void number_bits(const unsigned _num_bit)
  {
      num_bit = _num_bit;
  }

 protected:
  unsigned num_bit;
};




/** eoBitMutation --> classical mutation
\class eoBitMutation eoBitOp.h ga/eoBitOp.h
\ingroup bitstring
*/
template<class Chrom> class PyBitMutation: public eoMonOp<Chrom>
{
 public:
  /**
   * (Default) Constructor.
   * @param _rate Rate of mutation.
   * @param _normalize use rate/chrom.size if true
   */
  PyBitMutation(const double _rate = 0.01, bool _normalize=false):
    rate(_rate), normalize(_normalize) {}

  /// The class name.
  // virtual std::string className() const { return "eoBitMutation"; }

  /**
   * Mutate a chromosome.
   * @param chrom The chromosome to be mutated.
   */
  bool operator()(Chrom& chrom)
    {
        np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e = np::from_object(chrom.encoding, dt);
        unsigned _size       = e.shape(0);

        int * e_ptr = reinterpret_cast<int *>(e.get_data());


      double actualRate = (normalize ? rate/_size : rate);
      bool changed_something = false;
      for (unsigned i = 0; i < _size; i++)
            if (eo::rng.flip(actualRate))
        {
            *(e_ptr) = !(*(e_ptr));
            // chrom[i] = !chrom[i];
            changed_something = true;
        }

        return changed_something;
    }

 protected:
  double rate;
  bool normalize;                  // divide rate by chromSize
};


template<class Chrom> class PyBitInversion: public eoMonOp<Chrom>
{
 public:
  /// The class name.
  // virtual std::string className() const { return "eoBitInversion"; }

  /**
   * Inverts a range of bits in a binary chromosome.
   * @param chrom The chromosome whos bits are going to be inverted (a range).
   */
  bool operator()(Chrom& chrom)
    {
        np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e = np::from_object(chrom.encoding, dt);
        unsigned _size       = e.shape(0);

        int * e_ptr = reinterpret_cast<int *>(e.get_data());

      unsigned u1 = eo::rng.random(_size + 1);
      unsigned u2;
      do u2 = eo::rng.random(_size + 1); while (u1 == u2);
      unsigned r1 = std::min(u1, u2);
      unsigned r2 = std::max(u1, u2);

      r2--;
      while(r1<r2){
          int tmp = e[r1];
          e[r1++] = e[r2];
          e[r2--] = tmp;
      }

      // std::reverse(chrom.begin() + r1, chrom.begin() + r2);
      return true;
    }
};


/** eo1PtBitXover --> classic 1-point crossover
\class eo1PtBitCrossover eoBitOp.h ga/eoBitOp.h
\ingroup bitstring
*/

template<class Chrom> class Py1PtBitXover: public eoQuadOp<Chrom>
{
 public:
  /// The class name.
  // virtual std::string className() const { return "eo1PtBitXover"; }

  /**
   * 1-point crossover for binary chromosomes.
   * @param chrom1 The first chromosome.
   * @param chrom2 The first chromosome.
   */
  bool operator()(Chrom& chrom1, Chrom& chrom2)
    {
        np::dtype dt = np::dtype::get_builtin<int>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom1.encoding, dt);
        np::ndarray e2 = np::from_object(chrom2.encoding, dt);

        unsigned _size2       = e2.shape(0);

        std::vector<int> ind1(reinterpret_cast<int *>(e1.get_data()),
            reinterpret_cast<int *>(e1.get_data())+e1.shape(0));
        std::vector<int> ind2(reinterpret_cast<int *>(e2.get_data()),
            reinterpret_cast<int *>(e2.get_data())+e2.shape(0));

        //longueur différente ????
      unsigned site = eo::rng.random(std::min(e1.shape(0), e2.shape(0)));

      if (!std::equal(ind1.begin(), ind1.begin()+site, ind2.begin()))
      {

        std::swap_ranges(ind1.begin(), ind1.begin() + site, ind2.begin());

        return true;
      }
      return false;
  }
};





void
bit_op()
{
    using namespace boost::python;

    class_<PyOneBitFlip<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("OneBitFlip", init<>())
    .def("__call__", &PyOneBitFlip<PyEOT>::operator ())
    ;

    class_<PyDetBitFlip<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("DetBitFlip", init<unsigned>())
    .def("__call__", &PyDetBitFlip<PyEOT>::operator ())
    ;

    class_<PyDetSingleBitFlip<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("DetSingleBitFlip", init<unsigned>())
    .def("__call__", &PyDetBitFlip<PyEOT>::operator ())
    ;

    class_<PyBitMutation<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("BitMutation", init<optional<const double,bool>>())
        .def("__call__", &PyBitMutation<PyEOT>::operator ())
    ;

    class_<Py1PtBitXover<PyEOT>, bases<eoQuadOp<PyEOT> > >
        ("OnePtBitCrossover",init<>())
        .def("__call__",&Py1PtBitXover<PyEOT>::operator())
    ;


}
