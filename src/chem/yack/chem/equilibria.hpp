
//! \file

#ifndef YACK_CHEM_EQS_INCLUDED
#define YACK_CHEM_EQS_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{

    namespace chemical
    {

        typedef suffix_set<string,const equilibrium::pointer> equilibria_;

        class equilibria : public equilibria_
        {
        public:
            virtual ~equilibria() throw();
            explicit equilibria() throw();

            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
        };

    }

}

#endif
