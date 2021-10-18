//! \file

#ifndef YACK_SYNC_CONTEXT_INCLUDED
#define YACK_SYNC_CONTEXT_INCLUDED 1

#include "yack/concurrent/split.hpp"

namespace yack
{
    namespace concurrent
    {

        class context
        {
        public:
            const size_t size;
            const size_t rank;
            const size_t indx;

            context()  throw(); //!< sequential
            ~context() throw(); //!< cleanup
            context(const size_t sz, const size_t rk) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(context);
        };
        
    }

}

#endif
