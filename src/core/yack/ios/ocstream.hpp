//! \file

#ifndef YACK_IOS_OCSTREAM_INCLUDED
#define YACK_IOS_OCSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/c/writable.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! output C stream
        //
        //______________________________________________________________________
        class ocstream : public ostream
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual ~ocstream() throw();                                      //!< cleanup
            explicit ocstream(const ios::cstderr_t &);                        //!< link to stderr
            explicit ocstream(const ios::cstdout_t &);                        //!< link to stdout
            explicit ocstream(const char *filename, const bool append=false); //!< open file

            //__________________________________________________________________
            //
            // ostrean interface
            //__________________________________________________________________
            virtual void write(const char C);
            virtual void frame(const void *,const size_t);
            virtual void operator()(const char *fmt,...);

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            static void overwrite(const char *filename);                  //!< overwrite file name
            static void echo(const char *filename, const char *fmt, ...); //!< append to filename


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ocstream);
            writable_file file;
        };
    }
}

#endif
