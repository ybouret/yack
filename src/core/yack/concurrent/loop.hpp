//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/lockable.hpp"

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

        class loop
        {
        public:
            virtual ~loop() throw();
            
        protected:

            explicit loop() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
