

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        bool plexus:: inverseOmega0(const double factor) throw()
        {
            iOmega.assign(Omega0);
            for(size_t i=N;i>0;--i)
            {
                iOmega[i][i] *= factor;
            }
            return LU.build(iOmega);
        }

        bool plexus:: solve(writable<double> &C) throw()
        {
            assert(C.size()>=M);

            YACK_CHEM_PRINTLN("// <plexus.solve>");

            // initialize Xi, Ceq and Psi@Ceq
            computeExcess(C);

            // initialize Omega0
            computeOmega0();


            double factor = 1.0;
            if(!inverseOmega0(factor))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singular]");
                return false;
            }

            tao::v1::set(xi,Xi);
            LU.solve(iOmega,xi);

            // clamp xi
            correctExtent(C);

            computeDeltaC(C);


            return false;
        }


    }

}
