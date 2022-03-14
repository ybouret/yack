

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
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
            lib(std::cerr << "Cini=",C);
            for(size_t j=M;j>0;--j)
            {
                Ctry[j] = C[j];
            }

            const double rms = computeMissing(C);
            computeOmega0();
            std::cerr << "Omega = " << Omega0 << std::endl;

            return false;
        }


    }

}
