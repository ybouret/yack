
#include "yack/chem/lua/equilibria.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"
#include "yack/counting/comb.hpp"
#include "yack/apex.hpp"
#include "yack/type/utils.hpp"

using namespace yack;
using namespace chemical;


static inline
void try_combine(const string &aid, const readable<int> &a,
                 const string &bid, const readable<int> &b)
{
    YACK_ASSERT(a.size()==b.size());
    std::cerr << std::endl;
    std::cerr << aid << "=" << a << std::endl;
    std::cerr << bid << "=" << b << std::endl;
    const size_t M = a.size();
    for(size_t j=1;j<=M;++j)
    {
        const int A = a[j];
        const int B = b[j];
        if(A!=0 && B!=0)
        {
            apz mp_alpha = -B;
            apz mp_beta  =  A;
            apz::simplify(mp_alpha,mp_beta);
            //std::cerr << "\tmp.alpha = " << mp_alpha << std::endl;
            //std::cerr << "\tmp.beta  = " << mp_beta  << std::endl;

            int alpha = mp_alpha.cast_to<int>("alpha");
            int beta  = mp_beta.cast_to<int>("beta");
            std::cerr << "\talpha = " << alpha << std::endl;
            std::cerr << "\tbeta  = " << beta  << std::endl;
        }
    }


}

YACK_UTEST(comb)
{
    entity::verbose = environment::flag("VERBOSE");
    randomized::rand_ ran;
    library           lib;
    lua_equilibria    eqs;


    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << "lib=" << lib << std::endl;
    std::cerr << "eqs=" << eqs << std::endl;

    plexus cs(lib,eqs,0.0);
    std::cerr << "Nu=" << cs.Nu << std::endl;
    size_t nc = 0;
    for(size_t i=1;i<=cs.N;++i)
    {
        for(size_t k=i+1;k<=cs.N;++k)
        {
            ++nc;
            try_combine(eqs[i].name,cs.Nu[i],eqs[k].name,cs.Nu[k]);
        }
    }
}
YACK_UDONE()
