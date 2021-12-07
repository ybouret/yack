#include "yack/jive/pattern/first-bytes.hpp"
#include "yack/jive/pattern/basic/single.hpp"
#include "yack/jive/pattern/basic/within.hpp"

namespace yack
{
    namespace jive
    {
        void first_bytes:: build(list_of<pattern> &P) const
        {
            patterns si;
            patterns in;

            for(const domain *dom=head;dom;dom=dom->next)
            {
                const uint8_t lo = dom->lower;
                const uint8_t up = dom->upper;
                if(lo<up)
                    in.push_back( new within(lo,up) );
                else
                    si.push_back( new single(lo) );
            }

            P.merge_back(si);
            P.merge_back(in);
        }

    }

}

