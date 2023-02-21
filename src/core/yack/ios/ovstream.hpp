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
            //__________________________________________________________________
            //
            // types and definition
            //__________________________________________________________________
            typedef vector<char,ALLOCATOR> vbytes; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            inline explicit ovstream() noexcept : ostream(), buffer() {}                    //!< setup
            inline virtual ~ovstream() noexcept {}                                          //!< cleanup
            inline explicit ovstream(const size_t n) : ostream(), buffer(n,as_capacity) {} //!< setup with capacity


            //__________________________________________________________________
            //
            // interface
            //__________________________________________________________________
            inline virtual void write(const char C) { buffer.push_back(C); } //!< write
            inline virtual void flush() {}                                   //!< flush

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            vbytes *       operator->() noexcept       { return &buffer; } //!< syntax helper
            const vbytes * operator->() const noexcept { return &buffer; } //!< syntax helper

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            vbytes buffer; //!< actual data

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ovstream);
        };

        

    }
}

#endif

