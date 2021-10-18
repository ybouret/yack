//! \file

#ifndef YACK_SYNC_LOOP_INCLUDED
#define YACK_SYNC_LOOP_INCLUDED 1

#include "yack/lockable.hpp"
#include "yack/container/readable.hpp"
#include "yack/concurrent/context.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! interface for Single Instruction Multiple Data
        //
        //______________________________________________________________________
        class loop : public readable<context>
        {
        public:
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual lockable   &access()       throw() = 0; //!< to sync
            virtual const char *family() const throw() = 0; //!< for info

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~loop() throw();
        protected:
            explicit loop() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(loop);
        };

    }

}

#endif
