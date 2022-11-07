
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
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            xlimit(const actor &, const double x) throw(); //!< setup with x >= 0

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            const actor & operator*() const throw();       //!< get id

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actor &id; //!< persistent actor
            const double xi; //!< computed extent

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(xlimit);
            ~xlimit() throw();
        };
    }
    
}

#endif

