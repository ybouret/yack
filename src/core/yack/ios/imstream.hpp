


//! \file
#ifndef YACK_IOS_IMSTREAM_INCLUDED
#define YACK_IOS_IMSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/memory/buffer/ro.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //!  input memory stream
        //
        //______________________________________________________________________
        class imstream : public istream
        {
        public:
            virtual ~imstream() noexcept;                                    //!< cleanup
            explicit imstream(const void *addr, const size_t size) noexcept; //!< setup
            explicit imstream(const memory::ro_buffer &) noexcept;           //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(imstream);
            const uint8_t *data;
            size_t         left;
            virtual bool   query_(char &C);
            virtual size_t fetch_(void *addr, const size_t size);
        };
    }
}

#endif
