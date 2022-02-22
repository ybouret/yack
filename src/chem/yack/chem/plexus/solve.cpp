
#include "yack/container/for-each.hpp"
#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/numeric.hpp"
#include "yack/ios/fmt/align.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/indexing.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        
        void plexus:: solve(writable<double> &C)
        {

            switch(N)
            {
                case 0: return;
                case 1: {
                    const equilibrium &eq = ***(eqs.head());
                    eq.solve(K[1],C,Ctmp);
                    computeGammaAndPsi(C);
                } return;

                default:
                    break;
            }
            assert(N>=2);

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

            //------------------------------------------------------------------
            //
            // fetch equilibria
            //
            //------------------------------------------------------------------
            vector<equilibrium *> ev(N,NULL);
            vector<double>        kv(N,0);
            vector<size_t>        iv(N,0);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                ev[ei] = (equilibrium *)&eq;
                kv[ei] = K[ei];
                xs[ei] = eq.extent(kv[ei]=K[ei],Corg,Ctmp);
            }
            indexing::make(iv,comparison::decreasing<double>,kv);
            if(verbose)
            {
                std::cerr << "// <system>" << std::endl;
                for(size_t k=1;k<=N;++k)
                {
                    const size_t       i  = iv[k];
                    const equilibrium &eq = *ev[i];
                    eqs.pad(std::cerr << " @" << eq.name,eq);
                    std::cerr << " | K = " << std::setw(14) << kv[i];
                    std::cerr << " | Xs = " << std::setw(14) << xs[i];
                    std::cerr << " | Nu = " << Nu[i];
                    std::cerr << std::endl;
                }
                std::cerr << "// <system/>" << std::endl;
            }

            for(size_t k=1;k<=N;++k)
            {
                const size_t       itop = iv[k];
                const equilibrium &etop = *ev[itop];
                const size_t       row   = *etop;

                for(size_t l=N;l>k;--l)
                {
                    const size_t        isub  = iv[l];
                    const equilibrium  &esub = *ev[isub];
                    const size_t        col  = *esub;
                    const interactions &inter = conn[row][col];
                    std::cerr << etop.name << " / " << esub.name << " #" << inter.size << std::endl;
                    const double Ktop = K[row];
                    const double Ksub = K[col];
                    for(const interaction *in=inter.head;in;in=in->next)
                    {
                        const double Kin = pow( pow(Ktop,in->self) * pow(Ksub,in->peer), in->kexp );
                        coerce(*in).assign(Kin);
                        std::cerr << "  " << *in << std::endl;
                    }

                }

            }


            exit(1);



        }

    }

}


