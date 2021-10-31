

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

        YACK_APN_BINARY_REP(natural operator%,{ YACK_APN_BINARY_IMPL(natural::mod); })
        YACK_APN_UNARY_REP(natural & natural:: operator%=,{YACK_APN_UNARY_IMPL(%); } )

        natural  natural:: mod(const handle &numerator, const handle &denominator)
        {

            //__________________________________________________________________
            //
            //
            // sanity check
            //
            //__________________________________________________________________
            if(denominator.is0()) throw libc::exception(EDOM,"apn modulo by 0");

            //__________________________________________________________________
            //
            //
            // trivial cases
            //
            //__________________________________________________________________
            switch( scmp(numerator,denominator) )
            {
                case negative: return natural(numerator.entry,numerator.words);
                case __zero__: return natural(0);
                default:
                    break;
            }

            //__________________________________________________________________
            //
            // numerator>denominator
            // find (2^(p-1))*denominator <= numerator < (2^p)*denominator
            //__________________________________________________________________
            assert(numerator.words>=denominator.words);

#if defined(YACK_APEX_TRACKING)
            const uint64_t mark = wtime::ticks();
#endif

            size_t  p   = exp2_look_up(numerator,denominator);
            natural qhi = exp2(p); // start fom 2
            while(true)
            {
                const handle  Qhi(qhi);
                const natural num = mul(Qhi,denominator);
                const handle  Num(num);
                switch( scmp(Num,numerator) )
                {
                    case negative: break;             // too small
                    case __zero__: return natural(0); // early return (multiple)
                    case positive: goto BISECTION;    // generic case
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
                    mod_ticks += wtime::ticks() - mark;
                    ++mod_count;
#endif
                    const natural num = mul(Q,denominator);
                    const handle  Num(num); assert( cmp(Num,numerator) <= 0);
                    return sub(numerator,Num);
                }
                else
                {
                    const natural num = mul(Q,denominator);
                    const handle  Num(num);
                    switch( scmp(Num,numerator) )
                    {
                        case negative:  qlo.xch(q); break;  // move qlo up
                        case __zero__:  return natural(0);  // early return
                        case positive:  qhi.xch(q); break;  // move qhi down
                    }
                }
                goto CYCLE;
            }

        }

    }

}
