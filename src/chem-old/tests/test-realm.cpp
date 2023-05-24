
#include "yack/chem/weasel/designer.hpp"
#include "yack/chem/realm.hpp"
#include "yack/chem/realm/claw/custodian.hpp"
#include "yack/chem/realm/collector.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(realm)
{
    randomized::rand_ ran;
    species::verbose = true;
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
    realm chem(lib,eqs,K);

    const size_t   M = lib->size;
    vector<double> C(M,0);
    


    custodian keep(M);
    collector coll(chem.largest_domain_size(),M);


    for(size_t iter=0;iter<=100;++iter)
    {
        lib.conc(C,ran);
        for(size_t i=M;i>0;--i) if( ran.choice() ) C[i] = -C[i];
        lib(std::cerr << "C0=",C) << std::endl;
        keep.process(chem,C);
        coll.balance(chem,C);
        lib(std::cerr << "injected   = ", keep) << std::endl;
        lib(std::cerr << "suppressed = ", coll) << std::endl;
    }

    


}
YACK_UDONE()

