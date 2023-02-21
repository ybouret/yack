//! \file

#ifndef YACK_LARGE_OBJECT_INCLUDED
#define YACK_LARGE_OBJECT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for large_object
    //
    //__________________________________________________________________________
    class large_object
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~large_object() noexcept;
        explicit large_object() noexcept;

        //______________________________________________________________________
        //
        // single object new/delete
        //______________________________________________________________________
        static void * operator new(const size_t block_size);                             //!< new(block_size)
        static void   operator delete(void *block_addr,const size_t block_size) noexcept; //!< delete(block_size)

        //______________________________________________________________________
        //
        // multiple objects new[]/delete[]
        //______________________________________________________________________
        static void * operator new    [](const size_t block_size);                           //!< new[block_size]
        static void   operator delete [](void *block_addr, const size_t block_size) noexcept; //!< delete[block_size]

        //______________________________________________________________________
        //
        // placement new()/delete()
        //______________________________________________________________________
        static void *operator new(size_t block_size, void *addr) noexcept; //!< placement new
        static void  operator delete(void *, void *) noexcept;             //!< placement delete

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! acquire one zombie block
        template <typename T> static inline
        T *zacquire() { return static_cast<T *>( large_object::operator new(sizeof(T)) ); }

        //! release one zombie block
        template <typename T> static inline
        void zrelease(T *obj) noexcept { assert(obj); large_object::operator delete(obj,sizeof(T)); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(large_object);
    };
    
}

#endif

