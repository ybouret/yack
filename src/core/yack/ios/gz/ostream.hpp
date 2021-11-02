



//! \file
#ifndef YACK_IOS_OGZSTREAM_INCLUDED
#define YACK_IOS_OGZSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
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
            class ostream : public ios::ostream, public stream
            {
            public:
                //! close
                virtual ~ostream() throw();

                //! open
                explicit ostream(const char *filename, const int level=6); //!< open

                virtual void write(const char C);
                virtual void flush();

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(ostream);

            };
        }
    }
}

#endif
