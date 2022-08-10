
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

    actors reac;
    actors prod;
    prod( lib["H+"], 1 );
    prod( lib["HO-"],1 );

    rmulops ops;

    ops.free();
    ops.push(Kw);
    std::cerr << "Reac=" << reac.mass_action(C,ops) << std::endl;

    ops.free();
    std::cerr << "Prod=" << prod.mass_action(C,ops) << std::endl;




}
YACK_UDONE()

