

#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>

namespace yack
{
    namespace apex
    {
        natural  natural:: mod_inv( const natural &b, const natural &n )
        {
            natural n0 = n;
            natural b0 = b;
            natural t0 = 0;
            natural t  = 1;
            natural q  = n0/b0;
            natural r  = n0 - q*b0;

            while( r>0 )
            {
                const natural lhs = t0;
                const natural rhs = q*t;

                t0 = t;

                if( compare(lhs,rhs) >= 0 )
                {
                    assert(lhs>=rhs);
                    t = ( (lhs-rhs) % n );
                    assert(t<n);
                }
                else
                {
                    assert(rhs>lhs);
                    t  = n - ( (rhs-lhs)%n );
                    assert(t<=n);
                }

                n0 = b0;
                b0 = r;
                q  = n0/b0;
                r  = n0 - q * b0;
            }

            // normally an error if b0 != 1
            return t;
        }

        natural natural:: mod_exp( const natural &B, const natural &E, const natural &N )
        {
            static const uint8_t bits_table[8] = { 1, 2, 4, 8, 16, 32, 64, 128 };

            const handle NN(N); if(NN.is0()) throw libc::exception( EDOM, "natural::mod_exp %% 0");
            natural      result = 1;

            if(E.bytes>0)
            {
                assert(E.bits()>0);
                natural        base  = B;
                const size_t   nbit  = E.bits()-1;

                for(size_t i=0;i<nbit;++i)
                {
                    size_t j = i>>3;
                    if( E[++j] & bits_table[ (i&0x7) ] )
                    {
                        //______________________________________________________
                        //
                        // result = ( result * base ) % N;
                        //______________________________________________________
                        const natural rb   = result * base; const handle h(rb);
                        natural       tmp1 = mod(h,NN);
                        tmp1.xch( result );
                    }

                    //__________________________________________________________
                    //
                    // base = ( base * base ) % N;
                    //__________________________________________________________
                    {
                        const natural bsq  = squared(base); const handle h(bsq);
                        natural       tmp2 = mod(h,NN);
                        base.xch(tmp2);
                    }
                }

                //__________________________________________________________
                //
                //-- most significant bit !
                //__________________________________________________________
                assert( (E[1+(nbit>>3)] & bits_table[ (nbit&7) ]) != 0 );

                //__________________________________________________________
                //
                // result = ( result * base ) % N;
                //__________________________________________________________
                {
                    const natural rb   = result * base; const handle h(rb);
                    natural       tmp3 = mod(h,NN); 
                    tmp3.xch( result );
                }
            }

            return result;
        }
    }

}

