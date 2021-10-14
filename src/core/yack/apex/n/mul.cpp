
#include "yack/apex/natural.hpp"
#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif
namespace yack
{
    namespace apex
    {

        // YACK_APN_BINARY_REP(natural operator+,{ YACK_APN_BINARY_IMPL(natural::add); })
        //YACK_APN_UNARY_REP(natural & natural:: operator+=,{YACK_APN_UNARY_IMPL(+); } )


        natural natural:: lmul(const handle &l,
                               const handle &r)
        {
            assert(l.w);
            assert(r.w);

            const size_t     lnw = l.n;
            if(lnw>0)
            {
                const size_t rnw = r.n;
                if(rnw>0)
                {
                    const word_type *lhs = l.w;
                    const word_type *rhs = r.w;
                    const size_t     pnw = lnw+rnw;          // product nuw words
                    natural          res(pnw,as_capacity);   // product 

                    {
                        word_type   *ptr = res.word;
                        word_type   *tgt = ptr+rnw;
                        for(size_t i=0;i<lnw;++i)
                        {
                            const core_type  L     = *(lhs++);
                            core_type        carry = 0;
                            word_type       *sub   = ptr+i;
                            const word_type *r     = rhs;
                            for(size_t j=rnw;j>0;--j)
                            {
                                word_type        &here = *(sub++);
                                const core_type   temp = core_type(here) + carry + L * core_type(*(r++));
                                here  = word_type(temp&word_maxi);
                                carry = temp >> word_bits;
                            }
                            *(tgt++) = word_type(carry);
                        }

                    }

                    res.words = pnw;
                    res.update();
                    return res;
                }
                else
                {
                    return natural(0);
                }
            }
            else
            {
                return natural(0);
            }
        }

    }

}
