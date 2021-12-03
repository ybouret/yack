//! \file

#ifndef YACK_IOS_OSSTREAM_INCLUDED
#define YACK_IOS_OSSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/string.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output string stream
        //
        //______________________________________________________________________
        class osstream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~osstream()               throw(); //!< cleanup
            explicit osstream(string &target) throw(); //!< setup
            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual void write(char C); //!< append to host
            virtual void flush();       //!< do nothing

        private:
            string &host;
            YACK_DISABLE_COPY_AND_ASSIGN(osstream);
        };

    }
}

#endif

