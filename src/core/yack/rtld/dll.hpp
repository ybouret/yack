#ifndef YACK_DLL_INCLUDED
#define YACK_DLL_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{
    class dll
    {
    public:
        explicit dll(const char *soname);
        virtual ~dll() throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(dll);
        void  *handle;
    };

}

#endif

