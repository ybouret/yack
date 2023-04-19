//! \file

#ifndef YACK_PRIORITY_QUEUE_INCLUDED
#define YACK_PRIORITY_QUEUE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/mswap.hpp"
#include "yack/type/out-of-reach.hpp"
#include "yack/type/destruct.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! low-level priority queue operations
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

        //! insert with ENOUGH linear memory
        /**
         \param tree    linear space
         \param count   insert position (updated)
         \param args    for copy constructor
         \param compare comparison function, test only < 0
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
            ++count;

            //------------------------------------------------------------------
            // repetitive promotion (no throw)
            //------------------------------------------------------------------
            while(ipos>0)
            {
                const size_t  ppos   = (ipos-1)>>1;
                mutable_type &myself = tree[ipos];
                mutable_type &parent = tree[ppos];
                if( compare(parent,myself) < 0 ) {
                    mswap(myself,parent);
                    ipos = ppos;
                    continue; // check next promotion from ipos
                }
                else
                    break;    // balanced
            }

            //------------------------------------------------------------------
            // done
            //------------------------------------------------------------------
        }

        
        //! removing top element
        template <typename COMPARE> static inline
        void remove(mutable_type *tree,
                    size_t       &count,
                    COMPARE      &compare) noexcept
        {
            assert(count>0);

            //------------------------------------------------------------------
            // filter cases
            //------------------------------------------------------------------
            switch(count)
            {
                case 0: // shouldn't get here..
                    return;

                case 1: // last item
                    out_of_reach::naught( destructed( &tree[0]) );
                    count = 0;
                    return;

                default:
                    break;
            }
            assert(count>1);

            //------------------------------------------------------------------
            // contract tree: put last item at top
            //------------------------------------------------------------------
            {
                void *target =  out_of_reach::address( destructed( &tree[0] ) );
                void *source =  out_of_reach::address( &tree[--count] );
                out_of_reach::zmov(target,source,sizeof(type));
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

        //! helper to destruct all
        static inline void finish(mutable_type *tree, size_t &count) noexcept
        {
            while(count>0)
                out_of_reach::naught( destructed( &tree[--count]) );
        }
    };

}

#endif

