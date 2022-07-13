

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

        void   reactor:: zapEquilibriumAt(const size_t ei) throw()
        {
            writable<double> &Omi = Omega0[ei];
            blocked[ei] = true;
            sigma[ei]   = 0;
            Gamma[ei]   = 0;
            Omi.ld(0); Omi[ei] = 1.0;
            NuA[ei].ld(0);
        }

        const char * reactor::accepting(const bool flag) throw()
        {
            return flag ? " (+) " : " (-) ";
        }

        const equilibrium *reactor:: setupLattice(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::solve::setupLattice");

            double             Gmin = G0;
            const equilibrium *emin = NULL;

            //------------------------------------------------------------------
            //
            // setup from precomputed singles
            //
            //------------------------------------------------------------------
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


        void reactor:: updateOmega0() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::solve::updateOmega0");

            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium       &eq = ***node;
                const size_t             ei = *eq;
                const readable<double>  &psi = Psi[ei];
                writable<double>        &Omi = Omega0[ei];
                double                  &gam = Gamma[ei];

                if(blocked[ei])
                {
                    assert(fabs(sigma[ei])<=0);
                    gam = 0;
                    Omi.ld(0); Omi[ei]=1.0;
                    NuA[ei].ld(0);
                }
                else
                {
                    assert(sigma[ei]>0);
                    gam     = (fabs(Xl[ei]) <= 0) ? 0 : eq.mass_action(K[ei],Corg);
                    Omi[ei] = sigma[ei];
                    
                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t j = ****scan;
                        Omi[j] = - sorted::dot(psi,Nu[j],Ctry);
                    }

                }
            }

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
                    //----------------------------------------------------------
                    // empty
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN(" <success::empty/>");
                    return true;


                case 1:
                    //----------------------------------------------------------
                    // only 1
                    //----------------------------------------------------------
                {
                    const equilibrium &eq = ***singles.head();
                    eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(" <success::1D/>");
                } return returnSuccessful(C0,cycle);

                default:
                    //----------------------------------------------------------
                    // initialize for consistency
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }

            assert(N>0);


        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN(vpfx << "  ---------------- cycle " << cycle  << " ----------------");

            //------------------------------------------------------------------
            //
            //
            // global study
            //
            //
            //------------------------------------------------------------------

            //------------------------------------------------------------------
            //
            // setup singles for |Xi| and topology:
            // all the diagonal term (a.k.a sigma) are computed
            //
            //------------------------------------------------------------------
            size_t nrun = 0;
            if( setupSingles(nrun) ) return returnSuccessful(C0,cycle); // early return

            //------------------------------------------------------------------
            //
            // top-level hamiltonian
            //
            //------------------------------------------------------------------
            double G0 = Hamiltonian(Corg); YACK_CHEM_PRINTLN(vpfx << "    [initialized] G0 = " << G0);
            if(G0<=0) {
                YACK_CHEM_PRINTLN(vpfx << "  [success : G0 = 0 level-1]");
                return returnSuccessful(C0,cycle); // early return
            }

            //------------------------------------------------------------------
            //
            // global decrease
            //
            //------------------------------------------------------------------
            bool foundGlobalDecrease = false;
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
                    // recompute sigma for local step
                    //
                    //----------------------------------------------------------
                    if( setupSingles(nrun) ) return returnSuccessful(C0,cycle); // early return
                    foundGlobalDecrease = true;
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


            //------------------------------------------------------------------
            //
            //
            // Local Step
            //
            //
            //------------------------------------------------------------------
            
            {
                //--------------------------------------------------------------
                //
                // initialize local system
                //
                //--------------------------------------------------------------
                unsigned inner               = 0;     // inner counter
                bool     maximumAvailableDOF = true;  // start from maximum DoF
                updateOmega0();                       // nrun and diagonal are computed

            COMPUTE_EXTENT:
                ++inner;
                YACK_CHEM_PRINTLN(vpfx << "    -------- extent " << cycle << "." << inner << " --------");
                if(verbose) singles(std::cerr << vpfx << "sigma=",sigma,vpfx);

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

                assert(nrun>1);

                //--------------------------------------------------------------
                //
                // compute extent
                //
                //--------------------------------------------------------------
                iOmega.assign(Omega0);
                if(!LU->build(iOmega))
                {
                    YACK_CHEM_PRINTLN("  <singular local system>");
                    return false;
                }

                iota::load(xi,Gamma);
                LU->solve(iOmega,xi);

                //--------------------------------------------------------------
                //
                // validate primary constraints
                //
                //--------------------------------------------------------------
                {
                    bool overshoot = false;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        const double       xx = xi[ei];

                        if(verbose) singles.pad(std::cerr << "@{" << eq.name << "}",eq) << " : ";
                        if(blocked[ei])
                        {
                            if(verbose) std::cerr << "[blocked]";
                        }
                        else
                        {
                            const limits &lm = eq.primary_limits(Corg,lib.width);
                            if( lm.should_reduce(xx) )
                            {
                                overshoot = true;
                                --nrun;
                                YACK_CHEM_PRINT("[REJECT]");
                                zapEquilibriumAt(ei);
                            }
                            else
                            {
                                YACK_CHEM_PRINT("[accept]");
                            }
                            YACK_CHEM_PRINT( " " << std::setw(15) << xx << " | " << lm) ;
                        }
                        if(verbose) std::cerr << std::endl;
                    }

                    if(overshoot)
                    {
                        maximumAvailableDOF = false;
                        goto COMPUTE_EXTENT;
                    }
                }
                YACK_CHEM_PRINTLN(vpfx << "    [foundGlobalDecrease]=" << yack_boolean(foundGlobalDecrease) );
                YACK_CHEM_PRINTLN(vpfx << "    [maximumAvailableDOF]=" << yack_boolean(maximumAvailableDOF) );


                //--------------------------------------------------------------
                //
                // ready to probe better solution along the extent!
                //
                //--------------------------------------------------------------
                if( optimizeFullStep(G0) )
                {
                    //----------------------------------------------------------
                    //
                    // G0 has decreased => try again in any case
                    //
                    //----------------------------------------------------------
                    goto CYCLE;
                }
                else
                {
                    //----------------------------------------------------------
                    //
                    // G0 is stucked
                    //
                    //----------------------------------------------------------
                    if(!maximumAvailableDOF)
                    {
                        //------------------------------------------------------
                        // need to improve position
                        //------------------------------------------------------
                        goto CYCLE;
                    }
                    else
                    {
                        (void) returnSuccessful(C0,cycle);
                        std::cerr << std::endl << " *** CHECK!!! ***" << std::endl << std::endl;
                        exit(1);
                    }
                }

            }





            return false;
        }


    }

}


