

//! \file
#ifndef YACK_IOS_OSTREAM_INCLUDED
#define YACK_IOS_OSTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output stream base class
        //
        //______________________________________________________________________
        class ostream
        {
        public:
            //__________________________________________________________________
            //
            // virtual interface
            //__________________________________________________________________
            virtual void write(const char) = 0;             //!< write one char
            virtual void frame(const void *, const size_t); //!< default: write all block as chars
            virtual void operator()(const char *fmt,...);   //!< default it not implemented yet

            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            ostream & operator<<(const char C);     //!< uses write
            ostream & operator<<(const char *msg);  //!< uses frame

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ostream() throw(); //!< cleanup

        protected:
            explicit ostream() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ostream);
        };
    }
}

#endif
