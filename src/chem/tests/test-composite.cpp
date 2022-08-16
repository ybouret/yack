
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/container/matrix.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(composite)
{
    randomized::rand_ ran;
    library           lib;
    luaEquilibria     eqs;
    rmulops           xmul;
    raddops           xadd;

    eqs->dostring("function f(t) return 1.1+math.cos(t) end");

    for(int i=1;i<argc;++i)
    {
        eqs(lib,argv[i]);
    }

    std::cerr << lib << std::endl;
    std::cerr << "singles=" << eqs << std::endl;


    const size_t    N = eqs.size();
    equilibria      couples;
    library         libcopy(lib);
    vector<double>  K(N,0);
    for(const enode *lhs=eqs.head();lhs;lhs=lhs->next)
    {
        const equilibrium &eq = ***lhs;
        K[*eq] = eq.K(0);
    }

    composite::scatter(couples,libcopy,eqs,K,xmul);

    equilibria lattice(eqs);
    lattice.add(couples);

    std::cerr << "lattice=" << lattice << std::endl;
    const size_t L = lattice.size();
    matrix<bool> conn(L,L);
    for(const enode *lhs=lattice.head();lhs;lhs=lhs->next)
    {
        const equilibrium &LHS = ***lhs;
        const size_t       L_i = *LHS;
        conn[L_i][L_i]         = true;
        for(const enode *rhs=lhs->next;rhs;rhs=rhs->next)
        {
            const equilibrium &RHS = ***rhs;
            const size_t       R_i = *RHS;
            conn[L_i][R_i] =  conn[R_i][L_i] = LHS.connected_to(RHS);
        }
    }
    lattice(std::cerr << "conn=","",conn);
    vector<double> KL(L,0);
    for(size_t i=1;i<=N;++i)
    {
        KL[i] = K[i];
    }
    for(const enode *node=couples.head();node;node=node->next)
    {
        const equilibrium &eq = ***node;
        KL[*eq] = eq.K(0);
    }
    std::cerr << "KL=" << KL << std::endl;

    const size_t   M = lib.size();
    vector<double> C(M,0);
    vector<double> S(M,0);

    lib.fill(C,ran);
    lib(std::cerr << "C=","",C);
    for(const enode *node=lattice.head();node;node=node->next)
    {
        const equilibrium &eq  = ***node;
        const outcome      res = outcome::study(eq,KL[*eq],C,S,xmul,xadd);
        lattice.pad(std::cerr << eq.name,eq) << " : " << res << std::endl;
    }

}
YACK_UDONE()

