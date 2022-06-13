//! \file

#ifndef YACK_SORT_NWSWAPS_INCLUDED
#define YACK_SORT_NWSWAPS_INCLUDED

#include "yack/setup.hpp"

namespace yack
{
    namespace nwsrt
    {

        template <const size_t N>
        class swaps
        {
        public:
            const char * const name;
            
            inline virtual ~swaps() throw() {}


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps);
            const size_t * const LHS;
            const size_t * const RHS;
        };

    };
}

#endif

