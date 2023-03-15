
//! \file

#ifndef YACK_AQUEOUS_PRESERVED_INCLUDED
#define YACK_AQUEOUS_PRESERVED_INCLUDED 1

#include "yack/aqueous/realm/claw/conserved.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/sequence/cxx-array.hpp"

namespace yack
{
    namespace aqueous
    {

        class custodian
        {
        public:
            typedef solo_repo<const conservation> broken_list;
            typedef solo_list<double>             excess_list;
            typedef broken_list::node_type        broken_node;
            typedef excess_list::node_type        excess_node;

            explicit custodian(const size_t M); //!< setup for a given phase space
            virtual ~custodian() noexcept;

            void prepare() noexcept; //!< clear totalC  
            void process(writable<double> &C,
                         const conserved  &laws);

            cxx_array<double>             totalC; //!< [M] injected
            cxx_array<double>             deltaC; //!< [M] last increase
            broken_list                   broken;
            excess_list                   excess;
            cameo::add<double>            xadd;

        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
        };
    }

}

#endif

