
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/realm.hpp"
#include "yack/aqueous/realm/claw/custodian.hpp"
#include "yack/aqueous/realm/collector.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;


static inline
void make(writable<double> &C,
          const actors     &plus,
          const actors     &minus,
          randomized::bits &ran)
{
    for(const actor *a=plus.head;a;a=a->next)
    {
        C[ (**a).indx[top_level] ] = library::conc(ran);
    }

    for(const actor *a=minus.head;a;a=a->next)
    {
        C[ (**a).indx[top_level] ] = -library::conc(ran);
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
            realm      chem(lib,eqs,K);
            collector  coll(chem.largest_domain_size(),M);

            vector<double> C(M,0);

            for(size_t iter=1;iter<=1;++iter)
            {
                make(C,eq.reac,eq.prod,ran);
                coll.probe(chem,C);
            }








        }
    }

#if 0
    library            lib;
    lua_equilibria     eqs;
    weasel::designer  &wd = weasel::designer::instance();



    for(int i=1;i<argc;++i)
    {
        wd(lib,eqs,jive::module::open_data(argv[i]));
    }

    std::cerr << "lib=" << std::endl << lib << std::endl;
    std::cerr << "eqs=" << std::endl << eqs << std::endl;

    vector<double> K;
    realm cs(lib,eqs,K);

    //cs.graphviz("cs");


    const size_t   M = lib->size;
    vector<double> C(M,0);
#endif

}
YACK_UDONE()
