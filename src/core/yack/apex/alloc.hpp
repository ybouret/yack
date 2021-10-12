//! \file

#ifndef YACK_APEX_ALLOC_INCLUDED
#define YACK_APEX_ALLOC_INCLUDED 1

#include "yack/arith/ilog2.hpp"

namespace yack
{

    namespace apex
    {
        //______________________________________________________________________
        //
        //
        //! internal allocation
        //
        //______________________________________________________________________
        struct alloc
        {
            //! allocate 1 << block_exp2 bytes
            static void *block_acquire(int &block_exp2);

            //! release an old block
            static void  block_release(void * &block_addr, int &block_exp2) throw();

            //! allocate items_size = (1 << items_exp2), with updated block_exp2
            template <typename T> inline
            static T    *field_acquire(int &items_exp2, int &block_exp2)
            {
                assert(items_exp2>=0);
                assert(block_exp2<0);
                block_exp2 = items_exp2 + ilog2_of<T>::value;
                try {
                    return static_cast<T*>(block_acquire(block_exp2));
                }
                catch(...)
                {
                    items_exp2=-1;
                    throw;
                }
            }

            //! release a previously acquired field
            template <typename T> inline
            static void field_release(T * &entry, int &items_exp2, int &block_exp2) throw()
            {
                assert(NULL!=entry);
                assert(items_exp2>=0);
                assert(block_exp2>=0);
                assert(items_exp2+ilog2_of<T>::value==block_exp2);
                block_release(*(void **)&entry,block_exp2);
                items_exp2=-1;
            }


        };

    }

}

#endif

