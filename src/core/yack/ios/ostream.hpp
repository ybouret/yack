

//! \file
#ifndef YACK_IOS_OSTREAM_INCLUDED
#define YACK_IOS_OSTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"

namespace yack
{
    namespace ios
    {
        class ostream
        {
        public:
            virtual ~ostream() throw();

            // interface
            virtual void write(const char) = 0;
            virtual void frame(const void *, const size_t); //!< default: write all message
            virtual void operator()(const char *fmt,...);   //!< default it not implemented yet

            ostream & operator<<(const char C);
            ostream & operator<<(const char *msg);


        protected:
            explicit ostream() throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
