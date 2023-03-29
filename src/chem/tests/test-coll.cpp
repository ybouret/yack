
#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/realm.hpp"
#include "yack/chem/realm/claw/custodian.hpp"
#include "yack/chem/realm/collector.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


static inline
void make(writable<double> &C,
          const actors     &plus,
          const actors     &minus,
          randomized::bits &ran)
{
    for(const actor *a=plus.head;a;a=a->next)
    {
        C[ (**a).indx[top_level] ] = ran.to<double>();
    }

    for(const actor *a=minus.head;a;a=a->next)
    {
        C[ (**a).indx[top_level] ] = -ran.to<double>();
    }

}

YACK_UTEST(coll)
{
    randomized::rand_ ran;
    species::verbose = true;

    for(size_t nr=1;nr<=3;++nr)
    {
        for(size_t np=1;np<=3;++np)
        {
            library      lib;
            equilibria   eqs;
            equilibrium &eq = eqs( new const_equilibrium("eq",0.1,1) );
            char         id = 'a'-1;

            for(size_t i=1;i<=nr;++i)
            {
                const species &s = lib(++id,0);
                eq( -int(ran.in(1,3)), s);
            }

            for(size_t i=1;i<=np;++i)
            {
                const species &s = lib(++id,0);
                eq( int(ran.in(1,3)), s);
            }

            const size_t M = lib->size;
            eq.display(std::cerr) << std::endl;;
            vector<double> K;
            realm          chem(lib,eqs,K);
            collector      coll(chem.largest_domain_size(),M);
            vector<double> C(M,0);
            custodian      cust(M);

            for(size_t iter=1;iter<=10;++iter)
            {
                make(C,eq.reac,eq.prod,ran);
                cust.process(chem,C);
                coll.balance(chem,C);

                make(C,eq.prod,eq.reac,ran);
                cust.process(chem,C);
                coll.balance(chem,C);
            }

        }
    }


}
YACK_UDONE()
