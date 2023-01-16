
//! \file

#ifndef YACK_CHEMICAL_BOUNDARIES_INCLUDED
#define YACK_CHEMICAL_BOUNDARIES_INCLUDED 1

#include "yack/chem/reactor/balance/frontiers.hpp"

namespace yack {

    namespace chemical
    {


        //______________________________________________________________________
        //
        //
        //! assembly of frontiers, ranked
        //
        //______________________________________________________________________
        class boundaries
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            boundaries(const size_t m); //!< setup with capacities
            ~boundaries() throw();      //!< cleanup

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            frontiers limiting; //!< from positive concentration
            frontiers amending; //!< from negative concentration

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
        };


    }

}

#endif

