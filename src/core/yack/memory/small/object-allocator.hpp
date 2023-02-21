
//! \file

#ifndef YACK_SMALL_OBJECT_ALLOCATOR_INCLUDED
#define YACK_SMALL_OBJECT_ALLOCATOR_INCLUDED

#include "yack/memory/small/objects.hpp"
#include "yack/memory/small/objects-longevity.hpp"
#include "yack/singleton.hpp"

#if !defined(YACK_LIMIT_SIZE)
#define YACK_LIMIT_SIZE 256 //!< default limit size for small objects
#endif



namespace yack
{
    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! single class for small objects with a given limit size
        //
        //______________________________________________________________________
        template <size_t LIMIT_SIZE>
        class small_object_allocator : public singleton< small_object_allocator<LIMIT_SIZE> >,
        public small_objects
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            typedef small_object_allocator<LIMIT_SIZE> self_type;                                       //!< alias
            static  const at_exit::longevity           life_time = YACK_MEMORY_SMALL_OBJECTS_LONGEVITY; //!< alias
            static  const char  * const                call_sign;                                       //!< for singleton
            using concurrent::single::access;


            //__________________________________________________________________
            //
            //thread safe methods
            //__________________________________________________________________

            //! acquire a blank block
            inline void *acquire(const size_t block_size)
            {
                YACK_LOCK(access);
                return acquire_unlocked(block_size);
            }

            //! release a previously acquired block
            inline void release(void *block_addr, const size_t block_size) noexcept
            {
                YACK_LOCK(access);
                release_unlocked(block_addr,block_size);
            }

        private:
            inline explicit small_object_allocator() : singleton<self_type>(), small_objects(LIMIT_SIZE) {}
            inline virtual ~small_object_allocator() noexcept {}

            YACK_DISABLE_COPY_AND_ASSIGN(small_object_allocator);
            friend class singleton<self_type>;
        };

        template <size_t LIFE_TIME> const char * const small_object_allocator<LIFE_TIME>::call_sign = small_objects::designation;
    }

}

#endif
