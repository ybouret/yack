//! \file

#ifndef YACK_CHEM_VAT_CANON_INCLUDED
#define YACK_CHEM_VAT_CANON_INCLUDED 1

#include "yack/chem/vat/claws.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        // definitions
        //______________________________________________________________________
        typedef core_repo<const ConservationLaw> LawRepo; //!< base class for Act
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
            explicit Act(const ConservationLaw &first); //!< initialize with first law
            virtual ~Act() noexcept;                    //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            bool endorses(const ConservationLaw &) const noexcept; //!< check if law is in the list
            bool linkedTo(const Act             &) const noexcept; //!< check if there is a shared law
            void finalize() noexcept;                              //!< update Gathering for display

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            Act *next; //!< for list
            Act *prev; //!< for list

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Act);
        };

        //______________________________________________________________________
        //
        // definition
        //______________________________________________________________________
        typedef cxx_list_of<Act> Acts; //!< alias

    }

}

#endif
