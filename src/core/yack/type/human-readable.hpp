//! \file

#ifndef YACK_HUMAN_READABLE_INCLUDED
#define YACK_HUMAN_READABLE_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    class human_readable
    {
    public:
        
        const unsigned ip;
        const unsigned fp;
        const char     rx;

        human_readable(const uint64_t qw) throw();
        human_readable(const human_readable &) throw();
        ~human_readable() throw();
        human_readable & operator=(human_readable &) throw();

    };

}


#endif

