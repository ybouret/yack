
//! \file

#ifndef YACK_DATA_LIST_ORDERED_INCLUDED
#define YACK_DATA_LIST_ORDERED_INCLUDED 1

#include "yack/data/list.hpp"
#include "yack/signs.hpp"

namespace yack
{
    //__________________________________________________________________________
    //
    //
    //! operations on ordered list
    //
    //__________________________________________________________________________
    struct ordered_list
    {

        //______________________________________________________________________
        //
        //! search algorithm
        /**
         * \param l       the list
         * \param value   value of a node
         * \param compare compare(value,node)
         * \param target  the exact node or the node BEFORE
         * \return value was found
         */
        //______________________________________________________________________

        template <typename DATA, typename NODE> static inline
        bool search(const list_of<NODE> &l,
                    const DATA          &value,
                    sign_type          (*compare)(const DATA &, const NODE *),
                    NODE * &             target)
        {
            assert(NULL!=compare);
            assert(NULL==target);
            const size_t size = l.size;
            if(size<=0)
            {
                //______________________________________________________________
                //
                // no way
                //______________________________________________________________
                return false;
            }
            else
            {
                //______________________________________________________________
                //
                // initialize lower search
                //______________________________________________________________
                const NODE *lower = l.head;
                switch(compare(value,lower))
                {
                    case negative: assert(NULL==target);  return false; // => push_front
                    case __zero__: target = (NODE*)lower; return true; 
                    default:
                        break;
                }

                //______________________________________________________________
                //
                // data is strictly after lower, initialize upper
                //______________________________________________________________
                const NODE *upper = l.tail;
                switch(compare(value,upper))
                {
                    case positive: target = (NODE*)upper; return false;
                    case __zero__: target = (NODE*)upper; return true;
                    default:
                        break;
                }

                //______________________________________________________________
                //
                // data is strictly before upper as well
                //______________________________________________________________
                assert(size>=2);
                assert(positive==compare(value,lower));
                assert(negative==compare(value,upper));

                //______________________________________________________________
                //
                // one step search
                //______________________________________________________________
            SEARCH:
                const NODE *next = lower->next; assert(next);
                switch( compare(value,next) )
                {
                    case negative: target = (NODE*)lower; return false;
                    case __zero__: target = (NODE*)next;  return true;
                    case positive:
                        if(next==upper)
                        {
                            target = (NODE*)next;
                            return false;
                        }
                }
                lower = next;
                assert(lower!=upper);
                assert(positive==compare(value,lower));
                const NODE *prev = upper->prev;
                switch(compare(value,prev))
                {
                    case positive: target = (NODE*)prev; return false;
                    case __zero__: target = (NODE*)prev; return true;
                    case negative:
                        if(prev==lower)
                        {
                            target = (NODE*)prev;
                            return false;
                        }
                }
                upper = prev;
                assert(lower!=upper);
                assert(negative==compare(value,upper));
                goto SEARCH;

            }

        }


    };

}

#endif
