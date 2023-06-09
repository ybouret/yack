
//! \file

#ifndef YACK_CHEM_VAT_TRIBE_INCLUDED
#define YACK_CHEM_VAT_TRIBE_INCLUDED 1

#include "yack/chem/equilibrium.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/container/matrix.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        // Definitions for Tribes
        //
        //______________________________________________________________________
        typedef Equilibrium::CoreRepo Tribe_;    //!< alias
        

        //______________________________________________________________________
        //
        //
        //! Tribe of independant equilibria combinations
        //
        //______________________________________________________________________
        class Tribe : public object, public Tribe_
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________

            explicit Tribe(const Equilibrium &first);  //!< setup with first
            virtual ~Tribe() noexcept;                 //!< cleanup
            Tribe(const Tribe &, const Equilibrium &); //!< copy + append

            //__________________________________________________________________
            //
            // method
            //__________________________________________________________________

            //! build library from equilibria
            void finalize();

            //! accepts if indep and index greater
            bool accepts(const Equilibrium &, const matrix<bool> &) const noexcept;
            YACK_PROTO_OSTREAM(Tribe); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Species::CoreRepo lib;
            Tribe                  *next; //!< for list
            Tribe                  *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribe);
        };


    }

}

#endif

