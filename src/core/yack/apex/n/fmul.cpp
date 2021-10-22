

#include "yack/apex/natural.hpp"
#include "yack/apex/m/archon.hpp"
#include "yack/type/complex.hpp"
#include "yack/fft/fft1d.hpp"
#include "yack/system/exception.hpp"
#include "yack/arith/align.hpp"

#include <cmath>

#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif

namespace yack
{
    namespace apex
    {

        typedef complex<double> cplx_t;


        template <typename WORD>
        cplx_t *load_re(cplx_t *target, const WORD word) throw();
        
        template <>   inline
        cplx_t *load_re<uint8_t>(cplx_t *target, const uint8_t word) throw()
        {
            target[0].re = word; return target;
        }

        template <>   inline
        cplx_t *load_re<uint16_t>(cplx_t *target, const uint16_t word) throw()
        {
            target[0].re = (  word     & 0xff);
            target[1].re = ( (word>>8) & 0xff);
            return target+2;
        }

        template <>   inline
        cplx_t *load_re<uint32_t>(cplx_t *target, const uint32_t word) throw()
        {
            target[0].re = (  word      & 0xff);
            target[1].re = ( (word>>8)  & 0xff);
            target[2].re = ( (word>>16) & 0xff);
            target[3].re = ( (word>>24) & 0xff);
            return target+4;
        }

        template <typename WORD>
        cplx_t *load_im(cplx_t *target, const WORD word) throw();

        template <>   inline
        cplx_t *load_im<uint8_t>(cplx_t *target, const uint8_t word) throw()
        {
            target[0].im = word; return target;
        }

        template <>   inline
        cplx_t *load_im<uint16_t>(cplx_t *target, const uint16_t word) throw()
        {
            target[0].im = (  word     & 0xff);
            target[1].im = ( (word>>8) & 0xff);
            return target+2;
        }

        template <>   inline
        cplx_t *load_im<uint32_t>(cplx_t *target, const uint32_t word) throw()
        {
            target[0].im = (  word      & 0xff);
            target[1].im = ( (word>>8)  & 0xff);
            target[2].im = ( (word>>16) & 0xff);
            target[3].im = ( (word>>24) & 0xff);
            return target+4;
        }



        struct apn_to
        {

            static inline
            void cpx(cplx_t                   *target,
                     const natural::word_type *source,
                     const size_t              words)
            {
                for(size_t i=words;i>0;--i)
                {
                    target = load_re(target,*(source++));
                }
            }

            static inline
            void re(cplx_t                   *target,
                    const natural::word_type *source,
                    const size_t              words)
            {
                for(size_t i=words;i>0;--i)
                {
                    target = load_re(target,*(source++));
                }
            }

            static inline
            void im(cplx_t                   *target,
                    const natural::word_type *source,
                    const size_t              words)
            {
                for(size_t i=words;i>0;--i)
                {
                    target = load_im(target,*(source++));
                }
            }

        };

        

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
        

        natural natural:: fmul(const handle &l,
                               const handle &r)
        {
            assert(l.entry);
            assert(r.entry);

            const size_t     lnw = l.words;
            if(lnw>0)
            {
                const size_t rnw = r.words;
                if(rnw>0)
                {
#if                 defined(YACK_APEX_TRACKING)
                    const uint64_t mark = wtime::ticks();
#endif
                    const size_t     pnb = l.bytes+r.bytes; assert(pnb>0);              // product num bytes
                    const size_t     pnw = (YACK_ALIGN_TO(word_type,pnb)) >> word_exp2; // product nuw words
                    natural          res(pnw,as_capacity);       // product
                    const size_t     exp2 = res.max_bytes_exp2;  // shared max bytes exp2
                    const size_t     size = res.max_bytes;       // working bytes
                    archon::tableau<cplx_t>  com(exp2+1);
                    assert(com.size>=2*size);
                    assert(com.bytes()>=2*size*sizeof(cplx_t));
                    cplx_t          *L = &com[0];
                    cplx_t          *R = &com[size];

                    double *data = &(L->re) - 1;
                    
#if 0
                    // using dual fft
                    apn_to::cpx(L,l.entry,lnw); //std::cerr << "L="; display(L,size);
                    fft1d::forward(data, size);
                    apn_to::cpx(R,r.entry,rnw); //std::cerr << "R="; display(R,size);
                    fft1d::forward(&(R->re)-1, size);
#else
                    
                    // using compact/expand
                    apn_to::re(L,l.entry,lnw);           // compact data
                    apn_to::im(L,r.entry,rnw);           // compact data
                    fft1d::forward(data,size);           // fft
                    fft1d::expand(data,&(R->re)-1,size); // recompose
#endif
                    L[0].re *= R[0].re;
                    {
                        size_t i=size;
                        while(--i>0)
                        {
                            L[i] *= R[i];
                        }
                    }

                    fft1d::reverse(data,size);
                    finalize((uint8_t *)res.word,pnb,L,size);
                    
                    res.words = pnw;
                    res.update();
#if                 defined(YACK_APEX_TRACKING)
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

        natural  natural:: fsqr(const handle &l)
        {

            if(l.words)
            {
#if             defined(YACK_APEX_TRACKING)
                const uint64_t mark = wtime::ticks();
#endif
                const size_t             pnb = 2*l.bytes; assert(pnb>0);                    // product num bytes
                const size_t             pnw = (YACK_ALIGN_TO(word_type,pnb)) >> word_exp2; // product nuw words
                natural                  res(pnw,as_capacity);       // product
                const size_t             exp2 = res.max_bytes_exp2;  // shared max bytes exp2
                const size_t             size = res.max_bytes;       // working bytes
                archon::tableau<cplx_t>  com(exp2);
                assert(com.size>=size);
                cplx_t          *L = &com[0];

                double *data = &(L->re) - 1;

                // using compact/expand
                apn_to::re(L,l.entry,l.words);       // compact data
                fft1d::forward(data,size);           // fft

                L[0].re *= L[0].re;
                {
                    size_t i=size;
                    while(--i>0)
                    {
                        L[i] *= L[i];
                    }
                }

                fft1d::reverse(data,size);
                finalize((uint8_t *)res.word,pnb,L,size);

                res.words = pnw;
                res.update();
#if                 defined(YACK_APEX_TRACKING)
                fsqr_ticks += wtime::ticks() - mark;
                ++fsqr_count;
#endif
                return res;
            }
            else
            {
                return 0;
            }
        }


    }
}
