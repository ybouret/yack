
//! \file

#ifndef YACK_CHEM_LIMIT_INCLUDED
#define YACK_CHEM_LIMIT_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{
    namespace chemical
    {

        class limit
        {
        public:
            limit(const actor &, const double) throw();
            const double xi;

            const actor & operator*() const throw();

        private:
            const actor &host;
            YACK_DISABLE_COPY_AND_ASSIGN(limit);
            ~limit() throw();
        };

    }

}

#endif


