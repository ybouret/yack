#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/binary.hpp"
#include "yack/math/iota.hpp"

using namespace yack;
using namespace chemical;
using namespace math;

static inline
void buildC(writable<double> &C, const library &lib, randomized::bits &ran)
{
    lib.fill(C,ran);
    for(size_t i=C.size();i>0;--i)
    {
        if( ran.choice() )
        {
            C[i] = -C[i];
        }
    }

}

#include "yack/ios/xmlog.hpp"

YACK_UTEST(balance)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;
    rmulops            xmul;
    raddops            xadd;
    const xmlog        xml("[test.balance]",std::cerr,entity::verbose);

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "[[ LIBRARY ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    reactor        cs(lib,eqs,0.0);


    const size_t   M  = cs.M;
    const size_t   N  = cs.N;
    const matrix<double>  NuT(cs.Nu,transposed);


    vector<double> Corg(M,0);
    vector<double> xi(N,0);

    if(true)
    {
        for(size_t iter=0;iter<10;++iter)
        {
            buildC(Corg,lib,ran);
            lib(std::cerr << "Corg=","",Corg);

            cs.apply_laws(Corg,xml);
            exit(0);



            if( cs.balance(Corg) )
            {
                lib(std::cerr << "Cbal=","",Corg);
            }
            else
            {
                std::cerr << "No Balance!!" << std::endl;
            }
            std::cerr << std::endl;
        }
    }

    std::cerr << std::endl;
    for(size_t i=N;i>0;--i)
    {
        xi[i] = lib.concentration(ran);
        if(ran.choice()) xi[i] = -xi[i];
    }
    std::cerr << "xi=" << xi << std::endl;

    Corg.ld(0);
    //lib.fill(Corg,ran);
    iota::muladd(Corg, NuT, xi, cs.xadd);
    lib(std::cerr << "Corg=","",Corg);
    cs.apply_laws(Corg,xml);

    exit(0);

    if( cs.balance(Corg) )
    {
        lib(std::cerr << "Cbal=","",Corg);
    }
    else
    {
        std::cerr << "<no balance>" << std::endl;
    }

    

}
YACK_UDONE()
