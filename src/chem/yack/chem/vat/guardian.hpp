//! \file

#ifndef YACK_CHEM_VAT_GUARDIAN_INCLUDED
#define YACK_CHEM_VAT_GUARDIAN_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/solo-repo.hpp"

namespace yack
{
    namespace Chemical
    {
        //typedef vector<Extended::Real,memory::dyadic> Injected; //!< base class for Guardian

        typedef vector<Extended::AdderPtr,memory::dyadic> Injected; //!< base class for Guardian


        //----------------------------------------------------------------------
        //
        //
        //! operating on consrvation laws to enforce them
        //
        //----------------------------------------------------------------------
        class Guardian : public Injected
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef solo_repo<const ConservationLaw> Broken; //!< alias
            typedef Extended::SoloList               Excess; //!< alias
            typedef Extended::Node                   XsNode; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit Guardian();           //!< setup
            virtual ~Guardian() noexcept;  //!< cleanup


            //__________________________________________________________________
            //
            // mehods
            //__________________________________________________________________


            //! make this = zeros(M)
            void restart(const size_t M);


            //! iterative law enforcement, keeping track of injected
            /**
             \param xml  to display info
             \param Corg [TopLevel] concentrations, converted
             \param act  set of dependent conservation laws
             */
            void enforce(const xmlog              &xml,
                         writable<Extended::Real> &Corg,
                         const       Act          &act);

            //! iterative law enforcement on all acts of cluster
            void enforce(const xmlog              &xml,
                         writable<Extended::Real> &Corg,
                         const Cluster            &cluster);


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Guardian);
            Broken          broken;
            Excess          excess;
            Extended::Adder xadd;


        };

    }

}

#endif

