
//! \file
#ifndef YACK_IOS_OBZSTREAM_INCLUDED
#define YACK_IOS_OBZSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/c/writable.hpp"
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
            //! output bzstream
            //
            //__________________________________________________________________
            class ostream : public ios::ostream, public writable_file, public stream
            {
            public:
                //______________________________________________________________
                //
                // types and definitions
                //______________________________________________________________
                static const char clid[];         //!< bz::ostream
                static const int default_level=6; //!< default bz2 level

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual ~ostream() throw();               //!< cleanup

                //! open
                template <typename OUTPUT> inline
                explicit ostream(const OUTPUT &output,
                                 const int  level=default_level,
                                 const bool append=false) :
                ios::ostream(),
                writable_file(output,append),
                stream( open_stream(handle,level) )
                {
                }

                //______________________________________________________________
                //
                // C++
                //______________________________________________________________
                virtual void write(char C);                     //!< output(&C,1)
                virtual void flush();                           //!< do nothing
                virtual void frame(const void *, const size_t); //!< output
                

            private:
                YACK_DISABLE_COPY_AND_ASSIGN(ostream);
                static void   *open_stream(void*,const int);
                void           output(const void *addr, const size_t size);
                
            };
        }
    }
}

#endif
