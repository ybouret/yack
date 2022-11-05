#include "yack/chem/outcome.hpp"
#include "yack/chem/eqs/lua.hpp"
#include "yack/chem/library.hpp"
#include "yack/utest/run.hpp"
#include "yack/system/env.hpp"

using namespace yack;
using namespace chemical;

YACK_UTEST(ham)
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

    const size_t   M = lib.size();
    vector<double> Corg(M,0);
    vector<double> Cend(M,0);
    vector<double> psi(M,0);
    vector<int>    nu(M,0);

    lib.fill(Corg,ran);
    std::cerr << "[[ Corg ]]" << std::endl;
    lib(std::cerr,"",Corg);
    std::cerr << std::endl;


    for(const enode *node=eqs.head();node;node=node->next)
    {
        const equilibrium &eq = ***node;
        const double       Ki = eq.K(0);
        const outcome      oc = outcome::study(eq, Ki, Corg, Cend, xmul, xadd);
        std::cerr << eq << std::endl;
        std::cerr << oc << std::endl;
        ios::ocstream fp("h-" + eq.name + ".dat");
        if(oc.state == components::are_blocked)
        {
            std::cerr << "\tblocked" << std::endl;
        }
        else
        {
            const double xi = *oc;
            if(fabs(xi)<=0)
            {
                std::cerr << "\tready" << std::endl;
            }
            else
            {
                eq.fill(nu);
                eq.grad_action(psi, Ki, Cend, xmul);
                const double sigma = xadd.dot(psi,nu);
                std::cerr << "\txi    = " << std::setw(15) << xi    << std::endl;
                std::cerr << "\tsigma = " << std::setw(15) << sigma << std::endl;
                if(sigma>=0) throw exception("bad sigma!!");

                const size_t NP = 50;
                for(size_t i=0;i<=NP;++i)
                {
                    const double u  = i /double(NP);
                    const double xx = xi * u;
                    const double hh = eq.mass_action(Ki,Corg,xx,xmul)/(-sigma);
                    fp("%.15g %.15g %.15g\n",xx,hh,xi-xx);
                }
            }

        }

        std::cerr << std::endl;
    }



}
YACK_UDONE()

