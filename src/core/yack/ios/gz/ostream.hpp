



//! \file
#ifndef YACK_IOS_OGZSTREAM_INCLUDED
#define YACK_IOS_OGZSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/gz/stream.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! input gzstream
        //
        //______________________________________________________________________
        class ogzstream : public ostream, public gzstream
        {
        public:
            virtual ~ogzstream() throw();             //!< cleanup
            explicit ogzstream(const char *filename); //!< open
            
            virtual void write(const char C);
            virtual void flush();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ogzstream);
            
        };
    }
}

#endif
