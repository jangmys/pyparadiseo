#ifndef EO_NPTS_CROSSOVER
#define EO_NPTS_CROSSOVER

#include <algorithm>
#include <utils/eoRealVectorBounds.h>

template<class EOT> class eoTwoPtCrossover : public eoQuadOp<EOT>
{
public:
    /** (Default) Constructor. */
    eoTwoPtCrossover(){}

    /** The class name */
    virtual std::string className() const { return "eoNPtsCrossover"; }

    /** n-point crossover
    @param chrom1 The first chromosome.
    @param chrom2 The first chromosome.
    */
    bool operator()(EOT& _eo1, EOT& _eo2) {
        unsigned max_size(std::min(_eo1.size(), _eo2.size()));

        std::vector<bool> points(max_size, false);

        unsigned pt1 = eo::rng.random(max_size+1);
        unsigned pt2 = eo::rng.random(max_size+1);
        do{
            pt2 = eo::rng.random(max_size);
        }while(pt1==pt2);

        if(pt1>pt2)std::swap(pt1,pt2);

        std::swap_ranges(_eo1.begin()+pt1,_eo1.begin()+pt2,_eo2.begin()+pt1);

        return true;
    }
};

#endif
