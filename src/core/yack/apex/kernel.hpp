
//! \file

#ifndef YACK_APEX_KERNEL_INCLUDED
#define YACK_APEX_KERNEL_INCLUDED 1

#include "yack/apex.hpp"
#include "yack/container/matrix.hpp"
#include "yack/sequence/vector.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! apex kernel
    //
    //__________________________________________________________________________
    struct apk
    {

        //! find least common multiple of an array of rationals
        template <typename ARRAY, typename INDEX> static inline
        apn lcm(ARRAY &arr, const INDEX lo, const INDEX hi)
        {
            assert(lo<=hi);
            if(lo>=hi)
            {
                return arr[lo].den;
            }
            else
            {
                apn res = apn::lcm(arr[lo].den,arr[lo+1].den);
                for(INDEX i=lo+2;i<=hi;++i)
                {
                    res = apn::lcm(res,arr[i].den);
                }
                return res;
            }
        }

        //! find lcm for a sequence of rationals
        template <typename SEQUENCE> static inline
        apn lcm(SEQUENCE &seq)
        {
            return lcm<SEQUENCE,size_t>(seq,1,seq.size());
        }

        //! find lcm for a 1D field
        template <typename FIELD> static inline
        apn lcm1D(const FIELD &F)
        {
            return lcm<FIELD,unit_t>(F,F.lower,F.upper);
        }

        //! find lcm for a 2D field/matrix or rationals
        template <typename FIELD,typename INDEX> static inline
        apn lcm(const FIELD &F, const INDEX xlo, const INDEX xhi,
                const INDEX ylo, const INDEX yhi)
        {
            assert(xhi>=xlo);
            assert(yhi>=ylo);
            INDEX  y   = yhi;
            apn    res = lcm(F[y],xlo,xhi);
            for(--y;y>=ylo;--y)
            {
                const apn tmp = lcm(F[y],xlo,xhi);
                res = apn::lcm(res,tmp);
            }
            return res;
        }


        //! find lcm for a 2D field of rationals
        template <typename FIELD> static inline
        apn lcm2D(const FIELD &F)
        {
            return lcm<FIELD,unit_t>(F,F.lower.x,F.upper.x,F.lower.y,F.upper.y);
        }

        //! find lcm for a matrix
        static apn lcm(const matrix<apq> &);

        //! find proportionality coefficient
        template <typename LHS, typename RHS> static inline
        bool are_prop(const LHS &lhs, const RHS &rhs, apq *fac)
        {
            // initialize
            assert(lhs.size()==rhs.size());
            size_t       n = lhs.size();
            vector<apq>  f(2,as_capacity);
            if(fac) (*fac) = 0;

            // pair-wise loop
            for(size_t i=n;i>0;--i)
            {
                const apz L( lhs[i] );
                const apz R( rhs[i] );

                switch(__sign::pair_of(L,R))
                {
                    case zz_pair:
                        // 0/0 => compatible
                        break;

                    case nz_pair:
                    case pz_pair:
                    case zp_pair:
                    case zn_pair:
                        return false; // => not compatible

                    case pp_pair:
                    case nn_pair: { const apq q(L.n,R.n);    f << q; } break;

                    case pn_pair:
                    case np_pair: { apq q(L.n,R.n); q.neg(); f << q; } break;
                }

                switch(f.size())
                {
                    case 0:
                    case 1:
                        break;

                    case 2:
                        if(f[1]!=f[2])
                        {
                            return false;
                        }
                        f.pop_back();
                        break;
                }


            }

            if(fac && f.size() > 0 )
            {
                // coefficient
                assert(1==f.size());
                fac->xch(f.back());
            }


            return true;
        }


    };

}

#endif
