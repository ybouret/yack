//! \file

#ifndef YACK_RTTI_INCLUDED
#define YACK_RTTI_INCLUDED 1

#include "yack/setup.hpp"
#include "yack/string/fwd.hpp"
#include <typeinfo>

namespace yack
{

    class rtti
    {
    public:
        virtual ~rtti() throw();

    private:
        YACK_DISABLE_COPY_AND_ASSIGN(rtti);
    };

}

#endif

