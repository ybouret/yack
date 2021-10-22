
//! \file

#ifndef YACK_IOS_NULL_OSTREAM_INCLUDED
#define YACK_IOS_NULL_OSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! null output stream
        //
        //______________________________________________________________________
        class null_ostream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~null_ostream() throw(); //!< cleanup
            explicit null_ostream() throw(); //!< setup
            //!
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void write(const char) throw();             //!< write one char
            virtual void flush()           throw();
            


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(null_ostream);
        };
    }
}

#endif
