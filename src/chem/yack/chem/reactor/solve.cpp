
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/minimize.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: meanGammaSquared(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtmp[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtmp,sorted::by_value)/N;
        }

        double reactor:: operator()(const double u) throw()
        {
            assert(u>=0);
            assert(u<=1);
            const double omu = 1.0 - u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j]        = omu * Corg[j] + u * Cend[j];
            }
            return meanGammaSquared(Ctry);
        }

        double reactor:: optimizeDecreaseFrom(const double G0)
        {
            triplet<double> u = { 0, -1, 1 };
            triplet<double> g = { G0, -1, meanGammaSquared(Cend) };
            (void) minimize::find<double>::run_for(*this,u,g, minimize::inside);

            return g.b; //!< and Ctry is computed
        }

        double reactor:: selectDecreasedState()
        {
            // initialize
            const enode       *eOpt = eqs.head();
            double             gOpt = Gtot[****eOpt];

            // look up on single
            for(const enode *node=eOpt->next;node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            // look up on couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            // update
            const equilibrium &opt = ***eOpt;
            transfer(Corg,Ctot[*opt]);

            std::cerr << "Winner is " << opt.name << " : " << gOpt << "@" << Corg << std::endl;
            return gOpt;
        }

        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

#if 1
                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D success]");
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


            size_t cycle = 0;

            ++cycle;
            YACK_CHEM_PRINTLN("//   ---------------- cycle#" << cycle << " ----------------");

            //------------------------------------------------------------------
            //
            // starting point for this cycle
            //
            //------------------------------------------------------------------
            double G0 = meanGammaSquared(Corg);

            //------------------------------------------------------------------
            //
            // testing singles and |Xi| convergence
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "  [SINGLES]");
            {
                double sumAbsXi = 0;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;     assert(ei>0); assert(ei<=N);
                    const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                    if(ax>0)
                    {
                        sumAbsXi += ax;
                        Gtot[ei]  = optimizeDecreaseFrom(G0);
                        transfer(Ctot[ei],Ctry);
                        if(verbose)
                        {
                            eqs.pad(std::cerr << eq.name,eq) << " : Xi=" << std::setw(15) << Xtot[ei] << " : " << "Gopt=" << std::setw(15)<< Gtot[ei] << std::endl;
                        }
                    }
                    else
                    {
                        Gtot[ei] = G0;
                        transfer(Ctot[ei],Corg);
                    }
                }

                YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);

                if(sumAbsXi<=0)
                {
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
                    return true;
                }
            }

            //------------------------------------------------------------------
            //
            // testing couples
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "  [COUPLES]");
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;     assert(ei>N); assert(ei<=Ntot);
                const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                if(ax>0)
                {
                    Gtot[ei]  = optimizeDecreaseFrom(G0);
                    transfer(Ctot[ei],Ctry);
                    couples.pad(std::cerr << eq.name,eq) << " : Xi=" << std::setw(15) << Xtot[ei] << " : " << "Gopt=" << std::setw(15)<< Gtot[ei] << std::endl;
                }
                else
                {
                    Gtot[ei] = G0;
                    transfer(Ctot[ei],Corg);
                }
            }

            //------------------------------------------------------------------
            //
            // select best single/couple
            //
            //------------------------------------------------------------------
            G0 = selectDecreasedState();

            //------------------------------------------------------------------
            //
            // compute full metrics
            //
            //------------------------------------------------------------------

            coerce(NuTA).assign(NuT);
            size_t num_blocked = 0;
            size_t num_running = N;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega0[ei];
                const double       Ki  = K[ei];
                const double       ax  = fabs( Xtot[ei] = eq.solve1D(Ki,Corg,Ctmp) );

                Omi.ld(0);
                if(verbose)
                {
                    eqs.pad(std::cerr << vpfx << eq.name,eq) << " : Gamma=";
                }
                if(ax<=0)
                {
                    Gamma[ei] = 0;
                    eq.drvs_action(Psi[ei],Ki,Corg,Ctmp);
                }
                else
                {
                    Gamma[ei] = eq.grad_action(Psi[ei], Ki, Corg, Ctmp);
                }

                if(verbose)
                {
                    std::cerr << std::setw(15) << Gamma[ei];
                }

                const double diag = sorted::dot(psi,Nu[ei],Ctmp);
                if( diag >= 0)
                {
                    ++num_blocked;
                    --num_running;
                    std::cerr << " [blocked]" << std::endl;
                    Omi[ei]   = 1.0;
                    Gamma[ei] = 0.0;
                    psi.ld(0);
                }
                else
                {
                    std::cerr << " [running] " << psi;
                    Omi[ei] = diag;
                    for(size_t k=N;k>ei;--k)   Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                }

                if(verbose) std::cerr << std::endl;

            }
            std::cerr << "Omega=" << Omega0 << std::endl;


            // evaluate xi
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve> [SINGULAR]");
                return false;
            }




            exit(1);
            YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
            return true;
        }
    }

}


