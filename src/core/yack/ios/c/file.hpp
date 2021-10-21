

//! \file
#ifndef YACK_IOS_C_FILE_INCLUDED
#define YACK_IOS_C_FILE_INCLUDED 1

#include "yack/system/error.hpp"
#include "yack/setup.hpp"

namespace yack
{
    namespace ios
    {
        typedef int2type<0>   cstdin_t;
        extern const cstdin_t cstdin;

        typedef int2type<1>    cstdout_t;
        extern const cstdout_t cstdout;

        typedef int2type<2>    cstderr_t;
        extern const cstderr_t cstderr;

        class c_file
        {
        public:
            enum mode_type
            {
                r,
                w,
                a
            };
            static system_error_t last_close_error;
            virtual ~c_file() throw();


        protected:
            explicit c_file(const cstdin_t  &);
            explicit c_file(const cstdout_t &);
            explicit c_file(const cstderr_t &);
            explicit c_file(const char *, mode_type);

            void      *handle;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(c_file);
            const  bool   _close;
            static void * _open(const char *, mode_type);
        };

    }

}

#endif
