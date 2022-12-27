
//! \file

#ifndef YACK_CHEMICAL_EQ_TIER_INCLUDED
#define YACK_CHEMICAL_EQ_TIER_INCLUDED 1

#include "yack/chem/eqs/team.hpp"
#include "yack/ptr/auto.hpp"

namespace yack
{
    namespace chemical
    {

        class eq_tier : public object
        {
        public:
            typedef auto_ptr<eq_tier> pointer;

            explicit eq_tier() throw();
            virtual ~eq_tier() throw();

            const eq_team roaming;
            const eq_team bounded;


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(eq_tier);
        };

    }

}

#endif

