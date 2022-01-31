//! \file

#ifndef YACK_CHEM_PLEXUS_INCLUDED
#define YACK_CHEM_PLEXUS_INCLUDED 1

#include "yack/chem/equilibria.hpp"

namespace yack
{
    namespace chemical
    {
        
        class plexus
        {
        public:
            
            const library    &lib;
            const equilibria &eqs;
            
            virtual ~plexus() throw();
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(plexus);
        };
        
    }
}

#endif

