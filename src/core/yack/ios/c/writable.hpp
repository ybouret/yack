

//! \file
#ifndef YACK_IOS_C_WRITABLE_FILE_INCLUDED
#define YACK_IOS_C_WRITABLE_FILE_INCLUDED 1

#include "yack/ios/c/file.hpp"
#include "yack/check/printf.hpp"

namespace yack
{
    namespace ios
    {
        class writable_file : public c_file
        {
        public:
            virtual  ~writable_file() throw();
            explicit  writable_file(const cstderr_t &);
            explicit  writable_file(const cstdout_t &);
            explicit  writable_file(const char *filename, const bool append);

            void   put(const char C);
            void   put(const char *fmt,void *);
            void   putf(const char *fmt,...) YACK_PRINTF_API;
            void   put(const void *addr,const size_t size);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(writable_file);
        };

    }

}

#endif

