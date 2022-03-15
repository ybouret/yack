#include "yack/chem/plexus.hpp"
#include "yack/type/utils.hpp"
#include "yack/sort/sum.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{

    namespace chemical
    {

        double plexus:: computeMissing(const readable<double> &C) throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.missing>");
            const size_t M_ = M;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const double       Ki = K[ei];
                for(size_t j=M_;j>0;--j)  Ci[j] = C[j];
                xs[ei] = squared( Xi[ei] = eq.solve1D(Ki,C,Ci) );
                eq.drvs_action(Psi[ei],Ki,Ci,Ctmp);
                if(verbose)
                {
                    Gamma[ei] = eq.mass_action(Ki,Ci);
                    eqs.pad(std::cerr << "      @<" << eq.name << ">",eq) << " : " << std::setw(14) << Xi[ei] << std::endl;
                }
            }
            if(verbose)
            {
                eqs(std::cerr << "//    Gamma=",Gamma);
            }
            const double ans = sqrt( sorted::sum(xs,sorted::by_value)/N );
            YACK_CHEM_PRINTLN("//   <plexus.missing/> rms = " << ans );
            return ans;
        }

    }

}

