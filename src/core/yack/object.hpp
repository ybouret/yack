//! \file

#ifndef YACK_OBJECT_INCLUDED
#define YACK_OBJECT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for (small) objects
    //
    //__________________________________________________________________________
    class object
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~object() throw();
        explicit object() throw();

        //______________________________________________________________________
        //
        // single object new/delete
        //______________________________________________________________________
        static void * operator new(const size_t block_size);                             //!< new(block_size)
        static void   operator delete(void *block_addr,const size_t block_size) throw(); //!< delete(block_size)

        //______________________________________________________________________
        //
        // multiple objects new[]/delete[]
        //______________________________________________________________________
        static void * operator new    [](const size_t block_size);                           //!< new[block_size]
        static void   operator delete [](void *block_addr, const size_t block_size) throw(); //!< delete[block_size]

        //______________________________________________________________________
        //
        // placement new()/delete()
        //______________________________________________________________________
        static void *operator new(size_t block_size, void *addr) throw(); //!< placement new
        static void  operator delete(void *, void *) throw();             //!< placement delete

        //______________________________________________________________________
        //
        // helpers
        //______________________________________________________________________

        //! acquire one zombie block
        template <typename T> static inline
        T *zacquire() { return static_cast<T *>( object::operator new(sizeof(T)) ); }

        //! release one zombie block
        template <typename T> static inline
        void zrelease(T *obj) throw() { assert(obj); object::operator delete(obj,sizeof(T)); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(object);
    };
    
}

#endif

