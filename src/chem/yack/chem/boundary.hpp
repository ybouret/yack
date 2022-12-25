//! \file

#ifndef YACK_CHEM_BOUNDARY_INCUDED
#define YACK_CHEM_BOUNDARY_INCUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {

        class boundary : public sp_repo
        {
        public:
            double  xi;
            
            boundary(const sp_fund &) throw();
            ~boundary() throw();
            
            void free() throw();
            void add(const double   x,
                     const species &s);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundary);
        };
        
    }
    
}


#endif


