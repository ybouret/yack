
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace chemical;


static inline void try_solve(plexus &cs, writable<double> &C)
{
    cs.lib(std::cerr << "Cini=",C);
    cs.solve(C);
    cs.lib(std::cerr << "Cend=",C);
    std::cerr << "Gamma=" << cs.Gamma << std::endl;
}

YACK_UTEST(plexus)
{
    //concurrent::single::verbose = true;
    
    randomized::rand_ ran;

    library           lib;
    lua_equilibria    eqs;

    eqs->dostring("function K(t) return 1e-3*(1.1+math.cos(t)); end");

    for(int i=1;i<argc;++i)
    {
        eqs(argv[i],lib);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    plexus cs(lib,eqs);
    if( environment::get("YACK_CHEM_VERBOSE", NULL) )
        cs.verbose = true;

    cs.computeK(0);
    std::cerr << "K=" << cs.K << std::endl;

    if(cs.N)
    {
        vector<double> C(lib.size(),0);
        try
        {
            //----------------------------------------------------------------------
            // from 0
            //----------------------------------------------------------------------
            lib.fill(C,0.8,ran);
            try_solve(cs,C);

            //----------------------------------------------------------------------
            // loading active
            //----------------------------------------------------------------------
            vector<species *> spec;
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    spec.push_back( (species *)&s );
                }
            }
            YACK_CHECK(spec.size() == cs.A );


            //----------------------------------------------------------------------
            // just one not zero
            //----------------------------------------------------------------------
            for(size_t j=1;j<=spec.size();++j)
            {
                const species &s = *spec[j];
                std::cerr << "---------- With only " << s << std::endl;
                for(size_t iter=0;iter<10;++iter)
                {
                    C.ld(0);
                    C[s.indx] = species::concentration(ran);
                    try_solve(cs,C);
                }
                std::cerr << std::endl;
            }

            //----------------------------------------------------------------------
            // random
            //----------------------------------------------------------------------
            for(size_t iter=0;iter<10;++iter)
            {
                lib.fill(C,0.8,ran);
                try_solve(cs,C);
            }

        }
        catch(const exception &e)
        {
            e.show(std::cerr);
            std::cerr << "C=" << C << std::endl;
        }
    }


}
YACK_UDONE()
