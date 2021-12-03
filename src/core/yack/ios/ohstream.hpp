
//! \file

#ifndef YACK_IOS_OHSTREAM_INCLUDED
#define YACK_IOS_OHSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/hashing/function.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output hashing stream
        //
        //______________________________________________________________________
        class ohstream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ohstream() throw();                    //!< cleanup
            explicit ohstream(hashing::function &) throw(); //!< serup

            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            virtual void write(char C); //!< run in host
            virtual void flush();       //!< do nothing

        private:
            hashing::function &host;
            YACK_DISABLE_COPY_AND_ASSIGN(ohstream);
        };

    }
}

#endif

