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

        //______________________________________________________________________
        //
        //
        //! smart array
        //
        //______________________________________________________________________
        template <typename T>
        class tableau
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            
            //! allocate for a required exp2
            inline tableau(const int required_exp2):
            items_exp2(required_exp2),
            block_exp2(-1),
            addr( alloc::field_acquire<T>( coerce(items_exp2), coerce(block_exp2) ) ),
            size( size_t(1) << items_exp2 )
            {
            }

            //! cleanup
            inline ~tableau() throw()
            {
                alloc::field_release<T>(addr,coerce(items_exp2), coerce(block_exp2) );
                coerce(size) = 0;
            }
            
            //__________________________________________________________________
            //
            // access
            //__________________________________________________________________
            inline T       & operator[](const size_t indx) throw()       { assert(indx<size); return addr[indx]; } //!< 0<=indx<size
            inline const T & operator[](const size_t indx) const throw() { assert(indx<size); return addr[indx]; } //!< 0<=indx<size
            
            
            const int    items_exp2; //!< items = 1 << items_exp2
            const int    block_exp2; //!< bytes = 1 << block_exp2

        private:
            T           *addr;       //!< entry
        public:
            const size_t size;       //!< 1<<items_exp2

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(tableau);
        };

    }

}

#endif

