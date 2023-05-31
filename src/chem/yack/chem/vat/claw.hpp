//! \file

#ifndef YACK_CHEM_VAT_CLAW_INCLUDED
#define YACK_CHEM_VAT_CLAW_INCLUDED 1

#include "yack/chem/eqs/actors.hpp"
#include "yack/chem/com/xtypes.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace Chemical
    {



        //______________________________________________________________________
        //
        //
        //! Conservation Law as a list of Actors
        //
        //______________________________________________________________________
        class ConservationLaw : public Actors
        {
        public:
            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit ConservationLaw();          //!< setup
            virtual ~ConservationLaw() noexcept; //!< cleanup
            
            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________

            bool involves(const Species         &) const noexcept; //!< check if species is involved in conservation
            bool linkedTo(const ConservationLaw &) const noexcept; //!< check if a species is shared
            void finalize();                                       //!< compute Q/Q2

            //! simple display
            friend std::ostream & operator<<(std::ostream &, const ConservationLaw &);

            //! compute excess, must be process if positive
            Extended::Real excess(const readable<double> &C,
                                  Extended::Adder        &xadd,
                                  const IndexLevel        level) const;

            //__________________________________________________________________
            //
            // members
            //__________________________________________________________________
            const Extended::Real  Q2;   //!< sum nu^2
            ConservationLaw      *next; //!< for list
            ConservationLaw      *prev; //!< for list
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(ConservationLaw);
        };

      
    }

}

#endif
