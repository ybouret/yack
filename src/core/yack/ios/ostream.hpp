//! \file

#ifndef YACK_IOS_OSTREAM_INCLUDED
#define YACK_IOS_OSTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"
#include "yack/string/fwd.hpp"

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
            //   interface
            //__________________________________________________________________
            virtual void   write(const char) = 0;             //!< write one char
            virtual void   flush()           = 0;             //!< flush if necessary

            //__________________________________________________________________
            //
            // default virtual functions
            //__________________________________________________________________
            virtual void   frame(const void *, const size_t); //!< default: write all block as chars
            virtual size_t operator()(const char *fmt,...);   //!< default: format to block, write block
            
            //__________________________________________________________________
            //
            // non virtual interface
            //__________________________________________________________________
            ostream & operator<<(const char C);     //!< uses write
            ostream & operator<<(const char *msg);  //!< uses frame
            ostream & operator<<(const string &);   //!< uses frame

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
