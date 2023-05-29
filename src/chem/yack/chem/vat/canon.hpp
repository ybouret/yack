//! \file

#ifndef YACK_CHEM_VAT_CANON_INCLUDED
#define YACK_CHEM_VAT_CANON_INCLUDED 1

#include "yack/chem/vat/claws.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef core_repo<const ConservationLaw> LawRepo; //!< base class for Canon
        typedef LawRepo::node_type               LawNode; //!< alias

        //______________________________________________________________________
        //
        //
        //! Canon of dependents laws
        //
        //______________________________________________________________________
        class Canon : public object, public LawRepo
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Canon(const ConservationLaw &first); //!< initialize
            virtual ~Canon() noexcept;                    //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool endorses(const ConservationLaw &) const noexcept; //!< check if law is in the list
            bool linkedTo(const Canon           &) const noexcept; //!< check if shared law


            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Canon *next; //!< for list
            Canon *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Canon);
        };

        typedef cxx_list_of<Canon> Canons; //!< alias

    }

}

#endif
