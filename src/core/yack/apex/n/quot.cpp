#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>


namespace yack
{
    namespace apex
    {


        natural  natural:: quot(const handle &numerator, const handle &denominator, natural &rem)
        {

            //__________________________________________________________________
            //
            //
            // sanity check
            //
            //__________________________________________________________________
            rem = 0; // default
            if(denominator.is0()) throw libc::exception(EDOM,"apn quotient by 0");


            //__________________________________________________________________
            //
            //
            // trivial cases
            //
            //__________________________________________________________________
            switch( scmp(numerator,denominator) )
            {
                case negative: // numerator<denominator
                    rem = natural(numerator.entry,numerator.words);
                    return natural(0);

                case __zero__: // numerator==denominator
                    assert(0==rem);
                    return natural(1);

                default:
                    if(denominator.is1())
                    {
                        assert(0==rem);
                        return natural(numerator.entry,numerator.words);
                    }
                    else
                    {
                        break;
                    }
            }

            //__________________________________________________________________
            //
            // numerator>denominator
            // find (2^(p-1))*denominator <= numerator < (2^p)*denominator
            //__________________________________________________________________
            assert(numerator.bytes>=denominator.bytes);


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
                    case __zero__:   return qhi;       // early   return, multiple => rem=0
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
                    const natural qd = mul(Q,denominator);
                    const handle  QD(qd);
                    assert(cmp(numerator,QD)>=0);
                    rem = sub(numerator,QD);
                    return q;
                }
                else
                {
                    const natural num = mul(Q,denominator);
                    const handle  Num(num);
                    switch( scmp(Num,numerator) )
                    {
                        case negative:  qlo.xch(q); break;  // move qlo up
                        case __zero__:  return   q;         // early return, multiple => rem=0
                        case positive:  qhi.xch(q); break;  // move qhi down
                    }
                }
                goto CYCLE;
            }


        }

        natural natural:: quot(const natural &numerator, const natural &denominator, natural &rem)
        {
            const handle N(numerator);
            const handle D(denominator);
            return quot(N,D,rem);
        }

    }

}

