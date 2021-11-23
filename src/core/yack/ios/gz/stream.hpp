

//! \file
#ifndef YACK_IOS_GZSTREAM_INCLUDED
#define YACK_IOS_GZSTREAM_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"

namespace yack
{
    namespace ios
    {
        namespace gz
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
                //! open any gzFile
                explicit stream(const char   *filename, const char *mode);
                explicit stream(const string &filename, const char *mode);

                void *GZ; //!< handle got gzFile
                
            private:
                YACK_DISABLE_COPY_AND_ASSIGN(stream);
            };
        }
    }
}

#endif
