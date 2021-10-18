//! \file

#ifndef YACK_SYNC_LOOP_MONO_INCLUDED
#define YACK_SYNC_LOOP_MONO_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/fake-lock.hpp"

namespace yack
{
    namespace concurrent
    {

        //______________________________________________________________________
        //
        //
        //! sequential loop
        //
        //______________________________________________________________________
        class mono : public loop
        {
        public:
            static const char clid[]; //!< "mono"

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit mono() throw(); //!< setup sequential
            virtual ~mono() throw(); //!< cleanup

            //__________________________________________________________________
            //
            // readable interface
            //__________________________________________________________________
            virtual size_t       size()                   const throw();
            virtual const_type & operator[](const size_t) const throw();

            //__________________________________________________________________
            //
            // loop interface
            //__________________________________________________________________
            virtual lockable   & access()       throw();
            virtual const char * family() const throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(mono);
            const context ctx;
            fake_lock     acc;
        };

    }

}

#endif
