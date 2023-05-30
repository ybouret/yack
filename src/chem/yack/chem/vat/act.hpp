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
        //! Act of dependents laws
        //
        //______________________________________________________________________
        class Act : public object, public LawRepo, public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Act(const ConservationLaw &first); //!< initialize
            virtual ~Act() noexcept;                    //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool endorses(const ConservationLaw &) const noexcept; //!< check if law is in the list
            bool linkedTo(const Act             &) const noexcept; //!< check if shared law
            void finalize() noexcept;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Act *next; //!< for list
            Act *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Act);
        };

        typedef cxx_list_of<Act> Acts; //!< alias

    }

}

#endif
