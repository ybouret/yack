
//! \file

#ifndef YACK_DATA_LIST_ORDERED_INCLUDED
#define YACK_DATA_LIST_ORDERED_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/signs.hpp"

namespace yack
{

    struct ordered_list
    {

        template <typename DATA, typename NODE>
        bool search(const list_of<NODE> &l,
                    const DATA          &value,
                    sign_type          (*compare)(DATA &, const NODE *),
                    NODE * &             target)
        {
            assert(NULL!=compare);
            assert(NULL==target);
            const size_t size = l.size;
            if(size<=0)
            {
                return false;
            }
            else
            {

                const NODE *lower = l.head;
                switch(compare(value,lower))
                {
                    case negative: return false;
                    case __zero__: target = coerce(lower); return true;
                    default:
                        break;
                }
                // data is strictly after lower

                const NODE *upper = l.tail;
                switch(compare(value,lower))
                {
                    case positive: target = coerce(upper); return false;
                    case __zero__: target = coerce(upper); return true;
                    default:
                        break;
                }

                // data is strictly before upper as well
                assert(size>=2);

                return false;
            }

        }


    };

}

#endif
