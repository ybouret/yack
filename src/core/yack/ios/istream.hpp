

//! \file
#ifndef YACK_IOS_ISTREAM_INCLUDED
#define YACK_IOS_ISTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"
#include "yack/ios/character.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! base class for input streams
        //
        //______________________________________________________________________
        class istream
        {
        public:
            virtual ~istream() throw();                    //!< cleanup
            bool     query(char &C);                       //!< query one (cached) char
            void     store(char  C);                       //!< store char in cache
            size_t   fetch(void *addr, const size_t size); //!< fetch (cached) block
            bool     gets(characters &line);

        protected:
            explicit istream() throw(); //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(istream);
            characters cache;
            virtual bool   query_(char &C) = 0;
            virtual size_t fetch_(void *addr, const size_t size) = 0;
        };
    }
}

#endif
