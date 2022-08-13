
//! \file

#ifndef YACK_CHEMICAL_EQS_INCLUDED
#define YACK_CHEMICAL_EQS_INCLUDED 1

#include "yack/chem/eqs/constant.hpp"
#include "yack/chem/gathering.hpp"

namespace yack
{
    namespace chemical
    {
        
        typedef suffix_set<string,const equilibrium::pointer> eqs_db;
        typedef eqs_db::knot_type                              enode;
        
        
        class equilibria : public gathering
        {
        public:
            static const char clid[];
            
            explicit equilibria() throw();
            virtual ~equilibria() throw();
            equilibria(const equilibria &);
            
            size_t       size() const throw();
            const enode *head() const throw();
            
            template <typename NAME> inline
            equilibrium & operator()(const NAME  &uid,
                                     const double K)
            {
                return use( new const_equilibrium(uid,size()+1,K) );
            }
            
            
        private:
            YACK_DISABLE_ASSIGN(equilibria);
            eqs_db edb;
            
            equilibrium & use(equilibrium *eq);
            
            
        };
    }
}

#endif
