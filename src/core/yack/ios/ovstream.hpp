//! \file

#ifndef YACK_IOS_OVSTREAM_INCLUDED
#define YACK_IOS_OVSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/sequence/vector.hpp"
#include "yack/memory/allocator/dyadic.hpp"

namespace yack
{
    namespace ios
    {

        //______________________________________________________________________
        //
        //
        //! output vector stream
        //
        //______________________________________________________________________
        template <typename ALLOCATOR>
        class ovstream : public ostream, public vector<char,ALLOCATOR>
        {
        public:
            typedef vector<char,ALLOCATOR> vbytes; //!< alias

            inline explicit ovstream() throw() : ostream(), vbytes() {}                    //!< setup
            inline virtual ~ovstream() throw() {}                                          //!< cleanup
            inline explicit ovstream(const size_t n) : ostream(), vbytes(n,as_capacity) {} //!< setup with capacity


            inline virtual void write(const char C) { this->push_back(C); } //!< write
            inline virtual void flush() {}                                  //!< flush

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ovstream);
        };

        

    }
}

#endif

