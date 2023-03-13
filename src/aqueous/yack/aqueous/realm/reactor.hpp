
//! \file

#ifndef YACK_AQUEOUS_REACTOR_INCLUDED
#define YACK_AQUEOUS_REACTOR_INCLUDED 1

#include "yack/aqueous/equilibria.hpp"

namespace yack
{
    namespace aqueous
    {

        class reactor : public large_object, public eq_list
        {
        public:

            
            reactor *next;
            reactor *prev;
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(reactor);
        };

    }

}

#endif

