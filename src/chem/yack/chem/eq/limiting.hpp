
//! \file

#ifndef YACK_CHEM_LIMITING_INCLUDED
#define YACK_CHEM_LIMITING_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{

    namespace chemical
    {
        enum limitation
        {
            limited_by_none,
            limited_by_reac,
            limited_by_prod,
            limited_by_both
        };
      
        //______________________________________________________________________
        //
        //
        //! limiting actor an its extent
        //
        //______________________________________________________________________
        class limiting
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________

            //! setup
            limiting(const actor *pa_, const double xi_) throw();


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const actor   * const pa; //!< actor address
            const double          xi; //!< extent
            

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(limiting);
            ~limiting() throw();
        };

    }

}

#endif

