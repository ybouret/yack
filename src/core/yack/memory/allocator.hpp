
//! \file

#ifndef YACK_MEMORY_ALLOCATOR_INCLUDED
#define YACK_MEMORY_ALLOCATOR_INCLUDED 1

#include "yack/type/out-of-reach.hpp"

namespace yack
{
    namespace memory
    {
        //______________________________________________________________________
        //
        //
        //! base class for allocators
        //
        //______________________________________________________________________
        class allocator
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________

            //! calloc-style memory acquire
            /**
             'calloc(count,block_size)' then count *= block_size
             */
            virtual void       *acquire(size_t &count, const size_t block_size) = 0;

            //! release a previously acquire memory area
            virtual void        release(void * &addr, size_t &size)     throw() = 0;

            //! name for allocator
            virtual const char *variety()                         const throw() = 0;

            //__________________________________________________________________
            //
            // non-virtual interface
            //__________________________________________________________________

            //! helper to acquire memory for a field of objects
            template <typename T> inline
            T *invoke(size_t &items,
                      size_t &bytes)
            {
                try
                {
                    void *addr = acquire(bytes=items,sizeof(T));
                    assert(bytes>=items*sizeof(T));
                    items = bytes/sizeof(T);
                    return static_cast<T *>(addr);
                }
                catch(...) { items = bytes = 0; throw ; }
            }

            //! helper to release memory from a field of objects
            template <typename T> inline
            void revoke(T    * &entry,
                        size_t &bytes) throw()
            {
                void * &addr = *(void **) &entry;
                release(addr,bytes);
            }

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~allocator() throw(); //!< cleanup

        protected:
            explicit allocator() throw(); //!< setup
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(allocator);
        };
    }
    
}

#endif

