
#include "yack/apex/natural.hpp"

namespace yack
{
    namespace apex
    {
        
        YACK_APN_BINARY_REP(int natural::compare,
                            throw() { YACK_APN_BINARY_IMPL(cmp); })
        
        int natural:: cmp(const handle &l,
                          const handle &r) throw()
        {
            // sanity check
            const size_t     nl  = l.words;
            const size_t     nr  = r.words;
            
            if(nl<nr)
            {
                return -1;
            }
            else
            {
                if(nr<nl)
                {
                    return 1;
                }
                else
                {
                    assert(nl==nr);
                    const word_type *lhs = l.entry+nl;
                    const word_type *rhs = r.entry+nr;
                    for(size_t i=nl;i>0;--i)
                    {
                        const word_type L = *(--lhs);
                        const word_type R = *(--rhs);
                        if(L<R)
                        {
                            return -1;
                        }
                        else
                        {
                            if(R<L)
                            {
                                return 1;
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                    return 0;
                }
            }
        }
        
        sign_type natural:: scmp(const handle &l,
                                 const handle &r) throw()
        {
            // sanity check
            const size_t     nl  = l.words;
            const size_t     nr  = r.words;
            
            if(nl<nr)
            {
                return negative;
            }
            else
            {
                if(nr<nl)
                {
                    return positive;
                }
                else
                {
                    assert(nl==nr);
                    const word_type *lhs = l.entry+nl;
                    const word_type *rhs = r.entry+nr;
                    for(size_t i=nl;i>0;--i)
                    {
                        const word_type L = *(--lhs);
                        const word_type R = *(--rhs);
                        if(L<R)
                        {
                            return negative;
                        }
                        else
                        {
                            if(R<L)
                            {
                                return positive;
                            }
                            else
                            {
                                continue;
                            }
                        }
                    }
                    return __zero__;
                }
            }
        }
        
    }
    
}
