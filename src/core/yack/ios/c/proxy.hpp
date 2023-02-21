//! \file

#ifndef YACK_IOS_C_PROXY_INCLUDED
#define YACK_IOS_C_PROXY_INCLUDED 1

#include "yack/setup.hpp"
#include <cstdio>

namespace yack
{
    namespace ios
    {
        //______________________________________________________________________
        //
        //
        //! proxy to access internal FILE for legacy code
        //
        //______________________________________________________________________
        template <typename BASE_CLASS>
        class file_proxy : public BASE_CLASS
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            //! for readable/writable
            template <typename T>
            inline explicit file_proxy(const T &args) : BASE_CLASS(args) {}

            //! for writable
            template <typename T>
            inline explicit file_proxy(const T &args, const bool flag) : BASE_CLASS(args,flag) {}

            //! cleanup
            inline virtual ~file_proxy() noexcept {}

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            inline FILE * operator*() noexcept { return (FILE *)(this->handle); } //!< access

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(file_proxy);
        };
    }
}

#endif

