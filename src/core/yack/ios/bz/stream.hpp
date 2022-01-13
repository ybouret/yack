


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
                virtual ~stream() throw(); //!< cleanup
                static const char *errstr(const int err) throw(); //!< named errors


            protected:
                explicit stream(void *) throw(); //!< assign BZ
                void *BZ;                        //!< handle for BZFILE

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(stream);
            };
        }
    }
}

#endif
