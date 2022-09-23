#include <pyeot.h>
#include <eoOp.h>

#include <ga/eoBitOp.h>
#include <ga/eoStandardBitMutation.h>

#include <boost/python.hpp>
#include <boost/python/numpy.hpp>

#include <numeric>

namespace p  = boost::python;
namespace np = boost::python::numpy;



void
bit_op()
{
    using namespace boost::python;

    //-------------------eo/ga/eoStandardBitMutation.h-------------------
    //choose k from the binomial distribution Bin(n,p) and apply flip_k(x)
    class_<eoStandardBitMutation<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("StandardBitMutation", init<optional<double>>())
    .def("__call__", &eoStandardBitMutation<BinarySolution>::operator ())
    ;

    // Uniform bit mutation with mutation rate p:
    //   choose k from the uniform distribution U(0,n) and apply flip_k(x).
    class_<eoUniformBitMutation<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("UniformBitMutation", init<optional<double>>())
    .def("__call__", &eoUniformBitMutation<BinarySolution>::operator ())
    ;


    // Conditional standard bit mutation with mutation rate p:
    //  * choose k from the binomial distribution Bin(n,p) until k >0
    //  * and apply flip_k(x).
    //  *
    //  * This is identical to sampling k from the conditional binomial
    //  * distribution Bin>0(n,p) which re-assigns the probability to sample
    //  * a 0 proportionally to all values i ∈ [1..n].
    class_<eoConditionalBitMutation<BinarySolution>, bases<eoStandardBitMutation<BinarySolution> > >
        ("ConditionalBitMutation", init<optional<double>>())
    .def("__call__", &eoConditionalBitMutation<BinarySolution>::operator ())
    ;


    // Shifted standard bit mutation with mutation rate p:
    //  * choose k from the binomial distribution Bin(n,p).
    //  * When k= 0, set k= 1. Apply flip_k(x).
    //  *
    //  * This is identical to sampling k from the conditional binomial
    //  * distribution Bin0→1(n,p) which re-assigns the probability to
    //  * sample a 0 to sampling k= 1.
    class_<eoShiftedBitMutation<BinarySolution>, bases<eoStandardBitMutation<BinarySolution> > >
        ("ShiftedBitMutation", init<optional<double>>())
    .def("__call__", &eoShiftedBitMutation<BinarySolution>::operator ())
    ;

    // Mutation which size is sample in a gaussian.
    //  *
    //  * sample k from the normal distribution N(pn,σ^2)
    //  * and apply flip_k(x).
    //  *
    //  * From:
    //  * Furong Ye, Carola Doerr, and Thomas Back.
    //  * Interpolating local and global search by controllingthe variance of standard bit mutation.
    //  * In 2019 IEEE Congress on Evolutionary Computation(CEC), pages 2292–2299.
    //  *
    //  * In contrast to standard bit mutation, this operators allows to scale
    //  * the variance of the mutation strength independently of the mean.
    class_<eoNormalBitMutation<BinarySolution>, bases<eoStandardBitMutation<BinarySolution> > >
        ("NormalBitMutation", init<optional<double,double>>())
    .def("__call__", &eoNormalBitMutation<BinarySolution>::operator ())
    ;


    // Fast mutation which size is sampled from an adaptive power law.
    //  *
    //  * From:
    //  * Benjamin Doerr, Huu Phuoc Le, Régis Makhmara, and Ta Duy Nguyen.
    //  * Fast genetic algorithms.
    //  * In Proc. of Genetic and Evolutionary Computation Conference (GECCO’17), pages 777–784.ACM, 2017.
    class_<eoFastBitMutation<BinarySolution>, bases<eoStandardBitMutation<BinarySolution> > >
        ("FastBitMutation", init<optional<double,double>>())
    .def("__call__", &eoFastBitMutation<BinarySolution>::operator ())
    ;

    //-------------------eo/eoBitOp.h-------------------

    //choose one at random and flip
    class_<eoOneBitFlip<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("OneBitFlip", init<>())
    .def("__call__", &eoOneBitFlip<BinarySolution>::operator ())
    ;

    //flip k at random (no duplicate checking)
    class_<eoDetBitFlip<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("DetBitFlip", init<optional<const unsigned>>())
    .def("__call__", &eoDetBitFlip<BinarySolution>::operator ())
    ;

    //flip exactly k at random (no duplicates)
    class_<eoDetSingleBitFlip<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("DetSingleBitFlip", init<optional<const unsigned>>())
    .def("__call__", &eoDetSingleBitFlip<BinarySolution>::operator ())
    ;

    //at proba _prob flip bit (boolean triggers normalization of rate)
    class_<eoBitMutation<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("BitMutation", init<optional<const double,bool>>())
        .def("__call__", &eoBitMutation<BinarySolution>::operator ())
    ;

    //invert (reverse) a random range of bits
    class_<eoBitInversion<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("BitInversion", init<>())
        .def("__call__", &eoBitInversion<BinarySolution>::operator ())
    ;

    //next bitstring in lexicographic order
    class_<eoBitNext<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("BitNext", init<>())
        .def("__call__", &eoBitNext<BinarySolution>::operator ())
    ;

    //previous bitstring in lexicographic order
    class_<eoBitPrev<BinarySolution>, bases<eoMonOp<BinarySolution> > >
        ("BitPrevious", init<>())
        .def("__call__", &eoBitPrev<BinarySolution>::operator ())
    ;


    //CROSSOVER
    class_<eo1PtBitXover<BinarySolution>, bases<eoQuadOp<BinarySolution> > >
        ("OnePtBitCrossover",init<>())
        .def("__call__",&eo1PtBitXover<BinarySolution>::operator())
    ;

    class_<eoUBitXover<BinarySolution>, bases<eoQuadOp<BinarySolution> > >
        ("UBitXover",init<optional<double>>())
        .def("__call__",&eoUBitXover<BinarySolution>::operator())
    ;

    class_<eoNPtsBitXover<BinarySolution>, bases<eoQuadOp<BinarySolution> > >
        ("NPtsBitXover",init<optional<unsigned>>())
        .def("__call__",&eoNPtsBitXover<BinarySolution>::operator())
    ;

    class_<eoBitGxOver<BinarySolution>, bases<eoQuadOp<BinarySolution> > >
        ("BitGxOver",init<const unsigned,optional<const unsigned>>())
        .def("__call__",&eoBitGxOver<BinarySolution>::operator())
    ;
}
