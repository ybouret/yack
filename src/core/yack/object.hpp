//! \file

#ifndef YACK_OBJECT_INCLUDED
#define YACK_OBJECT_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    
    class object
    {
    public:
        virtual ~object() throw();
        explicit object() throw();

        static void * operator new(const size_t block_size);
        static void   operator delete(void *block_addr,const size_t block_size) throw();

        static void * operator new [](const size_t block_size);
        static void   operator delete [](void *block_addr, const size_t block_size) throw();

        static void *operator new(size_t block_size, void *addr) throw();
        static void  operator delete(void *, void *) throw(); //! placement delete

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(object);
    };
    
}

#endif

