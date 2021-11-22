
//! \file

#ifndef YACK_STRING_FWD_INCLUDED
#define YACK_STRING_FWD_INCLUDED 1


namespace yack
{
    namespace kernel
    {
        template <typename T>
        class string;
    }

    typedef kernel::string<char> string;
}

#endif

