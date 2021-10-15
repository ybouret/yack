#include "yack/apex/natural.hpp"
#if defined(YACK_APEX_TRACKING)
#include "yack/system/wtime.hpp"
#endif
namespace yack
{
    namespace apex
    {

        YACK_APN_BINARY_REP(natural operator+,{ YACK_APN_BINARY_IMPL(natural::add); })
        YACK_APN_UNARY_REP(natural & natural:: operator+=,{YACK_APN_UNARY_IMPL(+); } )


        natural natural:: add(const handle &l,
                              const handle &r)  
        {
            // sanity check
            const word_type *lhs = l.entry; assert(lhs);
            const word_type *rhs = r.entry; assert(rhs);
            size_t           nl  = l.count;
            size_t           nr  = r.count;
            
            // order to have left=small one
            if(nl>nr)
            {
                cswap(nl,nr);
                cswap(lhs,rhs);
            }
            assert(nl<=nr);

            // check cases
            if(nr>0)
            {
                // big is not 0
                if(nl>0)
                {
#if defined(YACK_APEX_TRACKING)
                    const uint64_t mark = wtime::ticks();
#endif
                    // initialize
                    const size_t ns=nr+1;
                    natural      ans(ns,as_capacity);
                    word_type   *sum = ans.word;

                    // dual sum
                    core_type carry = 0;
                    for(size_t i=nl;i>0;--i)
                    {
                        carry += *(lhs++);
                        carry += *(rhs++);
                        *(sum++) = word_type(carry);
                        carry >>= word_bits;
                    }

                    // propagate carry
                    for(size_t i=nr-nl;i>0;--i)
                    {
                        carry += *(rhs++);
                        *(sum++) = word_type(carry);
                        carry >>= word_bits;
                    }
                    *sum = word_type(carry);


                    // update
                    ans.words = ns;
                    ans.bytes = ns << word_exp2;
                    ans.update();
#if defined(YACK_APEX_TRACKING)
                    add_ticks += wtime::ticks() - mark;
                    ++add_count;
#endif
                    return ans;
                }
                else
                {
                    return natural(rhs,nr);
                }
            }
            else
            {
                // small is 0 as well
                assert(nr<=0);
                assert(nl<=0);
                return natural(0);
            }
        }


        natural natural:: operator+() const
        {
            return natural(*this);
        }

        natural & natural:: operator++()
        {
            const handle lhs(*this);
            word_type    one = 1;
            const handle rhs(one);
            natural      res = add(lhs,rhs);
            xch(res);
            return *this;
        }

        natural  natural:: operator++(int)
        {
            const handle lhs(*this);
            word_type    one = 1;
            const handle rhs(one);
            natural      res = add(lhs,rhs);
            xch(res);
            return res;
        }

    }
}


