//! \file

#ifndef YACK_SYNC_LOOP_MONO_INCLUDED
#define YACK_SYNC_LOOP_MONO_INCLUDED 1

#include "yack/concurrent/loop.hpp"
#include "yack/concurrent/fake-lock.hpp"

namespace yack
{
    namespace concurrent
    {

        class mono : public loop
        {
        public:
            static const char clid[];
            explicit mono() throw();
            virtual ~mono() throw();

            virtual size_t       size()                   const throw();
            virtual const_type & operator[](const size_t) const throw();

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
