

//! \file

#ifndef YACK_CHEM_LIMITS_INCLUDED
#define YACK_CHEM_LIMITS_INCLUDED 1

#include "yack/chem/limit.hpp"

namespace yack
{
    namespace chemical
    {

        enum limited_type
        {
            limited_by_none,
            limited_by_reac,
            limited_by_prod,
            limited_by_both
        };

        class limits
        {
        public:
            limits(const limit *r, const limit *p) throw();

            friend std::ostream & operator<<(std::ostream &, const limits &);

            const limit * const reac;
            const limit * const prod;
            const limited_type  type;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limits);
            ~limits() throw();
        };

    }

}

#endif


