//! \file

#ifndef YACK_KERNEL_STRING_INCLUDED
#define YACK_KERNEL_STRING_INCLUDED 1

#include "yack/string/string_.hpp"

namespace yack
{

    namespace kernel
    {

        
        template <typename T>
        class string : public string_
        {
        public:
            virtual ~string() throw();

        };

    }

}

#endif

