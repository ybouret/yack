//! \file

#ifndef YACK_PRIORITY_QUEUE_INCLUDED
#define YACK_PRIORITY_QUEUE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/mswap.hpp"
#include "yack/type/out-of-reach.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! priority queue operations
    //
    //__________________________________________________________________________
    template <typename T>
    struct priority_queue
    {
        //______________________________________________________________________
        //
        // types and definitions
        //______________________________________________________________________
        YACK_DECL_ARGS_(T,type); //!< aliases

        //______________________________________________________________________
        //
        // operations
        //______________________________________________________________________

        //! insert with enough linear memory
        /**
         \param tree    linear space
         \param count   insert position (updated)
         \param args    for copy constructor
         \param compare comparison function
         */
        template <typename COMPARE> static inline
        void insert(mutable_type *tree,
                    size_t       &count,
                    const_type   &args,
                    COMPARE      &compare)
        {
            assert(tree);

            //------------------------------------------------------------------
            // insert at ipos=count
            //------------------------------------------------------------------
            size_t ipos = count;
            new ( &tree[ipos] ) mutable_type(args);

            //------------------------------------------------------------------
            // repetitive promotion (no throw)
            //------------------------------------------------------------------
            while(ipos>0)
            {
                const size_t  ppos   = (ipos-1)>>1;
                mutable_type &myself = tree[ipos];
                mutable_type &parent = tree[ppos];
                if( compare(parent,myself) < 0 )
                {
                    mswap(myself,parent);
                    ipos = ppos;
                    continue;
                }
                else
                    break;
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
            ++count;
        }

        //! removing top element
        template <typename COMPARE> static inline
        void remove(mutable_type *tree,
                    size_t       &count,
                    COMPARE      &compare) throw()
        {
            assert(count>0);

            //------------------------------------------------------------------
            // filter cases
            //------------------------------------------------------------------
            switch(count)
            {
                case 0: // shouldn't get here..
                    return;

                case 1: // last iterm
                    out_of_reach::naught( destructed( &tree[0]) );
                    count = 0;
                    return;

                default:
                    break;
            }

            //------------------------------------------------------------------
            // contract tree: put last item at top
            //------------------------------------------------------------------
            assert(count>1);
            {
                uint8_t *target = static_cast<uint8_t *>(out_of_reach::address( destructed( &tree[0] ) ));
                uint8_t *source = static_cast<uint8_t *>(out_of_reach::address( &tree[--count] ));
                for(size_t i=0;i<sizeof(type);++i)
                {
                    target[i] = source[i];
                    source[i] = 0;
                }
            }

            //------------------------------------------------------------------
            // then rearrange tree
            //------------------------------------------------------------------
            size_t       ipos = 0;
        PROMOTE:
            const size_t temp = ipos<<1;
            const size_t lpos = temp+1;
            const size_t rpos = temp+2;
            size_t mpos = ( lpos<count && compare(tree[ipos],tree[lpos])<0 ) ? lpos : ipos;
            if( rpos<count && compare(tree[mpos],tree[rpos])<0 ) mpos = rpos;
            
            if(mpos==ipos)
            {
                // done
                return;
            }
            else
            {
                // promote
                mswap(tree[ipos],tree[mpos]);
                ipos = mpos;
                goto PROMOTE;
            }

        }


    };

}

#endif

