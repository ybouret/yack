
#include "yack/apex/natural.hpp"
#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif
#include "yack/system/exception.hpp"
#include <cerrno>



namespace yack
{
    namespace apex
    {

        YACK_APN_BINARY_REP(natural operator/,{ YACK_APN_BINARY_IMPL(natural::div); })
        YACK_APN_UNARY_REP(natural & natural:: operator/=,{YACK_APN_UNARY_IMPL(/); } )


        size_t    natural::exp2_look_up(const handle &numerator, const handle &denominator)
        {
            assert(numerator.bytes>=denominator.bytes);
            size_t delta = numerator.bytes - denominator.bytes;
            if(delta<=0)
            {
                return 1;
            }
            else
            {
                --delta;
                delta <<= 3;
                return ++delta;
            }
        }

        natural  natural:: div(const handle &numerator, const handle &denominator)
        {

            //__________________________________________________________________
            //
            //
            // sanity check
            //
            //__________________________________________________________________
            if(denominator.is0()) throw libc::exception(EDOM,"apn division by 0");


            //__________________________________________________________________
            //
            //
            // trivial cases
            //
            //__________________________________________________________________
            switch( scmp(numerator,denominator) )
            {
                case negative: return natural(0);
                case __zero__: return natural(1);
                default:
                    if(denominator.is1())
                        return natural(numerator.entry,numerator.words);
                    else
                        break;
            }

            //__________________________________________________________________
            //
            // numerator>denominator
            // find (2^(p-1))*denominator <= numerator < (2^p)*denominator
            //__________________________________________________________________
            assert(numerator.bytes>=denominator.bytes);

#if defined(YACK_APEX_TRACKING)
            const uint64_t mark = wtime::ticks();
#endif

            size_t  p   = exp2_look_up(numerator,denominator);
            natural qhi = exp2(p);
            while(true)
            {
                const handle  Qhi(qhi);
                const natural num = mul(Qhi,denominator);
                const handle  Num(num);
                switch( scmp(Num,numerator) )
                {
                    case negative: break;            // too small
                    case __zero__: return qhi;       // early   return
                    case positive: goto BISECTION;   // generic case
                }
                ++p;
                qhi = exp2(p);
            }


        BISECTION:
            //ios::ocstream::echo("divp.dat","%u %u\n", unsigned(delta), unsigned(p));

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
                        case __zero__:  return   q;         // early return
                        case positive:  qhi.xch(q); break;  // move qhi down
                    }
                }
                goto CYCLE;
            }



        }

    }

}

