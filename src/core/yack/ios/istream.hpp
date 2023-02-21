

//! \file
#ifndef YACK_IOS_ISTREAM_INCLUDED
#define YACK_IOS_ISTREAM_INCLUDED 1

#include "yack/ios/stream.hpp"
#include "yack/ios/characters.hpp"
#include "yack/object.hpp"
#include "yack/counted.hpp"

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
        class istream : public object, public counted
        {
        public:
            virtual ~istream() noexcept;                    //!< cleanup
            bool     query(char &C);                       //!< query one (cached) char
            void     store(char  C);                       //!< store char in cache
            size_t   fetch(void *addr, const size_t size); //!< fetch (cached) block
            bool     gets(characters &line);               //!< detect line
            
        protected:
            explicit istream() noexcept; //!< setup

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(istream);
            characters cache;
            virtual bool   query_(char &C) = 0;
            virtual size_t fetch_(void *addr, const size_t size) = 0;
        };
    }
}

#endif
