

//! \file
#ifndef YACK_IOS_C_WRITABLE_FILE_INCLUDED
#define YACK_IOS_C_WRITABLE_FILE_INCLUDED 1

#include "yack/ios/c/file.hpp"
#include "yack/check/printf.hpp"

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! write-only FILE
        //
        //______________________________________________________________________
        class writable_file : public c_file
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            virtual  ~writable_file() noexcept;                                  //!< cleanup
            explicit  writable_file(const cstderr_t &);                         //!< link to stderr
            explicit  writable_file(const cstdout_t &);                         //!< link to stdout
            explicit  writable_file(const char   *filename, const bool append); //!< open file
            explicit  writable_file(const string &filename, const bool append); //!< open file

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void   put(const char C);                          //!< put one char
            size_t put(const char *fmt,void *);                //!< helper to format
            size_t putf(const char *fmt,...) YACK_PRINTF_API;  //!< formatted output
            void   put(const void *addr,const size_t size);    //!< put one block

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(writable_file);
        };

    }

}

#endif

