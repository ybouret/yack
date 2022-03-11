

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

            for(size_t j=M;j>0;--j) Ctry[j] = C[j];
            size_t cycle = 0;

            ios::ocstream::overwrite("rms.dat");
        CYCLE:
            ++cycle;

            // initialize Xi, Ceq and Psi@Ceq
            const double rms = computeExcess(C);
            std::cerr << "// -------- RMS=" << rms << std::endl;
            ios::ocstream::echo("rms.dat","%g %g\n", double(cycle),rms);
            if(rms<=0)
            {
                YACK_CHEM_PRINTLN("// <plexus.solve> [success@cycle#" << cycle << "]");
                return true;
            }
            // initialize Omega0
            computeOmega0();
            std::cerr << "Omega=" << Omega0 << std::endl;

            double factor = 1.0;
            bool   scaled = false;

        EVAL_XI:
            if(!inverseOmega0(factor))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singular]");
                return false;
            }

            tao::v1::set(xi,Xi);
            LU.solve(iOmega,xi);

            // correct computed extent
            correctExtent(C);

            // compute dC
            if(!computeDeltaC(C))
            {
                assert(rstack.size());
                const double scale = rstack.front();
                assert(scale>0);
                assert(scale<=1);
                std::cerr << "scale=" << scale << std::endl;
                factor *= 10;
                scaled  = true;
                goto EVAL_XI;
            }

            // update C
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                C[j]  = (Ctry[j]=C[j]) + dC[j];
                const double d = fabs(C[j]-Ctry[j]);
                if(entity::verbose)
                {
                    lib.pad(std::cerr << "delta_" << s.name,s) << " : " << d << std::endl;
                }
            }

            if(cycle>=100)
            {
                exit(1);
            }
            goto CYCLE;

            return false;
        }


    }

}
