
//! \file

#ifndef YACK_COUNTING_COMB_INCLUDED
#define YACK_COUNTING_COMB_INCLUDED 1

#include "yack/setup.hpp"

namespace yack
{

    namespace nubbin
    {
        struct combination
        {
            size_t n;
            size_t k;
            size_t nmk;
            void  init(writable<size_t> &comb) throw();
            void  next(writable<size_t> &comb) throw();
        };
    }

}

#endif
