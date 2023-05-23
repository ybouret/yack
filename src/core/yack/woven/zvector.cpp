
#include "yack/woven/zvector.hpp"

namespace yack
{
    namespace woven
    {

        zvector:: zvector(const size_t dims) :
        spot_object(),
        metrics(dims),
        ztableau(dims),
        next(0),
        prev(0),
        ncoeff(0),
        weight(0)
        {
        }

        zvector:: ~zvector() noexcept
        {
        }


        zvector:: zvector(const zvector &other) :
        collection(),
        spot_object(),
        metrics(other),
        ztableau(other),
        next(0),
        prev(0),
        ncoeff(other.ncoeff),
        weight(other.weight)
        {
        }

        int zvector:: compare(const zvector *lhs, const zvector *rhs) noexcept
        {
            assert(lhs->size()==rhs->size());
            switch( __sign::of(lhs->weight,rhs->weight) )
            {
                case negative: return -1;
                case positive: return  1;
                case __zero__:
                    break;
            }
            return comparison::lexicographic(&(*lhs)[1], &(*rhs)[1], rhs->size());
        }

        void zvector:: finalize()
        {
            const readable<apz> &Z = *this;
            ncoeff = 0;
            weight = 0;
            for(size_t i=dimensions;i>0;--i)
            {
                const apz &z = Z[i];
                switch(z.s)
                {
                    case __zero__: continue;
                    case positive:
                    case negative:
                        ++ncoeff;
                        weight += z.n;
                        continue;
                }
            }
        }

    }

}

