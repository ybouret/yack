
//! \file

#ifndef YACK_RANDOMIZED_SHUFFLE_INCLUDED
#define YACK_RANDOMIZED_SHUFFLE_INCLUDED 1

#include "yack/randomized/bits.hpp"
#include "yack/type/mswap.hpp"

namespace yack
{

    namespace randomized
    {

        struct shuffle
        {

            template <typename T> static inline
            void data(T addr[], const size_t size, bits &ran) throw()
            {
                assert(!(NULL==addr&&size>0));
                if(size>1)
                {
                    for(size_t i=size-1;i>0;--i)
                    {
                        const size_t j = ran.leq(i);
                        assert(j<=i);
                        mswap(addr[i],addr[j]);
                    }
                }
            }

            //! card-desk shuffle for lists
            template <typename LIST> inline
            void list(LIST &l, bits &ran) throw()
            {
                LIST tmp;
                while(l.size)
                {
                    typename LIST::node_type *node=l.pop_back();
                    if(ran.choice()) tmp.push_back(node); else tmp.push_front(node);
                }
                tmp.swap_with(l);
            }
            


        };

    }

}

#endif
