

//! \file

#ifndef YACK_MEMORY_BUFFER_FIXED_INCLUDED
#define YACK_MEMORY_BUFFER_FIXED_INCLUDED 1

#include "yack/memory/buffer/ro.hpp"

namespace yack
{

    namespace memory
    {

        //______________________________________________________________________
        //
        //
        //! read-only, fixed buffer
        //
        //______________________________________________________________________
        class fixed_buffer : public ro_buffer
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~fixed_buffer()                  throw(); //!< cleanup
            fixed_buffer(const fixed_buffer &)       throw(); //!< soft-copy
            fixed_buffer(const void *, const size_t) throw(); //!< assign persistent area
            fixed_buffer(const char *)               throw(); //!< assign from persistent text

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t      measure() const throw(); //!< size_
            virtual const void *ro_addr() const throw(); //!< addr_

        private:
            YACK_DISABLE_ASSIGN(fixed_buffer);
            const void  * const addr_;
            const size_t        size_;
        };

    }

}

#endif
