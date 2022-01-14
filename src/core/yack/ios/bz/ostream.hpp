
//! \file
#ifndef YACK_IOS_OBZSTREAM_INCLUDED
#define YACK_IOS_OBZSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/bz/stream.hpp"

namespace yack
{
    namespace ios
    {
        namespace bz
        {
            //__________________________________________________________________
            //
            //
            //! output bzstream
            //
            //__________________________________________________________________
            class ostream : public ios::ostream, public stream
            {
            public:
                static const char clid[]; //!< bz::ostream

                virtual ~ostream() throw();               //!< cleanup
                explicit ostream(const string &filename, const int level=6); //!< open file
                explicit ostream(const char   *filename, const int level=6); //!< open file


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(ostream);

            };
        }
    }
}

#endif
