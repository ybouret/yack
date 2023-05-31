
//! \file

#ifndef YACK_CHEM_VAT_GUARDIAN_INCLUDED
#define YACK_CHEM_VAT_GUARDIAN_INCLUDED 1

#include "yack/chem/vat/cluster.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/data/dinky/solo-list.hpp"

namespace yack
{
    namespace Chemical
    {
        typedef vector<XReal,memory::dyadic> Injected; //!< alias

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
            typedef solo_list<XReal>                 Excess; //!< alias
            typedef Excess::node_type                XsNode; //!< alias

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

            //! iterative laws enforcement
            void operator()(const xmlog            &xml,
                            writable<double>       &C0,
                            const list_of<Cluster> &clusters);



            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Guardian);
            Broken          broken;
            Excess          excess;
            Extended::Adder xadd;

            //! make this = zeros(M)
            void restart(const size_t M);

            //! iterative law enforcement, keeping track of injected
            void enforce(const xmlog      &xml,
                         writable<double> &C0,
                         const       Act  &act);
        };

    }

}

#endif

