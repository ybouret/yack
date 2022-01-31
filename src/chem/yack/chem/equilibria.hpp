
//! \file

#ifndef YACK_CHEM_EQS_INCLUDED
#define YACK_CHEM_EQS_INCLUDED 1

#include "yack/chem/equilibrium.hpp"

namespace yack
{

    namespace chemical
    {

        typedef suffix_set<string,const equilibrium::pointer> equilibria_;
        typedef equilibria_::knot_type  enode;

        class equilibria : public equilibria_
        {
        public:
            virtual ~equilibria() throw();
            explicit equilibria() throw();

            equilibrium & use(equilibrium *);

            template <typename NAME> inline
            equilibrium & operator()(const NAME &name, const double K)
            {
                return use( new const_equilibrium(name,K) );
            }

            const enode *head() const throw();


        private:
            YACK_DISABLE_COPY_AND_ASSIGN(equilibria);
        };

    }

}

#endif
