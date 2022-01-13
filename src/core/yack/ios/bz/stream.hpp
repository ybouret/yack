


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

            protected:
                explicit stream(void *) throw();

                void *BZ; //!< handle got bzFile

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(stream);
            };
        }
    }
}

#endif
