
//! \file

#ifndef YACK_AQUEOUS_CUSTODIAN_INCLUDED
#define YACK_AQUEOUS_CUSTODIAN_INCLUDED 1

#include "yack/aqueous/realm/claw/conserved.hpp"
#include "yack/data/dinky/solo-list.hpp"
#include "yack/data/dinky/solo-repo.hpp"
#include "yack/sequence/cxx-array.hpp"
#include "yack/ios/xmlog.hpp"

namespace yack
{
    namespace aqueous
    {
        class realm;
        class domain;
        
        //______________________________________________________________________
        //
        //
        //! custodian to apply laws
        //
        //______________________________________________________________________
        class custodian : public cxx_array<double>
        {
        public:
            //__________________________________________________________________
            //
            // definitions
            //__________________________________________________________________
            typedef solo_repo<const conservation> broken_list; //!< alias
            typedef solo_list<double>             excess_list; //!< alias
            typedef broken_list::node_type        broken_node; //!< alias
            typedef excess_list::node_type        excess_node; //!< alias

            //__________________________________________________________________
            //
            // C++
            //__________________________________________________________________
            explicit custodian(const size_t m); //!< setup for a given phase space
            virtual ~custodian() noexcept;      //!< cleanup

            //__________________________________________________________________
            //
            // methods
            //__________________________________________________________________
            void prepare() noexcept; //!< ld(0)

            
            //! process all the groups of laws for each domain of the realm
            void process(const realm      &chem,
                         writable<double> &conc);




        private:
            YACK_DISABLE_COPY_AND_ASSIGN(custodian);
            broken_list                   broken;
            excess_list                   excess;
            cameo::add<double>            xadd;

            //! process the different grouop of laws in a domain
            void process(const xmlog      &xml,
                         writable<double> &C,
                         const domain      &dom);

            //! process a group of laws by minimal displacement
            void process(const xmlog      &xml,
                         writable<double> &C,
                         const conserved  &laws);
        };
    }

}

#endif

