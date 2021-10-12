//! \file

#ifndef YACK_APEX_TYPES_INCLUDED
#define YACK_APEX_TYPES_INCLUDED 1

#include "yack/type/ints.hpp"
#include "yack/arith/ilog2.hpp"

namespace yack
{

    namespace apex
    {

#define YACK_APEX_TYPE uint16_t

#if !defined(YACK_APEX_TYPE)
#     define YACK_APEX_TYPE void *
#endif

        struct cull
        {
            typedef unsigned_for<YACK_APEX_TYPE>::type work_type;
            static const size_t work_size = sizeof(work_type);
            static const size_t work_bits = work_size << 3;
            static const size_t core_size = work_size >> 1;
            static const size_t core_bits = work_bits >> 1;
            typedef unsigned_int<core_size>::type core_type;


            static void *block_acquire(int &block_exp2);
            static void  block_release(void * &block_addr, int &block_exp2) throw();

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
