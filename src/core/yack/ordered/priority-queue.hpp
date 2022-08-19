//! \file

#ifndef YACK_PRIORITY_QUEUE_INCLUDED
#define YACK_PRIORITY_QUEUE_INCLUDED 1

#include "yack/type/args.hpp"
#include "yack/type/mswap.hpp"

namespace yack
{

    template <typename T>
    struct priority_queue
    {
        YACK_DECL_ARGS_(T,type);

        //! insert with enough linear memory
        /**
         \param tree   linear space
         \param count  insert position (updated)
         \param args   for copy constructor
         \param proc   comparison function
         */
        template <typename COMPARE>
        static inline
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


    };

}

#endif

