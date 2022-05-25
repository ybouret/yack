

#include "yack/chem/plexus.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/indexing.hpp"

#include "yack/math/timings.hpp"
#include "yack/math/numeric.hpp"
#include "yack/counting/comb.hpp"


#include <cmath>
#include <iomanip>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        const char plexus::vpfx[] = "//   ";


        double plexus:: rmsGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            for(size_t i=N;i>0;--i)
            {
                xs[i] = squared(Gamma[i]*Gs[i]);
            }
            return sorted::sum(xs,sorted::by_value) / N;
        }

        double plexus:: operator()(const double u) throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t   j = ***node;
                Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
                //Ctry[j] = max_of( (1.0-u) * Corg[j] + u * Cend[j], 0.0 );
            }
            return rmsGamma(Ctry);
        }



        bool plexus:: solve(writable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            assert(are_valid(C0));

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);
            ios::ocstream::overwrite("rms.dat");
            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

#if 0
                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;
#endif

                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = Cend[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }

            //------------------------------------------------------------------
            //
            // regularize
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                double            &gam = Gamma[ei];
                gam = eq.grad_action(psi, Ki, Corg, Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    (void) eq.solve1D(Ki, Corg, Ctry);
                    transfer(Corg,Ctry);
                }
            }

            size_t cycle=0;
            //------------------------------------------------------------------
            //
            // compute state
            //
            //------------------------------------------------------------------
        CYCLE:
            ++cycle;
            std::cerr << "-------- cycle=" << cycle << " --------" << std::endl;
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);
            NuA.assign(Nu);
            NuAT.assign(NuT);
            size_t num_blocked = 0;
            double Xi2         = 0;
            Omega0.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Cei = Ceq[ei];
                writable<double>  &Omi = Omega0[ei];
                double            &gam = Gamma[ei];
                double            &xxx = Xi[ei];

                gam = eq.grad_action(psi,Ki,Corg,Ctmp);
                xxx = eq.solve1D(Ki,Corg,Cei);
                Xi2 += squared(xxx);
                const double Oii = sorted::dot(psi,Nu[ei],Ctmp);
                if(Oii>=0)
                {
                    Omi[ei]     = 1.0;
                    gam         = 0;
                    blocked[ei] = true;
                    Gs[ei]      = 0.0;
                    ++num_blocked;
                    NuA.ld_row(ei,0);
                    NuAT.ld_col(ei,0);
                }
                else
                {
                    blocked[ei]  = false;
                    Omi[ei]      = Oii;
                    Gs[ei]       = 1.0/(-Oii);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=N;k>ei;--k)   Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    if(fabs(xxx)<=0) gam = 0;
                }

            }


            if(Xi2<=0)
            {
                transfer(C0,Corg);
                goto SUCCESS;
            }

        EVAL_XI:
            // CHECK num blocked
            eqs(std::cerr << vpfx << "Gamma  =",  Gamma,vpfx);
            eqs(std::cerr << vpfx << "Psi    =",  Psi,  vpfx);
            eqs(std::cerr << vpfx << "Xi     =",     Xi,vpfx);
            eqs(std::cerr << vpfx << "Omega  =",  Omega0,vpfx);
            eqs(std::cerr << vpfx << "blocked=",blocked,vpfx);
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve> [SINGULAR]");
                return false;
            }

            tao::v1::neg(xi,Gamma);
            LU.solve(iOmega,xi);
            eqs(std::cerr << vpfx << "xi     =",     xi,vpfx);

            //------------------------------------------------------------------
            //
            // check xi
            //
            //------------------------------------------------------------------
            {
                bool recompute = false;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    double            &xx = xi[ei]; if(blocked[ei]) { xx = 0.0; continue; }
                    const limits      &lm = eq.primary_limits(Corg,lib.width);
                    eqs.pad(std::cerr << eq.name, eq) << " : " << lm << std::endl;
                    if(lm.should_reduce(xx))
                    {
                        std::cerr << "|_should reduce " << xx << std::endl;
                        recompute = true;
                        ++num_blocked;
                        writable<double> &Omi = Omega0[ei];
                        Omi.ld(0);
                        Omi[ei]     = 1.0;
                        Gamma[ei]   = 0;
                        blocked[ei] = true;
                        NuA.ld_row(ei,0);
                        NuAT.ld_col(ei,0);
                        Gs[ei]      = 0;
                    }
                }

                if(recompute)
                {
                    std::cerr << "Need To Recompute" << std::endl;
                    goto EVAL_XI;
                }
            }

            //------------------------------------------------------------------
            //
            // computing dC
            //
            //------------------------------------------------------------------
            {
                rstack.free();
                ustack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t   j = ***node;
                    const double   d = dC[j] = sorted::dot(xi,NuAT[j],xs);
                    const double   c = Corg[j]; assert(c>=0);
                    if(d<0)
                    {
                        rstack << c/(-d);
                        ustack << j;
                    }
                }

                std::cerr << "C =" << Corg  << std::endl;
                std::cerr << "dC=" << dC    << std::endl;

                double       expand = 2.0;
                const size_t ncut   = rstack.size();
                if(ncut>0)
                {
                    hsort(rstack,ustack, comparison::increasing<double> );
                    expand = min_of(expand,0.999*rstack[1]);
                }
                std::cerr << "rstack=" << rstack << std::endl;
                std::cerr << "ustack=" << ustack << std::endl;

                {
                    ios::ocstream fp("gam.dat");
                    const size_t NP=10000;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double u = expand*double(i)/NP;
                        fp("%g %g\n",u, (*this)(u) );
                    }
                }

                const double g0 = rmsGamma(Corg);
                triplet<double> u = { 0,  -1, expand };
                triplet<double> g = { g0, -1, (*this)(expand) };

                (void) minimize::find<double>::run_for(*this,u,g,minimize::inside);
                const double g1 = g.b;
                std::cerr << "g: " << g0 << " => " << g1 << " @" << u.b << std::endl;

                if(g1>=g0)
                {
                    std::cerr << "NO GAIN!!!" << std::endl;
                    exit(1);
                }
            }


            transfer(Corg,Ctry);

            if(cycle>=10)
                exit(1);

            goto CYCLE;

        SUCCESS:
            YACK_CHEM_PRINTLN("// <plexus.solve> [success]");
            return true;

        }



    }
}

