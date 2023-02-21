
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

        //______________________________________________________________________
        //
        //
        //! parallel kernel
        //
        //______________________________________________________________________
        typedef void (*kernel)(const context &,
                               void          *,
                               lockable      &);


        //______________________________________________________________________
        //
        //
        //! runnable interface with a private run method
        //
        //______________________________________________________________________
        class runnable
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~runnable() noexcept; //!< cleanup

        protected:
            explicit runnable() noexcept; //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(runnable);
            friend class loop;
            static  void call(const context &, void *,lockable &) noexcept;
            virtual void run(const context &, lockable &) noexcept = 0;
            
        };
    }

}

#endif

