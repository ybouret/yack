//! \file

#ifndef YACK_IOS_OVSTREAM_INCLUDED
#define YACK_IOS_OVSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/sequence/vector.hpp"

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
        class ovstream : public ostream
        {
        public:
            typedef vector<char,ALLOCATOR> vbytes; //!< alias

            inline explicit ovstream() throw() : ostream(), buffer() {}                    //!< setup
            inline virtual ~ovstream() throw() {}                                        //!< cleanup
            inline explicit ovstream(const size_t n) : ostream(), buffer(n,as_capacity) {} //!< setup with capacity


            inline virtual void write(const char C) { buffer.push_back(C); } //!< write
            inline virtual void flush() {}                                 //!< flush

            vbytes *       operator->() throw()       { return &buffer; }
            const vbytes * operator->() const throw() { return &buffer; }
            
            vbytes buffer;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ovstream);
        };

        

    }
}

#endif

