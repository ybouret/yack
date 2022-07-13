

#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/numeric.hpp"
#include <iomanip>
#include <cmath>


namespace yack
{
    using namespace math;

    namespace chemical
    {

        

        bool  reactor:: setupSingles(size_t &nrun) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::solve::setupSingles");
            double             Xmax = 0;
            const equilibrium *emax = NULL;
            nrun = 0;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                writable<double>  &Ci = Cl[ei];
                const double       xx = (Xl[ei]=eq.solve1D(Ki,Corg,Ci));
                const double       ax = fabs(xx);
                if(ax>Xmax)
                {
                    Xmax =  ax;
                    emax = &eq;
                }
                writable<double> &psi = Psi[ei];
                eq.drvs_action(psi,Ki,Ci,Ctry);
                const double den = - sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    blocked[ei] = true;
                    sigma[ei]   = 0.0;
                }
                else
                {
                    blocked[ei] = false;
                    sigma[ei]   = den;
                    ++nrun;
                }

                if(verbose)
                {
                    singles.pad(std::cerr << vpfx << "  @{" << eq.name << "}",eq);
                    std::cerr << " | xi = " << std::setw(15) << xx;
                    std::cerr << " | sigma = " << std::setw(15) << sigma[ei];
                    std::cerr << std::endl;
                }

            }

            if(!emax)
            {
                assert(Xmax<=0);
                YACK_CHEM_PRINTLN(vpfx << "  [success : |Xi| = 0 ]");
                return true;
            }
            else
            {
                YACK_CHEM_PRINTLN(vpfx << "  |Xi| = " << Xmax << " @{" << emax->name << "}" );
                return false;
            }
        }



        static inline const char *accepting(const bool flag) throw()
        {
            return flag ? " (+) " : " (-) ";
        }

        const equilibrium *reactor:: setupLattice(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::solve::setupLattice");

            double             Gmin = G0;
            const equilibrium *emin = NULL;

            // setup from precomputed singles
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq   = ***node;
                const size_t       ei   = *eq;
                writable<double>  &Ci   = Cl[ei];   iota::load(Cend,Ci);
                const double       Gtry = Htry(G0); iota::load(Ci,Ctry);
                const bool         good = Gtry<Gmin;
                if(good)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                YACK_CHEM_PRINTLN(vpfx << accepting(good) << std::setw(15) << Gmin << " @{" << eq.name << "}");

            }

            // setup from couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq   = ***node;
                const size_t       ei   = *eq;
                (void) eq.solve1D(Kl[ei],Corg,Cend);
                const double       Gtry = Htry(G0); iota::load(Cl[ei],Ctry);
                const bool         good = Gtry<Gmin;
                if(good)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                YACK_CHEM_PRINTLN(vpfx << accepting(good) << std::setw(15) << Gmin << " @{" << eq.name << "}");
            }

            return emin;
        }


        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::solve");
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);

            ios::ocstream::overwrite("g.dat");

            //------------------------------------------------------------------
            //
            //
            // checking cases
            //
            //
            //------------------------------------------------------------------
            unsigned cycle = 0;
            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN(" <success::empty/>");
                    return true;

                case 1: {
                    const equilibrium &eq = ***singles.head();
                    eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(" <success::1D/>");
                } return returnSuccessful(C0,cycle);

                default:
                    // initialize for consistency
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }

            assert(N>0);


        CYCLE:
            //------------------------------------------------------------------
            //
            //
            // global study
            //
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            //
            // setup singles for |Xi| and topology
            //
            //------------------------------------------------------------------
            size_t nrun = 0;
            if( setupSingles(nrun) ) return returnSuccessful(C0,cycle);

            //------------------------------------------------------------------
            //
            // top-level hamiltonian
            //
            //------------------------------------------------------------------
            double G0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN(vpfx << "    [initialized] G0 = " << G0);
            if(G0<=0) {
                YACK_CHEM_PRINTLN(vpfx << "  [success : G0 = 0 level-1]");
                return returnSuccessful(C0,cycle);
            }

            //------------------------------------------------------------------
            //
            // global decrease
            //
            //------------------------------------------------------------------
            {
                const equilibrium *emin = setupLattice(G0);
                if(emin)
                {
                    YACK_CHEM_PRINTLN(vpfx << "  <found global decrease @{" << emin->name << "} >");
                    //----------------------------------------------------------
                    //
                    // find optimized decrease
                    //
                    //----------------------------------------------------------
                    G0 = buildHamiltonian(*emin);
                    if(G0<=0) {
                        YACK_CHEM_PRINTLN(vpfx << "  [success : G0 = 0 level-2]");
                        return returnSuccessful(C0,cycle);
                    }

                    //----------------------------------------------------------
                    //
                    // recompute status for local step
                    //
                    //----------------------------------------------------------
                    if( setupSingles(nrun) ) return returnSuccessful(C0,cycle);
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // no better global, sigma is unchanged
                    //
                    //----------------------------------------------------------
                    assert(G0>0);
                    YACK_CHEM_PRINTLN(vpfx << "  <at global minimum>");
                }
            }

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(vpfx << "  [success : all-blocked]");
                    return returnSuccessful(C0,cycle);

                case 1:
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        if(!blocked[ei])
                        {
                            active.transfer(Corg,Cl[ei]);
                            goto CYCLE;
                        }
                    }
                    YACK_CHEM_PRINTLN(vpfx << "  [failure: corrupted-blocked]");
                    return false;

                default:
                    break;
            }

            singles(std::cerr << vpfx << "sigma=",sigma,vpfx);
            singles(std::cerr << vpfx << "blocked=",blocked,vpfx);





            return false;
        }


    }

}


