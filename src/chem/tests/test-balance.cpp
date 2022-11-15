#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/binary.hpp"
#include "yack/math/iota.hpp"

using namespace yack;
using namespace chemical;

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

YACK_UTEST(balance)
{

    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_  ran;
    library            lib;
    luaEquilibria      eqs;
    rmulops            xmul;
    raddops            xadd;

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);


    }

    std::cerr << "[[ LIBRARY ]]" << std::endl;
    std::cerr << lib << std::endl;
    std::cerr << std::endl;

    reactor        cs(lib,eqs,0.0);
    const size_t   M  = cs.M;
#if 0
    const size_t   N  = cs.N;
    const imatrix &Nu = cs.Nu;
    const imatrix  NuT(cs.Nu,transposed);

    typedef vector<int> ivector;
    ivector         beta(M,0);
    ivector         nub(N,0);
    vector<ivector> all;
    const size_t    blk = sizeof(int) * N;

    if(M>0)
    {
        std::cerr << "Nu=" << Nu << std::endl;
        binary_spots sp(M);

        sp.next();
        do
        {
            sp.fill(beta);
            math::iota::mul(nub, Nu, beta);
            std::cerr << "beta=" << beta << " -> " << nub  << std::endl;
            const void *source = &nub[1];
            bool        found  = false;
            for(size_t i=1;i<=all.size();++i)
            {
                const void *target = &all[i][1];
                if( 0 == memcmp(target,source,blk) )
                {
                    found  = true;
                    break;
                }
            }
            if(!found)
            {
                all << nub;
            }
        }
        while(sp.next());

        std::cerr << "#all=" << all.size() << " / " << (1<<M) << std::endl;


        return 0;
    }
#endif
    

    vector<double> Corg(M,0);

    for(size_t iter=0;iter<1;++iter)
    {
        buildC(Corg,lib,ran);
        lib(std::cerr << "Corg=","",Corg);
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
YACK_UDONE()
