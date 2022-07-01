
//! \file

#ifndef YACK_CHEM_MIX_GROUPS_INCLUDED
#define YACK_CHEM_MIX_GROUPS_INCLUDED 1

#include "yack/chem/mix/group.hpp"

namespace yack
{
    namespace chemical
    {

        class groups : public group::list
        {
        public:
            virtual ~groups() throw();
            explicit groups() throw();

            explicit groups(const equilibria &eqs);
            void     sort();

            friend std::ostream & operator<<(std::ostream &os, const groups &);

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(groups);
        };

    }


}

#endif

