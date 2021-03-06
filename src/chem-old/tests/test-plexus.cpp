
#include "yack/chem/plexus.hpp"
#include "yack/chem/lua/equilibria.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/comb.hpp"

using namespace yack;
using namespace chemical;



static inline void try_solve(plexus &cs, writable<double> &C)
{
    
    cs.lib(std::cerr << "Cini=",C);
    cs.evolve(C);
    cs.lib(std::cerr << "Cend=",C);
    cs.eqs(std::cerr << "Gamma=",cs.Gamma);
}

YACK_UTEST(plexus)
{
//    YACK_CHEM_VERBOSE=1 ./build/chem/tests/Debug/test-chem plexus '@one:-2[a]-3[b]+4[c]:1' '@two:-2[a]+[b]:2'
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


    if( environment::flag("YACK_CHEM_VERBOSE") )
    {
        plexus::verbose = true;
        //math::minimize::verbose = true;
        //math::bracket::verbose  = true;
    }

    plexus cs(lib,eqs);

    cs.computeK(0);
    std::cerr << "K=" << cs.K << std::endl;

    cs.gv("plexus.dot");
    
    

    if(cs.N)
    {
        vector<double> C(lib.size(),0);
        try
        {
            //----------------------------------------------------------------------
            // from 0
            //----------------------------------------------------------------------
            lib.fill(C,0.7,ran);
            try_solve(cs,C);
            //std::cerr << "Nu=" << cs.Nu << std::endl;

            return 0;

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

            for(size_t na=1;na<=spec.size();++na)
            {
                combination       comb(spec.size(),na);
                vector<species *> sub(na,NULL);
                do
                {
                    std::cerr << "-- using";
                    for(size_t i=1;i<=na;++i)
                    {
                        sub[i] = spec[ comb[i] ];
                        std::cerr << " " << *sub[i];
                    }
                    std::cerr << std::endl;

                    for(size_t iter=0;iter<1;++iter)
                    {
                        C.ld(0);
                        for(size_t i=1;i<=na;++i)
                        {
                            const species &s = *sub[i];
                            const size_t   j = *s;
                            C[j] = species::concentration(ran);
                        }
                        try_solve(cs,C);
                    }

                } while( comb.next() );
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
