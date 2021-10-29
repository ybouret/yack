

//! \file
#ifndef YACK_IOS_GZSTREAM_INCLUDED
#define YACK_IOS_GZSTREAM_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for streams
        //
        //______________________________________________________________________
        class gzstream
        {
        public:
            virtual ~gzstream() throw(); //!< cleanup
            
        protected:
            //! open any gzFile
            explicit gzstream(const char *filename, const char *mode);
            void *GZ; //!< handle got gzFile

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(gzstream);
        };
    }
}

#endif
