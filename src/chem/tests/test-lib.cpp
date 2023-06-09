#include "yack/chem/library.hpp"
#include "yack/chem/equilibria.hpp"
#include "yack/chem/eqs/const.hpp"

#include "yack/utest/run.hpp"

using namespace yack;
using namespace Chemical;

YACK_UTEST(lib)
{

    YACK_SIZEOF(Species);
    YACK_SIZEOF(Library);
    YACK_SIZEOF(Actor);
    YACK_SIZEOF(Actors);
    YACK_SIZEOF(Component);
    YACK_SIZEOF(Components);
    YACK_SIZEOF(Equilibrium);
    YACK_SIZEOF(Equilibria);


    Library lib;

    const Species   &proton  = lib("H+",1);
    const Species   &hydroxy = lib("HO-",-1);
    const Species   &AH      = lib("AH",0);
    const Species   &Am      = lib("Am",-1);

    std::cerr << lib->size << std::endl;
    std::cerr << proton    << std::endl;
    std::cerr << hydroxy   << std::endl;

    for(const sNode *sn = lib->head; sn; sn=sn->next)
    {
        const Species &sp = ***sn;
        lib.pad(std::cerr << sp,sp) << " : z=" << sp.z << std::endl;
    }

    {
        Actors A;
        for(unsigned i=1;i<=2;++i)
        {
            for(unsigned j=1;j<=2;++j)
            {
                A.release();
                A.push_back( new Actor(i,proton)  );
                A.push_back( new Actor(j,hydroxy) );
                A.makeName();
                std::cerr << "output: " << A << std::endl;
                std::cerr << "name  : " << A.name << std::endl;
            }
        }
    }

    {
        Components water;
        water(1,proton);
        water(1,hydroxy);
        std::cerr << "#water=" << water->size << std::endl;
        water.display(std::cerr) << std::endl;
    }

    Equilibria   eqs;
    {
        Equilibrium &water = eqs( new ConstEquilibrium("water") );
        water(1,proton);
        water(1,hydroxy);
    }
    {
        Equilibrium &weak = eqs( new ConstEquilibrium("weak") );
        weak(1,proton);
        weak(-1,AH);
        weak(1,Am);
    }

    eqs.finalize();

    std::cerr << eqs << std::endl;

    
}
YACK_UDONE()

