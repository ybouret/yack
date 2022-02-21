
#include "yack/chem/plexus.hpp"
#include "yack/sort/sum.hpp"


namespace yack
{
    
    namespace chemical
    {

        void plexus:: computeDeltaC(const readable<double> &xx) throw()
        {
            YACK_CHEM_PRINTLN("//" << std::endl <<  "//   <computeDeltaC>");
            dC.ld(0);
            for(const anode *node=active.head;node;node=node->next)
            {
                const species      &s = **node;
                const size_t        j = *s;
                const imatrix::row &nu = NuT[j];
                rstack.free();
                for(size_t i=N;i>0;--i)
                {
                    const double x = xx[i];
                    const int    n = nu[i];
                    if(n)
                        rstack.push_back_fast(n*x);
                }
                dC[j] = sorted::sum(rstack,sorted::by_value);
            }
            if(verbose)
            {
                lib(std::cerr << "dC=", dC);
                std::cerr << "//" << std::endl <<  "//   <computeDeltaC>" << std::endl;
            }

        }

    }
}
