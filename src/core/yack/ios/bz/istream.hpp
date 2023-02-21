//! \file
#ifndef YACK_IOS_IBZSTREAM_INCLUDED
#define YACK_IOS_IBZSTREAM_INCLUDED 1

#include "yack/ios/istream.hpp"
#include "yack/ios/bz/stream.hpp"
#include "yack/ios/c/readable.hpp"

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
            class istream : public ios::istream,   public readable_file, public stream
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const char clid[]; //!< bz::istream

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________

                //! cleanup
                virtual ~istream() noexcept;

                //! setup
                template <typename INPUT> inline
                explicit istream(const INPUT &input) :
                ios::istream(), readable_file(input),
                stream( open_stream(handle) ),
                eos(false)
                {
                }




            private:
                YACK_DISABLE_COPY_AND_ASSIGN(istream);
                virtual bool   query_(char &C);
                virtual size_t fetch_(void *addr, const size_t size);
                bool           eos; //!< end of stream was met
                static void   *open_stream(void*);

            };
        }
    }
}

#endif
