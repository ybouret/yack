
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


            size_t iter=0;
        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//   iter=" << iter);
            if(verbose)
            {
                lib(std::cerr << "C0=", Corg);
            }
            if(iter>=100)
            {
                exit(1);
            }
            W.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                const double       xx = xi[ei] = eq.extent(Ki,Corg,Ctmp);
                eq.drvs_action(Psi[ei],Ki, eq.move(Ctry,Corg,xx) );
                std::cerr << "Ceq_" << eq.name << " = " << Ctry << std::endl;
                std::cerr << "Psi_" << eq.name << " = " << Psi[ei] << std::endl;
            }


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
                    xi[i] = 0;
                    continue;
                }
                for(size_t j=N;j>i;--j)
                {
                    W_i[j] = tao::v1::dot<double>::of(psi,Nu[j])/fac;
                }
                for(size_t j=i-1;j>0;--j)
                {
                    W_i[j] = tao::v1::dot<double>::of(psi,Nu[j])/fac;
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

            double       scale = 1.0;
            const size_t count = truncation(scale);
            if(count)
            {
                YACK_CHEM_PRINTLN("// [limited@" << scale << "]");
                if(scale>1.0)
                {
                    make_trial(1.0);
                }
                else
                {
                    make_trial(scale,ustack);
                    tao::v1::set(Corg,Ctry);
                    goto ITER;
                }
            }
            else
            {
                YACK_CHEM_PRINTLN("// [unlimited]");
                make_trial(1.0);
            }

            {
                bool converged = true;
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   dC = fabs(Ctry[j]-Corg[j]);
                    if(fabs(dC)>numeric<double>::ftol*fabs(Ctry[j]))
                    {
                        converged = false;
                    }
                    Corg[j] = Ctry[j];
                }
                if(converged)
                {
                    std::cerr << "converged@iter=" << iter << std::endl;
                    goto DONE;
                }
            }

            goto ITER;

        DONE:
            computeGammaAndPsi(Corg);
            tao::v1::set(C,Corg);

        }




    }

}

