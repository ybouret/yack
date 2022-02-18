
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"

#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        void plexus:: evolve( writable<double> &C )
        {

            if(N<=0) return;

            vector<equilibrium*> evec(N,NULL);

            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            computeGammaAndPsi(Corg);
            if(regularize()>0 && verbose)
            {
                lib(std::cerr,Corg);
            }


            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = *eq;
                evec[i] = (equilibrium*)&eq;
                sc[i]   = eq.extent(K[i],Corg,Ctmp);
            }

            eqs(std::cerr,sc);




        }

    }

}

