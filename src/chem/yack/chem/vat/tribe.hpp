
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
        typedef core_repo<const Equilibrium> Tribe_;    //!< alias
        typedef Tribe_::node_type            TribeNode; //!< alias


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

            //! accepts if indep and index greater
            bool accepts(const Equilibrium &, const matrix<bool> &) const noexcept;
            YACK_PROTO_OSTREAM(Tribe); //!< display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Tribe *next; //!< for list
            Tribe *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribe);
        };


        typedef cxx_list_of<Tribe> Tribes_;

        class Tribes : public Tribes_
        {
        public:
            explicit Tribes() noexcept;
            virtual ~Tribes() noexcept;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Tribes);
        };
    }

}

#endif

