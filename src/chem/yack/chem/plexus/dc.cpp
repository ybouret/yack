
#include "yack/chem/plexus.hpp"
#include "yack/sort/sum.hpp"
#include "yack/comparison.hpp"

namespace yack
{

    using namespace math;

    namespace chemical
    {

        void plexus:: computeDeltaC(const anode *excl)
        {
            YACK_CHEM_PRINTLN("//   <computeDeltaC>");
            dC.ld(0);
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species      &sp = ***node;
                const size_t        jj = *sp;
                const imatrix::row &nu = NuT[jj];
                for(size_t i=N;i>0;--i) sc[i] = nu[i] * xi[i];
                dC[jj] = sorted::sum(sc, sorted::by_abs_value);
            }
            while(excl)
            {
                (**excl)(dC) = 0;
                excl = excl->next;
            }

            YACK_CHEM_PRINTLN("  dC     = " << dC);
            YACK_CHEM_PRINTLN("   C     = " << Corg);
            YACK_CHEM_PRINTLN("//   <computeDeltaC/>");

        }

    }

}


