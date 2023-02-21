
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
            boundaries(const size_t m, const sp_fund &); //!< setup with capacities
            ~boundaries() noexcept;                       //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            //! probe limiting/amending extents
            /**
             \param C concentration
             \param A actors: reac/prod
             \param F shared fund
             \param B conserved flag
             \return true if amending was was
             */
            bool probe(const readable<double> &C,
                       const actors           &A,
                       const sp_fund          &F,
                       const readable<bool>   &B);
            
            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            frontier  limiting; //!< single limiting
            frontiers amending; //!< from negative concentration

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(boundaries);
        };


    }

}

#endif

