//! \file

#ifndef YACK_CHEM_VAT_CLAWS_INCLUDED
#define YACK_CHEM_VAT_CLAWS_INCLUDED 1

#include "yack/chem/vat/claw.hpp"

namespace yack
{
    namespace Chemical
    {
        //______________________________________________________________________
        //
        //
        //! base type for Laws
        //
        //______________________________________________________________________
        typedef cxx_list_of<ConservationLaw> ConservationLaws_;

        //______________________________________________________________________
        //
        //
        //! List of conservations laws within a cluster
        //
        //______________________________________________________________________
        class ConservationLaws : public ConservationLaws_, public Gathering
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ConservationLaws() noexcept; //!< setup empty
            virtual ~ConservationLaws() noexcept; //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void finalize(); //!< make names and update gathering

            //! nice display once finalized
            friend std::ostream & operator<<(std::ostream &, const ConservationLaws &);
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ConservationLaws);
        };

        
        
    }

}

#endif

