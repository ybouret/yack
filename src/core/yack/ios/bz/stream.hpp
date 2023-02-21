


//! \file
#ifndef YACK_IOS_BZSTREAM_INCLUDED
#define YACK_IOS_BZSTREAM_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace ios
    {
        namespace bz
        {
            //__________________________________________________________________
            //
            //
            //! base class for streams
            //
            //__________________________________________________________________
            class stream
            {
            public:
                virtual ~stream() noexcept; //!< cleanup
                static const char *errstr(const int err) noexcept; //!< named errors


            protected:
                explicit stream(void*) noexcept;  //!< setup handle
                void *BZ;                        //!< handle for BZFILE

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(stream);
            };
        }
    }
}

#endif
