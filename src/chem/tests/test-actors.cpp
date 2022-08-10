
#include "yack/chem/library.hpp"
#include "yack/chem/actors.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace chemical;


YACK_UTEST(actors)
{
    
    library lib;
    lib << "H+:HO-";

    std::cerr << lib << std::endl;
    vector<double> C( lib.size() );

    C[1] = 1.2;
    C[2] = 1.3;
    const double Kw = 0.74;
    
    lib(std::cerr,"",C);
    
    actors reac;
    actors prod;
    prod( lib["H+"], 1 );
    prod( lib["HO-"],1 );

    std::cerr << "Reac: " << reac << std::endl;
    std::cerr << "Prod: " << prod << std::endl;

    rmulops ops;

    ops.free();
    ops.push(Kw);
    std::cerr << "Reac=" << reac.mass_action(C,ops) << std::endl;

    ops.free();
    std::cerr << "Prod=" << prod.mass_action(C,ops) << std::endl;
    
    const xlimit *rlim = reac.genuine_limit(C);
    if(rlim)
    {
        std::cerr << "rlim: " << rlim->xi << " by " << (*(rlim->id)).name << std::endl;
    }
    else
    {
        std::cerr << "no rlim" << std::endl;
    }
    
    const xlimit *plim = prod.genuine_limit(C);
    if(plim)
    {
        std::cerr << "plim: " << plim->xi << " by " << (*(plim->id)).name << std::endl;
    }
    else
    {
        std::cerr << "no plim" << std::endl;
    }



}
YACK_UDONE()

