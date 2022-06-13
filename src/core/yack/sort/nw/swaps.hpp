//! \file

#ifndef YACK_SORT_NWSWAPS_INCLUDED
#define YACK_SORT_NWSWAPS_INCLUDED

#include "yack/setup.hpp"

namespace yack
{
    namespace nwsrt
    {

        template <const size_t INPUTS>
        class swaps
        {
        public:
            const char * const name;
            const size_t       nswp;

            inline virtual ~swaps() throw() {}

        protected:
            explicit swaps(const char   *sid,
                           const size_t  num,
                           const size_t *lhs,
                           const size_t *rhs) throw();

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(swaps);
            const size_t * const LHS;
            const size_t * const RHS;
        };

    };
}

#endif

