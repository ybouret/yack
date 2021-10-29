


//! \file
#ifndef YACK_IOS_IGZSTREAM_INCLUDED
#define YACK_IOS_IGZSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/ios/z/gzstream.hpp"

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
        class igzstream : public istream, public gzstream
        {
        public:
            virtual ~igzstream() throw(); //!< cleanup
            explicit igzstream(const char *filename); //!< open
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(igzstream);
            virtual bool   query_(char &C);
            virtual size_t fetch_(void *addr, const size_t size);
        };
    }
}

#endif
