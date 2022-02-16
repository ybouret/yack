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


        double  plexus::  computeVariance(const readable<double> &C)
        {
            assert(C.size()>=M);
            if(N>0)
            {
                computeGamma(C);
                for(size_t i=N;i>0;--i)
                {
                    sc[i] = squared(Gamma[i]);
                }
                return  (sorted::sum(sc,sorted::by_value)/N);
            }
            else
            {
                return 0;
            }

        }

        



        double plexus::operator()(const double u)
        {
            make_trial(u);
            return computeVariance(Ctry);
        }



        static inline char choice(const bool flag)
        {
            return flag?'+':'-';
        }

        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            if(N<=0) return;

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            for(size_t j=M;j>0;--j)
            {
                Corg[j] = Ctry[j] = C[j];
            }
            computeGammaAndPsi(Corg);
            size_t iter = 0;


            //------------------------------------------------------------------
            //
            //
            // Start iteration
            //
            //
            //------------------------------------------------------------------
        ITER:
            ++iter;
            YACK_CHEM_PRINTLN("//" << std::endl << "// <solving#" << iter << ">");
            //------------------------------------------------------------------
            //
            // regularizing by checking gradients
            //
            //------------------------------------------------------------------
            {
                const size_t moved = regularize();
                if(moved && verbose)
                {
                    YACK_CHEM_PRINTLN("// moved #" << moved);
                    if(blocked.size()>0) std::cerr << "// blocked=" << blocked << std::endl;
                }
            }
            if(verbose) lib(std::cerr<<"C0=",Corg);

            //------------------------------------------------------------------
            //
            // regularized variance |Gamma|@Corg
            //
            //------------------------------------------------------------------
            const double g0 = computeVariance(Corg);
            if( g0 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-1]");
                tao::v1::set(C,Corg);
                return;
            }

            //------------------------------------------------------------------
            //
            // compute Xi@Corg
            //
            //------------------------------------------------------------------
            computeXi();


            //------------------------------------------------------------------
            //
            // check   descent
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// [descent]");
            bool altered = false;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                const double       xx = xi[i];
                const double       gx = Gamma[i];
                const bool         ok = (gx*xx >= 0);
                YACK_CHEM_PRINTLN("//   @" << ios::align(eq.name,eqs.width)
                                  << " = " << std::setw(14) << xx
                                  << " | gamma = " << std::setw(14) << gx
                                  << " | " << choice(ok) );
                if(!ok)
                {
                    xi[i]   = 0;
                    altered = true;
                }
            }

            if(altered)
            {
                YACK_CHEM_PRINTLN("// [descent] altering extent");
                YACK_CHEM_PRINTLN("  xi     = " << xi);
            }
            else
            {
                YACK_CHEM_PRINTLN("// [descent] consistent composition");
            }



            //------------------------------------------------------------------
            //
            // computing and checking dC, rstack and ustack are rebuilt
            //
            //------------------------------------------------------------------
            computeDeltaC();

            //------------------------------------------------------------------
            //
            // move along dC
            //
            //------------------------------------------------------------------
            const double g1 = move(g0);

            if(altered)
            {
                std::cerr << "Was Altered..." << std::endl;
                const equilibrium *best = NULL;
                double             xmax = 0;
                for(const enode *node = eqs.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       i   = eq.indx;
                    sc[i] = eq.scale(K[i],Ctry,Ctmp);
                    eqs.pad(std::cerr << "$" << eq.name,eq.name);
                    std::cerr << " : " << sc[i];
                    std::cerr << std::endl;
                    const double x = fabs(sc[i]);
                    if(x>xmax)
                    {
                        best=&eq;
                        xmax=x;
                    }
                }
                if(best)
                {
                    lib(std::cerr << "Ctry=",Ctry);
                    best->solve(K[best->indx],Ctry,Ctmp);
                    lib(std::cerr << "Ctry=",Ctry);
                    computeGammaAndPsi(Ctry);
                    const double g2 = computeVariance(Ctry);
                    std::cerr << "g2=" << g2 << "/g1=" << g1 << std::endl;
                }
                exit(1);
            }

            //------------------------------------------------------------------
            //
            // check new values
            //
            //------------------------------------------------------------------
            //tao::v1::set(xi,Gamma);    // save old Gamma in xi
            computeGammaAndPsi(Ctry);  // new values
            const double dg = g1-g0;
            YACK_CHEM_PRINTLN("// g1=" << g1 << " (dg=" << dg << ") @Ctry=" << Ctry);


            if( g1 <= 0 )
            {
                YACK_CHEM_PRINTLN("// [numerical success level-2]");
                goto SUCCESS;
            }



            if( g1 >= g0 )
            {
                YACK_CHEM_PRINTLN("// [numerical minimum @" << g1 << "]");
                goto SUCCESS;
            }

#if 0
            static const double     gtol = numeric<double>::ftol;
            {
                bool converged = true;
                for(const enode *node = eqs.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       i   = eq.indx;
                    const double       gm  = Gamma[i];
                    const double       dg  = fabs(xi[i]-gm);
                    const double       err = gtol * fabs(gm);
                    const bool         bad = (dg > err);
                    if(verbose)
                    {
                        eqs.pad(std::cerr << "// (" << choice(!bad) << ") Gamma(" << eq.name << ")",eq.name) ;
                        std::cerr << " : " << std::setw(14) << dg;
                        std::cerr << " / " << std::setw(14) << err;
                        std::cerr << " @"  << gm;
                        std::cerr << std::endl;
                    }
                    if(bad)
                    {
                        converged = false;
                        if(!verbose) break;
                    }
                }
                if(converged)
                {
                    YACK_CHEM_PRINTLN("// [unchanged variance]");
                    goto SUCCESS;
                }
            }
#endif



            YACK_CHEM_PRINTLN("// [updating]");
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank<=0)
                {
                    assert(fabs(s(dC)<=0));
                    continue;
                }
                const size_t   j   = s.indx;
                const double   c   = Ctry[j]; assert(c>=0);
                const double   d   = fabs(c-Corg[j]);
                const double   err = fabs(numeric<double>::ftol * c);
                const bool     bad = (d>err);
                if(verbose)
                {
                    lib.pad(std::cerr << "// (" << choice(!bad) << ") d[" << s.name << "]",s.name) ;
                    std::cerr << " : " << std::setw(14) << d;
                    std::cerr << " / " << std::setw(14) << err;
                    std::cerr << " @"  << c;
                    std::cerr << std::endl;
                }
                Corg[j] = c;
            }

            if(iter>=100)
            {
                std::cerr << "Too Many..." << std::endl;
                exit(1);
            }
            goto ITER;

        SUCCESS:
            tao::v1::set(C,Ctry);
            return;

        }

    }

}
