
#include "yack/chem/reactor/balance/frontiers.hpp"

namespace yack {

    namespace chemical
    {

        frontiers:: ~frontiers() noexcept {}

        frontiers:: frontiers(const size_t m) :
        base_type(m) {}


        static inline
        void grow(frontiers     &self,
                  const double   xi,
                  const species &sp,
                  const sp_fund &fn)
        {
            frontier & f = self.add<const sp_fund>(fn);
            f.xi = xi;
            f << sp;
        }


        
        void frontiers:: operator()(const double   xi,
                                    const species &sp,
                                    const sp_fund &fn)
        {
            grow(*this,xi,sp,fn);
            writable<frontier> &self = *this;
            size_t i=size();
        SORT:
            const size_t im = i-1;
            if(im<=0) return;
            frontier &lhs = self[im];
            frontier &rhs = self[i];
            switch( __sign::of(lhs.xi,rhs.xi) )
            {
                    //----------------------------------------------------------
                    // ordered!
                    //----------------------------------------------------------
                case negative:
                    assert(are_increasing());
                    return;

                    //----------------------------------------------------------
                    // exchange an try again
                    //----------------------------------------------------------
                case positive:
                    mswap(lhs,rhs); assert(lhs.xi<rhs.xi);
                    break;

                    //----------------------------------------------------------
                    // same numerical value
                    //----------------------------------------------------------
                case __zero__:
                    lhs.merge_back(rhs);      // |_steal species
                    suppress(i);              // |_compress
                    assert(are_increasing()); // |_debug
                    return;                   // |_done
            }

            i = im;
            goto SORT;


        }


        bool frontiers:: are_increasing() const noexcept
        {
            const size_t n = size();
            switch(n)
            {
                case 0:
                case 1:
                    return true;
                default:
                    break;
            }

            const readable<frontier> &self = *this;
            for(size_t i=1;i<n;++i)
            {
                if(self[i].xi>=self[i+1].xi) return false;
            }
            return true;

        }
        
    }

}

