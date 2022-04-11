#include <pyeot.h>
#include <eoOp.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <numeric>

namespace p  = boost::python;
namespace np = boost::python::numpy;

// almost copies from eoBitOp.h .... not nice.
// PROBLEM: can't instantiate classes from eoRealOp unless EOT inherits from std::vector<double> (needs operator[], size(), ...).

template <class Chrom, typename T> class PyOneBitFlip : public eoMonOp<Chrom>
{
public:
    /// The class name.
    // virtual std::string className() const { return "OneBitFlip"; }

    /**
     * Change one bit.
     * @param chrom The cromosome which one bit is going to be changed.
     */
    bool
    operator () (Chrom& chrom)
    {
        // get ndarray from object
        np::ndarray enc = np::from_object(chrom.encoding, np::dtype::get_builtin<T>());
        // get ptr to ndarray data
        T* enc_ptr = reinterpret_cast<T*>(enc.get_data());
        // choose random flip site
        unsigned i = eo::rng.random(enc.shape(0));
        *(enc_ptr + i) = !(*(enc_ptr + i));
        return true;
    }
};

template <class Chrom> class PyDetBitFlip : public eoMonOp<Chrom>
{
public:

    /**
     * (Default) Constructor.
     * @param _num_bit The number of bits to change
     * default is one - equivalent to eoOneBitFlip then
     */
    PyDetBitFlip(const unsigned _num_bit = 1) : num_bit(_num_bit){ }

    /// The class name.
    // virtual std::string className() const { return "eoDetBitFlip"; }

    /**
     * Change num_bit bits.
     * @param chrom The cromosome which one bit is going to be changed.
     */
    bool
    operator () (Chrom& chrom)
    {
        // get ndarray from object
        np::ndarray enc = np::from_object(chrom.encoding, np::dtype::get_builtin<bool>());
        // get ptr to ndarray data
        bool * enc_ptr = reinterpret_cast<bool *>(enc.get_data());

        // for duplicate checking see eoDetSingleBitFlip
        for (unsigned k = 0; k < num_bit; k++) {
            unsigned i = eo::rng.random(enc.shape(0));
            *(enc_ptr + i) = !(*(enc_ptr + i));
        }
        return true;
    }

private:
    unsigned num_bit;
};



/** Changes exactly k bits with checking for duplicate
 * \ingroup bitstring
 */
template <class Chrom>
class PyDetSingleBitFlip : public eoMonOp<Chrom>
{
public:

    /**
     * (Default) Constructor.
     * @param _num_bit The number of bits to change
     * default is one - equivalent to eoOneBitFlip then
     */
    PyDetSingleBitFlip(const unsigned _num_bit = 1) : num_bit(_num_bit){ }

    /// The class name.
    // virtual std::string className() const { return "eoDetSingleBitFlip"; }

    /**
     * Change num_bit bits.
     * @param chrom The cromosome which one bit is going to be changed.
     */
    bool
    operator () (Chrom& chrom)
    {
        // np::dtype dt = np::dtype::get_builtin<bool>();

        // get ndarray from object
        np::ndarray chr = np::from_object(chrom.encoding, np::dtype::get_builtin<bool>());
        bool * e_ptr    = reinterpret_cast<bool *>(chr.get_data());

        unsigned _size = chr.shape(0);

        // All possible indices
        std::vector<unsigned> indices(_size);
        std::iota(std::begin(indices), std::end(indices), 0);

        // indices.reserve(_size);
        // for(unsigned i=0; i<_size; ++i) {
        //     indices.push_back(i);
        // }

        // Shuffle indices ... -> std::random_shuffle ?
        for (unsigned i = 0; i < indices.size(); ++i) {
            unsigned other = eo::rng.random(indices.size());
            std::swap(indices[i], indices[other]);
        }

        // Flip at first indices
        assert(num_bit <= chr.shape(0));
        for (unsigned i = 0; i < num_bit; ++i) {
            *(e_ptr + indices[i]) = !(*(e_ptr + indices[i]));
            // chrom[indices[i]] = !chrom[indices[i]];
        }

        if (num_bit > 0) {
            return true;
        } else {
            return false;
        }
    } // ()

    void
    number_bits(const unsigned _num_bit)
    {
        num_bit = _num_bit;
    }

protected:
    unsigned num_bit;
};


/** eoBitMutation --> classical mutation
 * \class eoBitMutation eoBitOp.h ga/eoBitOp.h
 * \ingroup bitstring
 */
template <class Chrom> class PyBitMutation : public eoMonOp<Chrom>
{
public:

    /**
     * (Default) Constructor.
     * @param _rate Rate of mutation.
     * @param _normalize use rate/chrom.size if true
     */
    PyBitMutation(const double _rate = 0.01, bool _normalize = false) :
        rate(_rate), normalize(_normalize){ }

    /// The class name.
    // virtual std::string className() const { return "eoBitMutation"; }

    /**
     * Mutate a chromosome.
     * @param chrom The chromosome to be mutated.
     */
    bool
    operator () (Chrom& chrom)
    {
        // get ndarray from object
        np::ndarray e = np::from_object(chrom.encoding, np::dtype::get_builtin<bool>());
        bool * e_ptr  = reinterpret_cast<bool *>(e.get_data());

        unsigned _size = e.shape(0);

        double actualRate      = (normalize ? rate / _size : rate);
        bool changed_something = false;
        for (unsigned i = 0; i < _size; i++)
            if (eo::rng.flip(actualRate)) {
                *(e_ptr) = !(*(e_ptr));
                // chrom[i] = !chrom[i];
                changed_something = true;
            }

        return changed_something;
    }

protected:
    double rate;
    bool normalize; // divide rate by chromSize
};




template <class Chrom> class PyBitInversion : public eoMonOp<Chrom>
{
public:
    /// The class name.
    // virtual std::string className() const { return "eoBitInversion"; }

    /**
     * Inverts a range of bits in a binary chromosome.
     * @param chrom The chromosome whos bits are going to be inverted (a range).
     */
    bool
    operator () (Chrom& chrom)
    {
        np::ndarray e = np::from_object(chrom.encoding, np::dtype::get_builtin<bool>());
        bool * e_ptr  = reinterpret_cast<bool *>(e.get_data());

        unsigned _size = e.shape(0);

        unsigned u1 = eo::rng.random(_size + 1);
        unsigned u2;
        do u2 = eo::rng.random(_size + 1); while (u1 == u2);
        unsigned r1 = std::min(u1, u2);
        unsigned r2 = std::max(u1, u2);

        r2--;
        // std::reverse(chrom.begin() + r1, chrom.begin() + r2);
        while (r1 < r2) {
            int tmp = e_ptr[r1];
            e_ptr[r1++] = e_ptr[r2];
            e_ptr[r2--] = tmp;
        }
        return true;
    }
};


/** eo1PtBitXover --> classic 1-point crossover
\class eo1PtBitCrossover eoBitOp.h ga/eoBitOp.h
\ingroup bitstring
*/
template <class Chrom> class Py1PtBitXover : public eoQuadOp<Chrom>
{
public:
    /// The class name.
    // virtual std::string className() const { return "eo1PtBitXover"; }

    /**
     * 1-point crossover for binary chromosomes.
     * @param chrom1 The first chromosome.
     * @param chrom2 The first chromosome.
     */
    bool
    operator () (Chrom& chrom1, Chrom& chrom2)
    {
        np::dtype dt = np::dtype::get_builtin<bool>();

        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom1.encoding, dt);
        np::ndarray e2 = np::from_object(chrom2.encoding, dt);

        bool* arr1 = reinterpret_cast<bool *>(e1.get_data());
        bool* arr2 = reinterpret_cast<bool *>(e2.get_data());

        std::vector<bool> ind1(arr1, arr1 + e1.shape(0));
        std::vector<bool> ind2(arr2, arr2 + e2.shape(0));

        // longueur différente ????
        unsigned site = eo::rng.random(std::min(e1.shape(0), e2.shape(0)));

        if (!std::equal(ind1.begin(), ind1.begin() + site, ind2.begin())) {
            std::swap_ranges(ind1.begin(), ind1.begin() + site, ind2.begin());

            std::copy(ind1.begin(), ind1.end(), arr1);
            std::copy(ind2.begin(), ind2.end(), arr2);

            return true;
        }

        return false;
    }
};



/** eoUBitXover --> classic Uniform crossover
\class eoUBitXover eoBitOp.h ga/eoBitOp.h
\ingroup bitstring
*/
template <class Chrom> class PyUBitXover : public eoQuadOp<Chrom>
{
public:
    /// (Default) Constructor.
    PyUBitXover(double _preference = 0.5) : preference(_preference)
    {
        if ( (_preference <= 0.0) || (_preference >= 1.0) )
            std::runtime_error("UxOver --> invalid preference");
    }

    /// The class name.
    // virtual std::string className() const { return "eoUBitXover"; }

    /**
     * Uniform crossover for binary chromosomes.
     * @param chrom1 The first chromosome.
     * @param chrom2 The first chromosome.
     *    std::runtime_error if sizes don't match
     */
    bool
    operator () (Chrom& chrom1, Chrom& chrom2)
    {
        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom1.encoding, np::dtype::get_builtin<bool>());
        np::ndarray e2 = np::from_object(chrom2.encoding, np::dtype::get_builtin<bool>());

        std::vector<bool> ind1(reinterpret_cast<bool *>(e1.get_data()),
          reinterpret_cast<bool *>(e1.get_data()) + e1.shape(0));
        std::vector<bool> ind2(reinterpret_cast<bool *>(e2.get_data()),
          reinterpret_cast<bool *>(e2.get_data()) + e2.shape(0));

        if (ind1.size() != ind2.size())
            std::runtime_error("UxOver --> chromosomes sizes don't match");

        bool changed = false;
        for (unsigned int i = 0; i < ind1.size(); i++) {
            if (ind1[i] != ind2[i] && eo::rng.flip(preference)) {
                std::swap(ind1[i], ind2[i]);
                changed = true;
            }
        }
        if (changed) {
            std::copy(ind1.begin(), ind1.end(), reinterpret_cast<bool*>(e1.get_data()));
            std::copy(ind2.begin(), ind2.end(), reinterpret_cast<bool*>(e2.get_data()));
        }

        return changed;
    }

protected:
    double preference;
};





/** eoNPtsBitXover --> n-point crossover
 * \class eoNPtsBitXover eoBitOp.h ga/eoBitOp.h
 * \ingroup bitstring
 */
template <class Chrom> class PyNPtsBitXover : public eoQuadOp<Chrom>
{
public:

    /** (Default) Constructor. */
    PyNPtsBitXover(unsigned _num_points = 2) : num_points(_num_points)
    {
        if (num_points < 1)
            std::runtime_error("NxOver --> invalid number of points");
    }

    /** The class name */
    // virtual std::string className() const { return "eoNPtsBitXover"; }

    /** n-point crossover for binary chromosomes.
     *
     * @param chrom1 The first chromosome.
     * @param chrom2 The first chromosome.
     */
    bool
    operator () (Chrom& chrom1, Chrom& chrom2)
    {
        // get ndarray from object
        np::ndarray e1 = np::from_object(chrom1.encoding, np::dtype::get_builtin<bool>());
        np::ndarray e2 = np::from_object(chrom2.encoding, np::dtype::get_builtin<bool>());

        std::vector<bool> ind1(reinterpret_cast<bool*>(e1.get_data()),
          reinterpret_cast<bool*>(e1.get_data()) + e1.shape(0));
        std::vector<bool> ind2(reinterpret_cast<bool*>(e2.get_data()),
          reinterpret_cast<bool*>(e2.get_data()) + e2.shape(0));

        unsigned max_size(std::min(ind1.size(), ind2.size()));
        unsigned max_points(std::min(max_size - 1, num_points));
        std::vector<bool> points(max_size, false);

        // select ranges of bits to swap
        do {
            unsigned bit(eo::rng.random(max_size));
            if (points[bit]) {
                continue;
            } else {
                points[bit] = true;
                --max_points;
            }
        } while (max_points);

        // swap bits between chromosomes
        bool change(false);
        for (unsigned bit = 1; bit < points.size(); bit++) {
            if (points[bit])
                change = !change;
            if (change) {
                std::swap(ind1[bit], ind2[bit]);
            }
        }

        std::copy(ind1.begin(), ind1.end(), reinterpret_cast<bool*>(e1.get_data()));
        std::copy(ind2.begin(), ind2.end(), reinterpret_cast<bool*>(e2.get_data()));

        return true;
    } // ()

private:

    /** @todo Document this data member */
    unsigned num_points;
};




void
bit_op()
{
    using namespace boost::python;

    //MUTATION
    class_<PyOneBitFlip<PyEOT,bool>, bases<eoMonOp<PyEOT> > >
        ("OneBitFlip", init<>())
    .def("__call__", &PyOneBitFlip<PyEOT,bool>::operator ())
    ;

    class_<PyDetBitFlip<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("DetBitFlip", init<optional<const unsigned>>())
    .def("__call__", &PyDetBitFlip<PyEOT>::operator ())
    ;

    class_<PyDetSingleBitFlip<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("DetSingleBitFlip", init<optional<const unsigned>>())
    .def("__call__", &PyDetSingleBitFlip<PyEOT>::operator ())
    ;

    class_<PyBitMutation<PyEOT>, bases<eoMonOp<PyEOT> > >
        ("BitMutation", init<optional<const double,bool>>())
        .def("__call__", &PyBitMutation<PyEOT>::operator ())
    ;

    //TODO
    //eoBitInversion
    //eoBitNext
    //eoBitPrev


    //CROSSOVER
    class_<Py1PtBitXover<PyEOT>, bases<eoQuadOp<PyEOT> > >
        ("OnePtBitCrossover",init<>())
        .def("__call__",&Py1PtBitXover<PyEOT>::operator())
    ;

    class_<PyUBitXover<PyEOT>, bases<eoQuadOp<PyEOT> > >
        ("UBitCrossover",init<optional<double>>())
        .def("__call__",&PyUBitXover<PyEOT>::operator())
    ;

    class_<PyNPtsBitXover<PyEOT>, bases<eoQuadOp<PyEOT> > >
        ("NPtsBitCrossover",init<optional<unsigned>>())
        .def("__call__",&PyNPtsBitXover<PyEOT>::operator())
    ;


}
