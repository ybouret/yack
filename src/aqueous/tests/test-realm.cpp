
#include "yack/aqueous/weasel/designer.hpp"
#include "yack/aqueous/realm.hpp"
#include "yack/aqueous/realm/claw/custodian.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace aqueous;

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
    realm cs(lib,eqs,K);

    return 0;

    const size_t   M = lib->size;
    vector<double> C(M,0);


    lib.conc(C,ran);
    for(size_t i=M;i>0;--i) if( ran.choice() ) C[i] = -C[i];

    lib(std::cerr << "C=",C) << std::endl;
    

    custodian        keeper(M);
    keeper.process(C,cs);



    YACK_SIZEOF(domain);
    YACK_SIZEOF(realm);
    YACK_SIZEOF(custodian);
    YACK_SIZEOF(conservation);
    YACK_SIZEOF(conserved);
    YACK_SIZEOF(preserved);
    YACK_SIZEOF(sp_list);
    YACK_SIZEOF(eq_list);


}
YACK_UDONE()
