
//! \file

#ifndef YACK_CHEMICAL_XLIMIT_INCLUDED
#define YACK_CHEMICAL_XLIMIT_INCLUDED 1

#include "yack/chem/species.hpp"

namespace yack
{
    namespace chemical
    {
        class actor;
        
        //______________________________________________________________________
        //
        //
        //! extent limit=actor+xi>=0
        //
        //______________________________________________________________________
        class xlimit
        {
        public:
            //! x>=0
            xlimit(const actor &, const double x) throw();
            
            const actor &id; //!< persistent actor
            const double xi; //!< computed extent
                             
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimit);
            ~xlimit() throw();
        };
    }
    
}

#endif

