#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        bool reactor:: solved(writable<double> &C0)
        {
            working.transfer(C0,Corg);
            if(verbose)
            {
                corelib(std::cerr << "Cend=", "", C0);
            }
            return true;
        }


        double reactor:: Hamiltonian(const readable<double> &C)
        {
            xadd.ldz();

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(blocked[ei]) continue;
                assert(sigma[ei]<0);
                xadd.ld( squared(eq.mass_action(K[ei],C,xmul)/sigma[ei]) );
            }

            return sqrt(xadd.get());
        }




        double reactor:: operator()(const double u1)
        {
            const double u0 = 1.0 - u1;
            for(const anode *node=working.head;node;node=node->next)
            {
                const size_t j = ***node;
                const double C0 = Corg[j];
                const double C1 = Cend[j];
                double       Cmin=C0,Cmax=C1;
                if(Cmax<Cmin) cswap(Cmin,Cmax);
                Ctry[j] = clamp(Cmin,C0*u0 + C1*u1,Cmax);
            }
            return Hamiltonian(Ctry);
        }

        double reactor:: Optimized1D(const double H0)
        {
            triplet<double> U = { 0,  -1, 1.0 };
            triplet<double> H = { H0, -1, Hamiltonian(Cend) };
            optimize::run_for(*this, U, H, optimize::inside);
            return H.b;
        }

        const equilibrium * reactor:: maxOfSingles(size_t &nrun, outcome &ppty)
        {
            nrun                    = 0;
            double             amax = 0;
            const equilibrium *emax = NULL;

            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const double       Ki = K[ei];
                const outcome      oc = outcome::study(eq, Ki, Corg, Ci, xmul, xadd);
                writable<double>  &psi = Psi[ei];



                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        Xl[ei]      = 0;
                        sigma[ei]   = 0;
                        break;

                    case components::are_running: {
                        ++nrun;
                        blocked[ei] = false;
                        const double ax = fabs( Xl[ei] = oc.value );
                        if(ax>amax)
                        {
                            amax =  ax;
                            emax = &eq;
                            ppty =  oc;
                        }
                        eq.drvs_action(psi,Ki,Ci,xmul);
                        sigma[ei] = xadd.dot(psi, Nu[ei]);
                        if(sigma[ei]>=0) throw imported::exception(clid,"corrupted <%s>",eq.name());
                    } break;
                }

                if(verbose) {
                    singles.pad(std::cerr << "\t (+) " << '<' << eq.name << '>', eq) << " : " << oc << " @sigma= " << sigma[ei] << std::endl;
                }

            }

            return emax;
        }

        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor.solve] ";
            YACK_CHEM_PRINTLN(fn);
            if(verbose)
            {
                corelib(std::cerr << "Cini=","", C0);
            }
            //------------------------------------------------------------------
            //
            // depending on topology
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [empty]");
                    return true;

                case 1: {
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [single]");
                    const equilibrium &eq = ***singles.head();
                    outcome::study(eq, K[1], C0, Corg, xmul, xadd);
                    return solved(C0);
                } break;

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                    }
            }


            unsigned cycle = 0;
        CYCLE:

            ++cycle;
            YACK_CHEM_PRINTLN(fn << "---------------- #cycle= " << std::setw(3) << cycle << " ----------------");

            //------------------------------------------------------------------
            //
            //
            // study singles, initialize phase space
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << " [check singles|level-1]");
            size_t              nrun = 0;
            outcome             ppty;
            const  equilibrium *emax = maxOfSingles(nrun,ppty);

            if(!emax)
            {
                YACK_CHEM_PRINTLN(fn << "SUCCESS [fully solved]");
                return solved(C0);
            }

            //------------------------------------------------------------------
            //
            //
            // check running dimensions
            //
            //
            //------------------------------------------------------------------
            assert(emax);
            YACK_CHEM_PRINTLN(fn << "emax=" << emax->name);

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked|level-1]");
                    return solved(C0);

                case 1:
                    YACK_CHEM_PRINTLN(fn << "is the only one running|level-1");
                    working.transfer( Corg, Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // Compute Hamiltonian @Corg
            //
            //
            //------------------------------------------------------------------
            double H0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN( fn << "H0=" << H0);


            //------------------------------------------------------------------
            //
            //
            // look within pre-computed singles
            //
            //
            //------------------------------------------------------------------
            const equilibrium *emin = NULL;
            double             Hmin = H0;

            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;              if(blocked[ei])  continue;
                writable<double>  &Ci = Ceq[ei];          working.transfer(Cend,Ci);
                const double       H1 = Optimized1D(H0);  working.transfer(Ci,Ctry);
                const bool         ok = (H1<Hmin);
                if(ok)
                {
                    Hmin = H1;
                    emin = &eq;
                }

                if(verbose)
                {
                    lattice.pad(std::cerr << "    @" << eq.name,eq) << " -> " << std::setw(15) << H1;
                    if(ok) std::cerr << " <--";
                    std::cerr << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // look within couples
            //
            //
            //------------------------------------------------------------------
            for(const enode *node = couples.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const outcome      oc = outcome::study(eq, Kl[ei], Corg, Cend, xmul, xadd);
                switch(oc.state)
                {
                    case components::are_blocked:
                        Xl[ei] = 0;
                        working.transfer(Ceq[ei],Corg);
                        break;

                    case components::are_running: {
                        Xl[ei] = oc.value;
                        const double H1 = Optimized1D(H0);working.transfer(Ceq[ei],Ctry);
                        const bool   ok = (H1<Hmin);
                        if(ok)
                        {
                            Hmin = H1;
                            emin = &eq;
                        }

                        if(verbose)
                        {
                            lattice.pad(std::cerr << "    @" << eq.name,eq) << " -> " << std::setw(15) << H1;
                            if(ok) std::cerr << " <--";
                            std::cerr << std::endl;
                        }
                    } break;
                }
            }

            if(emin)
            {
                //--------------------------------------------------------------
                //
                // found a global decrease
                //
                //---------------------------------------------------------------
                const equilibrium &eq = *emin;
                if(verbose)
                {
                    lattice.pad(std::cerr << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
                }

                //--------------------------------------------------------------
                //
                // initialize search for optimized combination
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN( fn << " [looking for a better combination]");
                const group *g = solving.find_first(eq); assert(g);
                iota::load(Cend,Ceq[*eq]);
                do {
                    assert(g->is_ortho());
                    iota::load(Ctry,Corg);
                    for(const gnode *ep=g->head;ep;ep=ep->next)
                    {
                        const equilibrium &member = **ep;
                        member.transfer(Ctry,Ceq[*member]);
                    }
                    const double Htry = Hamiltonian(Ctry);
                    const bool   ok   = (Htry<Hmin);
                    if(verbose)
                    {
                        std::cerr << "Htry=" << std::setw(15) << Htry;
                        std::cerr << (ok? " [+]" : " [-]");
                        std::cerr << " @" << *g << std::endl;
                    }
                    if(ok)
                    {
                        Hmin = Htry;
                        working.transfer(Cend,Ctry);
                    }

                } while( NULL != ( g=solving.find_next(g,eq)  ) );

                // update @Corg
                YACK_CHEM_PRINTLN( fn << " [moving at optimized]");
                working.transfer(Corg,Cend);
                if(verbose)
                {
                    corelib(std::cerr << "Cnew=","", Corg);
                }
                YACK_CHEM_PRINTLN(fn << " [check singles|level-2]");
                emax = maxOfSingles(nrun,ppty);
                switch(nrun)
                {
                    case 0:
                        YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked|level-2]");
                        return solved(C0);

                    case 1:
                        YACK_CHEM_PRINTLN(fn << "is the only one running|level-2");
                        working.transfer( Corg, Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;
                }

                H0 = Hamiltonian(Corg);
                YACK_CHEM_PRINTLN( fn << "H0=" << H0 << " (updated)");


            }
            else
            {
                //--------------------------------------------------------------
                //
                // stay @Corg, doesn't change H0
                //
                //------------------------------------------------------------------
                YACK_CHEM_PRINTLN(fn << "[no global step]");
                assert( fabs(H0-Hamiltonian(Corg))<=0 );
            }




            return false;
        }

    }
}
