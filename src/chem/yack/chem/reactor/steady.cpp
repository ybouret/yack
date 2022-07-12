
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


        double  reactor:: Hamiltonian(const readable<double> &C) throw()
        {
            ratio.free();
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(blocked[ei]) continue;

                assert(sigma[ei]>0);
                //ratio.push_back_fast( squared(eq.mass_action(K[ei],C)/sigma[ei] ) );
                ratio.push_back_fast( fabs(eq.mass_action(K[ei],C)/sigma[ei] ) );

            }

            assert(ratio.size()>0);
            return  (sorted::sum(ratio,sorted::by_value));
            //return sqrt(sorted::sum(ratio,sorted::by_value)/ratio.size());
        }

        double reactor:: Htry(const double G0) throw()
        {
            triplet<double> u = { 0,  -1 , 1 };
            triplet<double> g = { G0, -1, Hamiltonian(Cend)  };
            optimize::run_for( *this, u, g, optimize::inside );
            return g.b;
        }

        double  reactor:: mixedHamiltonian(writable<double> &C, const group &g) throw()
        {
            assert(g.is_valid());
            assert(g.is_ortho());
            assert(C.size()==Corg.size());
            iota::save(C,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium      &eq = **ep;
                const readable<double> &Ci = Cl[*eq]; assert(eq.other_are_unchanged(Ci,Corg));
                eq.transfer(C,Ci);
            }
            const double res = Hamiltonian(C);
            return res;
        }

        double  reactor:: buildHamiltonian(const equilibrium &eq) throw()
        {
            const group       *gOpt = look_up->find_first( eq ); assert(gOpt);
            tableau           &Copt = Cend;
            double             hOpt = mixedHamiltonian(Copt,*gOpt);
            YACK_CHEM_PRINTLN("(#) G = " << std::setw(15) << hOpt  << " @" << *gOpt);

            //----------------------------------------------------------
            //
            // look up in other groups containing equilibirium
            //
            //----------------------------------------------------------
            for(const group   *gTmp = gOpt->next;gTmp;gTmp=gTmp->next)
            {
                if(!gTmp->contains(eq)) continue;
                const double hTmp = mixedHamiltonian(Ctry,*gTmp);
                const bool   good = (hTmp<hOpt);
                YACK_CHEM_PRINTLN( (good?"(+)":"(-)") << " G = " << std::setw(15) << hTmp  << " @" << *gTmp );
                if(good)
                {
                    gOpt = gTmp;
                    hOpt = hTmp;
                    active.transfer(Copt,Ctry);
                }
            }

            //----------------------------------------------------------
            //
            // update current status
            //
            //----------------------------------------------------------
            YACK_CHEM_PRINTLN("Gopt  = " << std::setw(15) << hOpt << " @" << *gOpt);
            active.transfer(Corg,Copt);
            return hOpt;
        }


        bool    reactor:: steady(writable<double> &C0) throw()
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::computeSteady");
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);

            ios::ocstream::overwrite("g.dat");

            //------------------------------------------------------------------
            //
            //
            // checking cases
            //
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN(" <success::empty/>");
                    return true;

                case 1: {
                    const equilibrium &eq = ***singles.head();
                    eq.solve1D(K[*eq],C0,Corg);
                    YACK_CHEM_PRINTLN(" <success::1D/>");
                } return returnSuccessful(C0,0);

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                        Cstp[i] = 0;
                    }
                    break;
            }

            assert(N>0);

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            // initialize singles
            double             Xmax = 0;
            const equilibrium *emax = NULL;
            size_t             nrun = 0;

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = Kl[ei];
                writable<double>  &Ci  = Cl[ei];
                writable<double>  &psi = Psi[ei];
                const double       xx  = ( Xl[ei] = eq.solve1D(Ki,Corg,Ci) );
                const double       ax  = fabs(xx);

                if(ax>Xmax)
                {
                    Xmax = ax;
                    emax = &eq;
                }

                eq.drvs_action(psi,Ki,Ci,Ctry);
                const double den = -sorted::dot(psi,Nu[ei],Ctry);
                if(den<=0)
                {
                    sigma[ei]   = 0.0;
                    blocked[ei] = true;
                }
                else
                {
                    sigma[ei]   = den;
                    blocked[ei] = false;
                    ++nrun;
                }
            }

            singles(std::cerr << vpfx << "Xi=",Xl,vpfx);
            std::cerr << vpfx << "|Xmax|=" << Xmax;
            if(emax)
            {
                assert(Xmax>0);
                std::cerr << " @" << emax->name;
            }
            std::cerr << std::endl;
            singles(std::cerr << vpfx << "sigma=",sigma,vpfx);

            if(Xmax<=0)
            {
                YACK_CHEM_PRINTLN("  <success:: |Xi| = 0/>");
                return returnSuccessful(C0,cycle);
            }


            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN("  <success::all-blocked/>");
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
                    YACK_CHEM_PRINTLN("  <failure::corrupted-1D>");
                    return false;
            }

            double G0 = Hamiltonian(Corg);
            if(G0<=0)
            {
                YACK_CHEM_PRINTLN("  <success:: Hamiltonian=0/>");
                return returnSuccessful(C0,cycle);
            }

            ios::ocstream::echo("g.dat", "%u %.15g\n",cycle,log10(G0));
            YACK_CHEM_PRINTLN(vpfx << " G0 = " << G0);

            double             Gmin = G0;
            const equilibrium *emin = NULL;

            // minimize singles
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Cl[ei];
                iota::load(Cend,Ci);
                const double Gtry = Htry(G0);
                if(Gtry<Gmin)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                iota::load(Ci,Ctry);

                lattice.pad(std::cerr << vpfx << "@" << eq.name,eq) << " : " << std::setw(15) << Gtry << std::endl;

            }

            // minimize couples
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                eq.solve1D(Kl[ei],Corg,Cend);
                const double Gtry = Htry(G0);
                if(Gtry<Gmin)
                {
                    Gmin = Gtry;
                    emin = &eq;
                }
                iota::load(Cl[ei],Ctry);
                lattice.pad(std::cerr << vpfx << "@" << eq.name,eq) << " : " << std::setw(15) << Gtry << std::endl;
            }



            if(emin)
            {
                std::cerr << "Gmin=" << Gmin << " @" << emin->name << std::endl;
                std::cerr << "Hmin=" << Hamiltonian(Cl[**emin]) << std::endl;
                assert(Gmin<G0);
                const double G1 = buildHamiltonian(*emin);
                assert(G1<=Gmin);
                G0 = G1;
                std::cerr << "DELTA=" << G0-G1 << std::endl;
                lib(std::cerr << vpfx << "Cnew=",Corg,vpfx);

                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    lattice.pad(std::cerr << "@{" << eq.name << "}",eq);
                    const double ma = eq.mass_action(K[ei],Corg);
                    std::cerr << ": ma = " <<  std::setw(15) << ma;
                    const double sig = sigma[ei];
                    std::cerr << " / " <<  std::setw(15) << sig;
                    if(!blocked[ei])
                        std::cerr << " => +" << fabs(ma/sig);
                    std::cerr << std::endl;
                }

                if(G0<=0)
                {
                    YACK_CHEM_PRINTLN("  <success:: Hamiltonian=0/>");
                    return returnSuccessful(C0,cycle);
                }


                ios::ocstream::echo("g.dat", "%u.5 %.15g\n",cycle,log10(G0));

                goto CYCLE;



                // update singles
                Xmax = 0;
                nrun = 0;
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    const double       Ki  = Kl[ei];
                    writable<double>  &Ci  = Cl[ei];
                    writable<double>  &psi = Psi[ei];
                    const double       xx  = ( Xl[ei] = eq.solve1D(Ki,Corg,Ci) );
                    const double       ax  = fabs(xx);

                    if(ax>Xmax)
                    {
                        Xmax = ax;
                    }

                    eq.drvs_action(psi,Ki,Ci,Ctry);
                    const double den = -sorted::dot(psi,Nu[ei],Ctry);
                    if(den<=0)
                    {
                        sigma[ei]   = 0.0;
                        blocked[ei] = true;
                    }
                    else
                    {
                        sigma[ei]   = den;
                        blocked[ei] = false;
                        ++nrun;
                    }
                }
                singles(std::cerr << vpfx << "sigma=",sigma,vpfx);


            }
            else
            {
                exit(1);
            }




            return false;
        }

    }

}

