


//! \file
#ifndef YACK_IOS_IGZSTREAM_INCLUDED
#define YACK_IOS_IGZSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/ios/gz/stream.hpp"

namespace yack
{
    namespace ios
    {
        namespace gz
        {
            //__________________________________________________________________
            //
            //
            //! input gzstream
            //
            //__________________________________________________________________
            class istream : public ios::istream, public stream
            {
            public:
                virtual ~istream() noexcept;               //!< cleanup
                explicit istream(const char   *filename); //!< open
                explicit istream(const string &filename); //!< open

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(istream);
                virtual bool   query_(char &C);
                virtual size_t fetch_(void *addr, const size_t size);
            };
        }
    }
}

#endif
