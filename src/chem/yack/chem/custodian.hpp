
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
            
            
            
            
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
        };
        
    }
}

#endif

