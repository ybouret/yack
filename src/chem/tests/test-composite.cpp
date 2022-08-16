
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/utest/run.hpp"
#include "yack/chem/eqs/composite.hpp"

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
    std::cerr << eqs << std::endl;

    composite::coeffs cof;
    for(const enode *lhs=eqs.head();lhs;lhs=lhs->next)
    {
        std::cerr << "<" << (***lhs).name << ">" << std::endl;
        for(const enode *rhs=lhs->next;rhs;rhs=rhs->next)
        {
            std::cerr << "\t<" << (***rhs).name << ">" << std::endl;
            const size_t nc = composite::collect(cof,***lhs,***rhs);
            std::cerr << "\t#" << nc << " : " << cof << std::endl;
        }
    }


}
YACK_UDONE()

