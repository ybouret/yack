
//! \file

#ifndef YACK_CHEMICAL_CUSTODIAN_INCLUDED
#define YACK_CHEMICAL_CUSTODIAN_INCLUDED 1

#include "yack/chem/nexus.hpp"


namespace yack
{
    
    namespace chemical
    {
        //! will apply conservation laws
        class custodian
        {
        public:
            explicit custodian(const nexus &);
            virtual ~custodian() throw();
            
            const nexus &hub;
            tableau      xs;
            rmatrix      Cs;
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
        };
        
    }
}

#endif

