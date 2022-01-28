
//! \file

#ifndef YACK_CHEM_LIMITING_INCLUDED
#define YACK_CHEM_LIMITING_INCLUDED 1

#include "yack/chem/actor.hpp"

namespace yack
{

    namespace chemical
    {
      
        //______________________________________________________________________
        //
        //
        //! limiting actor
        //
        //______________________________________________________________________
        class limiting
        {
        public:
            //__________________________________________________________________
            //
            // types and definitions
            //__________________________________________________________________
            
            //! classification
            enum status
            {
                by_none, //!< no limits
                by_reac, //!< limited by reactant
                by_prod, //!< limited by product
                by_both  //!< limited by both sides
            };

            //! helper
            static const char *text(const status s) throw();

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

