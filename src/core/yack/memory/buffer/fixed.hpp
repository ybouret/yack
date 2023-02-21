

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
            virtual ~fixed_buffer()                  noexcept; //!< cleanup
            fixed_buffer(const fixed_buffer &)       noexcept; //!< soft-copy
            fixed_buffer(const void *, const size_t) noexcept; //!< assign persistent area
            fixed_buffer(const char *)               noexcept; //!< assign from persistent text

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual size_t      measure() const noexcept; //!< size_
            virtual const void *ro_addr() const noexcept; //!< addr_

        private:
            YACK_DISABLE_ASSIGN(fixed_buffer);
            const void  * const addr_;
            const size_t        size_;
        };

    }

}

#endif
