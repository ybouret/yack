
//! \file

#ifndef YACK_SYNC_LOOP_SIMD_INCLUDED
#define YACK_SYNC_LOOP_SIMD_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/mutex.hpp"

namespace yack
{
    namespace concurrent
    {

        class simd : public loop
        {
        public:
            virtual ~simd() throw();
            explicit simd() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(simd);
        };

    }

}

#endif
