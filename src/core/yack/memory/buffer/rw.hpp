

//! \file

#ifndef YACK_MEMORY_BUFFER_RW_INCLUDED
#define YACK_MEMORY_BUFFER_RW_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! read-write buffer interface
        //
        //______________________________________________________________________
        class rw_buffer : public ro_buffer
        {
        public:
            void    *rw_addr() throw(); //!< from ro_addr

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~rw_buffer() throw(); //!< cleanup
        protected:
            explicit rw_buffer() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(rw_buffer);
        };

    }

}

#endif
