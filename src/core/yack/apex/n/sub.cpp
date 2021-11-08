#include "yack/apex/natural.hpp"
#include "yack/system/exception.hpp"
#include <cerrno>
#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif
namespace yack
{
    namespace apex
    {

        YACK_APN_BINARY_REP(natural operator-,{ YACK_APN_BINARY_IMPL(natural::sub); })
        YACK_APN_UNARY_REP(natural & natural:: operator-=,{YACK_APN_UNARY_IMPL(-); } )

        natural natural:: sub(const handle &l,
                              const handle &r)
        {

            static const char                   fn[] = "natural::sub: ";
            typedef signed_int<core_size>::type carry_t;
            static  const  carry_t              radix(word_base);

            //------------------------------------------------------------------
            //
            // initialize, lhs>=rhs
            //
            //------------------------------------------------------------------
            const size_t lnw = l.words;
            const size_t rnw = l.words;
            if(lnw<rnw)
            {
                throw libc::exception(EDOM,"%slhs<rhs [level-1]",fn);
            }

            assert(lnw>=rnw);
#if defined(YACK_APEX_TRACKING)
            const uint64_t mark = wtime::ticks();
#endif
            natural          ans(lnw,as_capacity);
            word_type       *s   = ans.word;
            const word_type *lhs = l.entry;
            const word_type *rhs = r.entry;

            //__________________________________________________________________
            //
            // firt loop: common words
            //__________________________________________________________________
            carry_t   carry = 0;
            for( size_t i=rnw; i>0; --i )
            {
                const carry_t a = *(lhs++); // L[i];
                const carry_t b = *(rhs++); // R[i];
                carry_t       d = (a-b)-carry;
                if( d < 0 )
                {
                    d    += radix;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<radix);
                *(s++) = word_type(d);
            }


            //__________________________________________________________________
            //
            // second loop: propagate carry
            //__________________________________________________________________
            for(size_t i=lnw-rnw;i>0;--i)
            {
                const carry_t a = *(lhs++);  //L[i];
                carry_t       d = a-carry;
                if( d < 0 )
                {
                    d    += radix;
                    carry = 1;
                }
                else
                {
                    carry = 0;
                }
                assert(d>=0);
                assert(d<radix);
                *(s++) = word_type(d);
            }

            if(carry!=0)
            {
                const natural L(l.entry,l.words);
                const natural R(r.entry,r.words);
                std::cerr << "*** Error: carry=" << carry << std::endl;
                std::cerr << "*** Error: lhs = ";  L.output_hex(std::cerr) << std::endl;
                std::cerr << "*** Error: lh  = ";  l.display(std::cerr) << std::endl;
                std::cerr << "*** Error: rhs = ";  R.output_hex(std::cerr) << std::endl;
                std::cerr << "*** Error: rh  = ";  r.display(std::cerr) << std::endl;
                throw libc::exception(EDOM,"%slhs<rhs [level-2]",fn);
            }

            ans.words=lnw;
            ans.update();
#if defined(YACK_APEX_TRACKING)
            sub_ticks += wtime::ticks() - mark;
            ++sub_count;
#endif
            YACK_APN_CHECK(ans,"sub");
            return ans;
        }

        natural & natural:: operator--()
        {
            const handle lhs(*this);
            word_type    one = 1;
            const handle rhs(one);
            natural      res = sub(lhs,rhs);
            xch(res);
            return *this;
        }

        natural  natural:: operator--(int)
        {
            const handle lhs(*this);
            word_type    one = 1;
            const handle rhs(one);
            natural      res = sub(lhs,rhs);
            xch(res);
            return res;
        }

    }

}
