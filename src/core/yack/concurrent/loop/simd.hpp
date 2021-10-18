
//! \file

#ifndef YACK_SYNC_LOOP_SIMD_INCLUDED
#define YACK_SYNC_LOOP_SIMD_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/condition.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {

        class simd : public loop
        {
        public:
            static const char clid[];
            
            virtual ~simd() throw();
            explicit simd(const size_t n);

            virtual size_t       size()                   const throw();
            virtual const_type & operator[](const size_t) const throw();

            virtual lockable   & access()       throw();
            virtual const char * family() const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(simd);
            class worker;
            mutex        sync;
            condition    cond;
            const size_t nthr;
            size_t       capa;
            worker      *team;

        };

    }

}

#endif
