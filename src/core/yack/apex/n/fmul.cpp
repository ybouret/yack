

#include "yack/apex/natural.hpp"
#include "yack/apex/n/alloc.hpp"
#include "yack/type/complex.hpp"
#include "yack/fft/fft1d.hpp"
#include "yack/system/exception.hpp"

#include <cmath>

#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif

namespace yack
{
    namespace apex
    {

        typedef complex<double> cplx_t;


        template <typename WORD>   cplx_t *transfer(cplx_t *target, const WORD word) throw();

        template <>   inline
        cplx_t *transfer<uint8_t>(cplx_t *target, const uint8_t word) throw()
        {
            target[0].re = word; return target;
        }

        template <>   inline
        cplx_t *transfer<uint16_t>(cplx_t *target, const uint16_t word) throw()
        {
            target[0].re = (  word     & 0xff);
            target[1].re = ( (word>>8) & 0xff);
            return target+2;
        }

        template <>   inline
        cplx_t *transfer<uint32_t>(cplx_t *target, const uint32_t word) throw()
        {
            target[0].re = (  word      & 0xff);
            target[1].re = ( (word>>8)  & 0xff);
            target[2].re = ( (word>>16) & 0xff);
            target[3].re = ( (word>>24) & 0xff);
            return target+4;
        }




        static inline
        void apn2cpx(cplx_t                   *target,
                     const natural::word_type *source,
                     const size_t              words)
        {
            for(size_t i=words;i>0;--i)
            {
                target = transfer(target,*(source++));
            }
        }

        static inline
        void finalize(uint8_t      *prod,
                      const size_t  size,
                      const cplx_t *L,
                      const size_t  nn) throw()
        {
            const double fac   = 1.0/nn;
            double       carry = 0.0;
            const size_t top   = size - 1;
            for(size_t i=0;i<top;++i)
            {
                const double q = floor( (carry += (L[i].re*fac + 0.5) ) * 0.00390625 );
                const double r = carry - (256.0 * q);
                prod[i]        = uint8_t(r);
                carry          = q;
            }
            prod[top] = uint8_t(carry);
        }

#if 0
        static inline
        void display(const cplx_t *z, const size_t n)
        {
            std::cerr << "[";
            for(size_t i=0;i<n;++i) std::cerr << ' ' << z[i];
            std::cerr << " ]" <<std::endl;
        }
#endif

        natural natural:: fmul(const handle &l,
                               const handle &r)
        {
            assert(l.entry);
            assert(r.entry);

            const size_t     lnw = l.count;
            if(lnw>0)
            {
                const size_t rnw = r.count;
                if(rnw>0)
                {
#if defined(YACK_APEX_TRACKING)
                    const uint64_t mark = wtime::ticks();
#endif
                    const size_t     pnw = lnw+rnw;              // product nuw words
                    natural          res(pnw,as_capacity);       // product
                    const size_t     exp2 = res.max_bytes_exp2;  // shared max bytes exp2
                    const size_t     size = res.max_bytes;       // working bytes
                    tableau<cplx_t>  com(exp2+1); assert(com.size==2*size);
                    cplx_t          *L = &com[0];
                    cplx_t          *R = &com[size];

                    double *data = &(L->re) - 1;
                    apn2cpx(L,l.entry,lnw);
                    fft1d::forward( data, size);
                    apn2cpx(R,r.entry,rnw);
                    fft1d::forward( &(R->re)-1, size);


                    for(size_t i=0;i<size;++i)
                    {
                        L[i] *= R[i];
                    }
                    fft1d::reverse(data,size);
                    finalize((uint8_t *)res.word,pnw*sizeof(word_type),L,size);
                    
                    res.words = pnw;
                    res.update();
#if defined(YACK_APEX_TRACKING)
                    fmul_ticks += wtime::ticks() - mark;
                    ++fmul_count;
#endif
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
