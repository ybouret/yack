
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
        typedef vector<XReal,memory::dyadic> Injected;

        class Guardian : public Injected
        {
        public:
            typedef solo_repo<const ConservationLaw> Broken;
            typedef solo_list<XReal>                 Excess;
            typedef Excess::node_type                XsNode;
            
            explicit Guardian();
            virtual ~Guardian() noexcept;

            void restart(const size_t M);
            void enforce(const xmlog      &xml,
                         writable<double> &C0,
                         const       Act  &act);

            void operator()(const xmlog            &xml,
                            writable<double>       &C0,
                            const list_of<Cluster> &clusters);


            Broken broken;
            Excess excess;
            Adder  xadd;
            
        private:
            YACK_DISABLE_COPY_AND_ASSIGN(Guardian);
        };

    }

}

#endif

