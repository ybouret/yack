//! \file

#ifndef YACK_IOS_OCSTREAM_INCLUDED
#define YACK_IOS_OCSTREAM_INCLUDED 1

#include "yack/ios/ostream.hpp"
#include "yack/ios/c/writable.hpp"

namespace yack
{
    namespace ios
    {
        class ocstream : public ostream
        {
        public:
            virtual ~ocstream() throw();
            explicit ocstream(const ios::cstderr_t &);
            explicit ocstream(const ios::cstdout_t &);
            explicit ocstream(const char *filename, const bool append=false);

            virtual void write(const char C);
            virtual void operator()(const char *fmt,...);
            
            static void overwrite(const char *filename);
            static void echo(const char *filename, const char *fmt, ...);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ocstream);
            writable_file file;
        };
    }
}

#endif
