//! \file

#ifndef YACK_IOS_ITSTREAM_INCLUDED
#define YACK_IOS_ITSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! input token stream
        //
        //______________________________________________________________________
        class itstream : public ios::istream
        {
        public:
            explicit itstream(const characters &) noexcept;
            virtual ~itstream()                   noexcept;

        private:
            const character *curr;
            size_t           left;
            YACK_DISABLE_COPY_AND_ASSIGN(itstream);
            virtual bool   query_(char &C)                      noexcept;
            virtual size_t fetch_(void *addr, const size_t size) noexcept;
        };
    }

}
#endif

