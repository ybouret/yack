
#include "yack/apex/natural.hpp"

namespace yack
{
    namespace apex
    {

        YACK_APN_BINARY_REP(int natural::compare,
        const throw() { YACK_APN_BINARY_IMPL(cmp); })
        
        int natural:: cmp(const handle &l,
                          const handle &r) throw()
        {
            // sanity check
            const size_t     nl  = l.n;
            const size_t     nr  = r.n;


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
                    const word_type *lhs = l.w+nl;
                    const word_type *rhs = r.w+nr;
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

    }

}
