
//! \file

#ifndef YACK_CHEM_LIMIT_INCLUDED
#define YACK_CHEM_LIMIT_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{
    namespace chemical
    {

        //______________________________________________________________________
        //
        //
        //! limit from actors
        //
        //______________________________________________________________________
        class limit
        {
        public:
            limit(const actor &, const double) throw(); //!< setup
            const actor & operator*()    const throw(); //!< access
            const double xi;                            //!< host's extent >= 0
            
        private:
            const actor &host;
            YACK_DISABLE_COPY_AND_ASSIGN(limit);
            ~limit() throw();
        };

    }

}

#endif


