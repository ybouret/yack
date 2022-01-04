//! \file

#ifndef YACK_THING_INCLUDED
#define YACK_THING_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    //__________________________________________________________________________
    //
    //
    //! base class for (midsize) thing
    //
    //__________________________________________________________________________
    class thing
    {
    public:
        //______________________________________________________________________
        //
        // C++
        //______________________________________________________________________
        virtual ~thing() throw();
        explicit thing() throw();

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
        T *zacquire() { return static_cast<T *>( thing::operator new(sizeof(T)) ); }

        //! release one zombie block
        template <typename T> static inline
        void zrelease(T *obj) throw() { assert(obj); thing::operator delete(obj,sizeof(T)); }


    private:
        YACK_DISABLE_COPY_AND_ASSIGN(thing);
    };
    
}

#endif

