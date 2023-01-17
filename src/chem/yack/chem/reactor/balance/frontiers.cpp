
#include "yack/chem/reactor/balance/frontiers.hpp"

namespace yack {

    namespace chemical
    {

        frontiers:: ~frontiers() throw() {}

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
            size_t i=size();

        SORT:
            const size_t im = i-1;
            if(im<=0) return;

            i = im;
            goto SORT;


        }

        
    }

}

