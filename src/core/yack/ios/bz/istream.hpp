//! \file
#ifndef YACK_IOS_IBZSTREAM_INCLUDED
#define YACK_IOS_IBZSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/ios/bz/stream.hpp"

namespace yack
{
    namespace ios
    {
        namespace bz
        {
            //__________________________________________________________________
            //
            //
            //! input bzstream
            //
            //__________________________________________________________________
            class istream : public ios::istream,   public stream
            {
            public:
                static const char clid[]; //!< bz::istream

                virtual ~istream() throw();               //!< cleanup
                explicit istream(const string &filename); //!< open file
                explicit istream(const char   *filename); //!< open file


            private:
                YACK_DISABLE_COPY_AND_ASSIGN(istream);
                virtual bool   query_(char &C);
                virtual size_t fetch_(void *addr, const size_t size);
                bool    eos;
            };
        }
    }
}

#endif
