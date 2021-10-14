
#include "yack/apex/natural.hpp"
#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif
#include "yack/system/exception.hpp"
#include <cerrno>

#include <iostream>

namespace yack
{
    namespace apex
    {

        YACK_APN_BINARY_REP(natural operator/,{ YACK_APN_BINARY_IMPL(natural::div); })
        YACK_APN_UNARY_REP(natural & natural:: operator/=,{YACK_APN_UNARY_IMPL(/); } )

        natural  natural:: div(const handle &numerator, const handle &denominator)
        {

            //__________________________________________________________________
            //
            //
            // sanity check
            //
            //__________________________________________________________________
            if(denominator.is0()) throw libc::exception(EDOM,"apn division by 0");
            
#if defined(YACK_APEX_TRACKING)
            const uint64_t mark = wtime::ticks();
#endif

            //__________________________________________________________________
            //
            //
            // trivial cases
            //
            //__________________________________________________________________
            switch( scmp(numerator,denominator) )
            {
                case number::negative: return natural(0);
                case number::naught:   return natural(1);
                default:
                    if(denominator.is1())
                        return natural(numerator.entry,numerator.count);
                    else
                        break;
            }

            //__________________________________________________________________
            //
            // numerator>denominator
            // find (2^(p-1))*denominator <= numerator < (2^p)*denominator
            assert(numerator.count>=denominator.count);
            // TODO: use delta count to start from a better p
            //__________________________________________________________________


            size_t  p   = 1;
            natural qhi = exp2(p); // start fom 2
            while(true)
            {
                const handle  Qhi(qhi);
                const natural num = mul(Qhi,denominator);
                const handle  Num(num);
                switch( scmp(Num,numerator) )
                {
                    case negative: break;            // too small
                    case naught:   return qhi;       // early   return
                    case positive: goto BISECTION;   // generic case
                }
                ++p;
                qhi = exp2(p);
            }

        BISECTION:
            //__________________________________________________________________
            //
            //
            // qhi * denominator > numerator >= qlo * denominator
            //
            //__________________________________________________________________
            natural qlo = exp2(--p);

        CYCLE:
            {
                natural      q = qlo+qhi;
                const handle Qlo(qlo);
                const handle Q(q.shr());

                if(cmp(Q,Qlo)<=0)
                {
#if defined(YACK_APEX_TRACKING)
                    div_ticks += wtime::ticks() - mark;
                    ++div_count;
#endif
                    return q;
                }
                else
                {
                    const natural num = mul(Q,denominator);
                    const handle  Num(num);
                    switch( scmp(Num,numerator) )
                    {
                        case negative:  qlo.xch(q); break;  // move qlo up
                        case naught:     return q;          // early return
                        case positive:  qhi.xch(q); break;  // move qhi down
                    }
                }
                goto CYCLE;
            }



        }

    }

}

