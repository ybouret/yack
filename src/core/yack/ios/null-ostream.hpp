
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
            virtual ~null_ostream() noexcept; //!< cleanup
            explicit null_ostream() noexcept; //!< setup

            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void write(const char) noexcept;   //!< do nothing
            virtual void flush()           noexcept;   //!< do nothing


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(null_ostream);
        };
    }
}

#endif
