
//! \file

#ifndef YACK_SYNC_LOOP_RUNNABLE_INCLUDED
#define YACK_SYNC_LOOP_RUNNABLE_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/concurrent/context.hpp"

namespace yack
{
    namespace concurrent
    {

        class loop;
        typedef void (*kernel)(const context &,
                               void          *,
                               lockable      &);


        class runnable
        {
        public:
            virtual ~runnable() throw();


        protected:
            explicit runnable() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(runnable);
            friend class loop;
            static  void call(const context &, void *,lockable &) throw();
            virtual void run(const context &, lockable &) throw() = 0;

        };
    }

}

#endif

