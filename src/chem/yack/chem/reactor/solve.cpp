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

        bool reactor:: returnSolved(writable<double> &C0)
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

        bool reactor:: isTurnedOff(const group *g) const throw()
        {
            assert(g);
            for(const gnode *node=g->head;node;node=node->next)
            {
                if( blocked[ ***node ] ) return true;
            }
            return false;
        }

        const equilibrium * reactor:: setTopology(size_t &nrun, outcome &ppty)
        {
            nrun                    = 0;      // running eqs
            double             amax = 0;      // max absolute solving extent
            const equilibrium *emax = NULL;   // whose extent it is
            NuA.assign(Nu);                   // initial: full run

            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Ceq[ei];
                const double       Ki  = K[ei];
                const outcome      oc  = outcome::study(eq, Ki, Corg, Ci, xmul, xadd);
                writable<double>  &psi = Psi[ei];


                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        Xl[ei]      = 0;
                        sigma[ei]   = 0;
                        NuA[ei].ld(0);
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
                        eq.grad_action(psi,Ki,Ci,xmul);
                        sigma[ei] = xadd.dot(psi, Nu[ei]);
                        if(sigma[ei]>=0) throw imported::exception(clid,"corrupted <%s>",eq.name());
                    } break;
                }

                if(verbose) {
                    singles.pad(std::cerr << "| (+) " << '<' << eq.name << '>', eq) << " : " << oc << " @sigma= " << sigma[ei] << std::endl;
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
            // initialize depending on topology
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
                    return returnSolved(C0);
                } break;

                default:
                    // initialize consistent state
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                        dC[i]   = 0;
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
            YACK_CHEM_PRINTLN(fn << "[singles topology @level-1]");
            size_t              nrun = 0;
            outcome             ppty;
            const  equilibrium *emax = setTopology(nrun,ppty);

            if(!emax)
            {
                YACK_CHEM_PRINTLN(fn << "SUCCESS [fully solved]");
                return returnSolved(C0);
            }

            //------------------------------------------------------------------
            //
            //
            // check running dimensions
            //
            //
            //------------------------------------------------------------------
            assert(emax);
            YACK_CHEM_PRINTLN(fn << "found most displaced @" << emax->name);

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked @level-1]");
                    return returnSolved(C0);

                case 1:
                    YACK_CHEM_PRINTLN(fn << "is the only one running @level-1");
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
            YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (initial)");


            //------------------------------------------------------------------
            //
            //
            // look within pre-computed singles
            //
            //
            //------------------------------------------------------------------

            YACK_CHEM_PRINTLN(fn << "[looking for a minimum]");
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
                    if(ok) std::cerr << " <-- :)";
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
                            if(ok) std::cerr << " <-- :)";
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
                    //----------------------------------------------------------
                    // skip group with a blocked member
                    //----------------------------------------------------------
                    assert(g->is_ortho());
                    if(isTurnedOff(g))
                    {
                        YACK_CHEM_PRINTLN("    Htry: skipping " << *g);
                        continue;
                    }

                    //----------------------------------------------------------
                    // build a trial concentration
                    //----------------------------------------------------------
                    iota::load(Ctry,Corg);
                    for(const gnode *ep=g->head;ep;ep=ep->next)
                    {
                        const equilibrium &member = **ep;
                        member.transfer(Ctry,Ceq[*member]);
                    }

                    //----------------------------------------------------------
                    // compute trial value
                    //----------------------------------------------------------
                    const double Htry = Hamiltonian(Ctry);
                    const bool   ok   = (Htry<Hmin);
                    if(verbose)
                    {
                        std::cerr << "    Htry=" << std::setw(15) << Htry;
                        std::cerr << (ok? " [+]" : " [-]");
                        std::cerr << " @" << *g << std::endl;
                    }
                    if(ok)
                    {
                        Hmin = Htry;
                        working.transfer(Cend,Ctry);
                    }

                } while( NULL != ( g=solving.find_next(g,eq)  ) );

                //----------------------------------------------------------
                // full update @Corg
                //----------------------------------------------------------
                YACK_CHEM_PRINTLN( fn << " [moving at optimized]");
                working.transfer(Corg,Cend);
                if(verbose)
                {
                    corelib(std::cerr << "Cnew=","", Corg);
                }
                YACK_CHEM_PRINTLN(fn << "[singles topology @level-2]");
                emax = setTopology(nrun,ppty);
                switch(nrun)
                {
                    case 0:
                        YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked @level-2]");
                        return returnSolved(C0);

                    case 1:
                        YACK_CHEM_PRINTLN(fn << "is the only one running @level-2");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;
                }

                H0 = Hamiltonian(Corg);
                YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (updated)");
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

            //------------------------------------------------------------------
            //
            //
            // computing Omega
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << "[computing Omega]");
            bool maxDof = true;
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium      &eq  = ***node;
                const size_t            ei  = *eq;
                writable<double>       &Omi = Omega[ei];
                writable<double>       &psi = Psi[ei];
                if(blocked[ei])
                {
                    Omi.ld(0);
                    psi.ld(0);
                    Omi[ei]   = 1.0;
                    Gamma[ei] = 0.0;
                }
                else
                {
                    const double      Ki  = K[ei];
                    eq.grad_action(psi,Ki,Corg,xmul);
                    Gamma[ei] = fabs(Xl[ei])<=0 ? 0 : eq.mass_action(Ki,Corg,xmul);
                    for(const enode *scan=singles.head();scan;scan=scan->next)
                    {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,Nu[ej]);
                    }
                }
            }


            singles(std::cerr << "Omega=","",Omega);
            singles(std::cerr << "Gamma=","",Gamma);
            singles(std::cerr << "NuA  =","",NuA);
            std::cerr<< "Omega=" << Omega << std::endl;

            //------------------------------------------------------------------
            //
            //
            // computing xi
            //
            //
            //------------------------------------------------------------------

            iOmega.assign(Omega);
            if( !solv.build(iOmega,xadd) )
            {
                YACK_CHEM_PRINTLN(fn << "[singular Omega]");
                exit(1);
            }

            iota::neg(xi,Gamma);
            solv.solve(iOmega,xi);



            //------------------------------------------------------------------
            //
            //
            // checking primary extent
            //
            //
            //------------------------------------------------------------------
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium      &eq  = ***node;
                const size_t            ei  = *eq;
                const double            xx  = xi[ei];
                const xlimits          &lm  = eq.primary_limits(Corg,corelib.maxlen);
                const bool              ok  = lm.acceptable(xx);
                if(verbose)
                {
                    if(ok)
                    {
                        std::cerr << "correct";

                    }
                    else
                    {
                        std::cerr << "too big";

                    }
                    singles.pad(std::cerr << ' ' << eq.name,eq) << " @" << std::setw(15) << xx <<": ";
                    std::cerr << lm << std::endl;
                }

                if(!ok)
                {
                    maxDof = false;
                }

            }

            if(!maxDof)
            {
                std::cerr << "bad step..." << std::endl;
                exit(1);
            }

            //------------------------------------------------------------------
            //
            //
            // compute delta C and checking when dC<0
            //
            //
            //------------------------------------------------------------------
            vector<double> ratio;

            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                xadd.ldz();
                for(size_t i=N;i>0;--i)
                {
                    xadd.ld( NuA[i][j] * xi[i] );
                }
                const double d = (dC[j] = xadd.get());
                if(d<0)
                {
                    ratio << Corg[j]/(-d);
                }
            }

            corelib(std::cerr << "dC=", "", dC);
            double umax = 1;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                umax = min_of(umax,ratio.front());
                //umax = ratio.front();
            }
            std::cerr << "ratio=" << ratio << std::endl;
            
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                Cend[j] = max_of<double>(Corg[j]+dC[j],0);
            }
            
            {
                ios::ocstream fp("ham.dat");
                const size_t np=100;
                for(size_t i=0;i<=np;++i)
                {
                    const double u = (umax*i)/double(np);
                    fp("%.15g %.15g\n",u,(*this)(u));
                }
            }

            

            exit(0);

            return false;
            

        }

    }
}
