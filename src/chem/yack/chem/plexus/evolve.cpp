
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

        

        void plexus:: evolve(writable<double> &C)
        {

            if(N<=0) return;

            //------------------------------------------------------------------
            //
            // fetch initial phase space
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }

            YACK_CHEM_PRINTLN("// <solving>");
            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }

            size_t iter=0;
        ITER:
            ++iter;
            W.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                const double       xx = xi[ei] = eq.extent(Ki,Corg,Ctmp);
                eq.drvs_action(Psi[ei],Ki, eq.move(Ctry,Corg,xx) );
                std::cerr << "Psi_" << eq.name << " = " << Psi[ei] << std::endl;
            }

            YACK_CHEM_PRINTLN("//   C0 = " << Corg);
            YACK_CHEM_PRINTLN("//   Psi= " << Psi);
            YACK_CHEM_PRINTLN("//   Xi = " << xi);
            if(verbose) eqs(std::cerr << "Xi=", xi);

            W.ld(0);
            for(size_t i=N;i>0;--i)
            {
                writable<double>       &W_i = W[i];
                const readable<double> &psi = Psi[i];
                W_i[i] = 1;
                const double fac = tao::v1::dot<double>::of(psi,Nu[i]);
                assert(fac<=0);
                if(fabs(fac)<=0)
                {
                    continue;
                }
                for(size_t j=N;j>i;--j)
                {
                    W_i[j] = tao::v1::dot<double>::of(psi,Nu[j]);
                }
                for(size_t j=i-1;j>0;--j)
                {
                    W_i[j] = tao::v1::dot<double>::of(psi,Nu[j]);
                }
            }
            YACK_CHEM_PRINTLN("//   Om = " << W);
            if(!LU.build(W)) throw exception("Singular Composition");

            LU.solve(W,xi);
            YACK_CHEM_PRINTLN("//     xi_p   = " << xi);

            YACK_CHEM_PRINTLN("//     <correcting>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const limits      &lm = eq.find_primary_limits(Corg);
                const size_t       ei = *eq;
                if(verbose)
                {
                    eqs.pad(std::cerr << "//       @" << eq.name,eq.name) << " : " << std::setw(14) << xi[ei] << " | " << lm << std::endl;
                }
                xi[ei] = lm.crop(xi[ei]);
            }
            YACK_CHEM_PRINTLN("//     <correcting/>");
            YACK_CHEM_PRINTLN("//     xi_c   = " << xi);

            computeDeltaC(xi);
            
            const double g0 = computeVariance(Corg);
            const double g1 = move(g0);

            std::cerr << "g1=" << g1 << " / " << g0 << std::endl;

            tao::v1::set(Corg,Ctry);

            tao::v1::set(C,Corg);

            if(iter>=4)
            {
                computeGammaAndPsi(Corg);
                return;
            }

            goto ITER;

        }




    }

}

