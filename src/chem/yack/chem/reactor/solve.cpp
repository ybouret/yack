#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        bool reactor:: solved(writable<double> &C0)
        {
            working.tranfer(C0,Corg);
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
            YACK_CHEM_PRINTLN(fn << " [check singles]");
            double              amax = 0;
            size_t              nrun = 0;
            const  equilibrium *emax = NULL;
            outcome             ppty;

            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const double       Ki = K[ei];
                const outcome      oc = outcome::study(eq, Ki, Corg, Ci, xmul, xadd);
                writable<double>  &psi = Psi[ei];

                if(verbose) {
                    singles.pad(std::cerr << "\t (+) " << '<' << eq.name << '>', eq) << " : " << oc << std::endl;
                }

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
            }

            if(!emax)
            {
                YACK_CHEM_PRINTLN(fn << "SUCCESS [fully solved]");
                return solved(C0);
            }

            assert(emax);
            YACK_CHEM_PRINTLN(fn << "emax=" << emax->name);

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [all blocked]");
                    return solved(C0);

                case 1:
                    YACK_CHEM_PRINTLN(fn << "is the only one running...");
                    working.tranfer( Corg, Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }


            if(verbose)
            {
                singles(std::cerr,"blocked:",blocked);
                singles(std::cerr,"sigma:",sigma);
            }

            const double H0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN( fn << "H0=" << H0);
            const equilibrium *emin = NULL;
            double             Hmin = H0;

            // look within pre-computed singles
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;        if(blocked[ei]) continue;;
                writable<double>  &Ci = Ceq[ei];    working.tranfer(Cend,Ci);
                const double H1 = Optimized1D(H0);  working.tranfer(Ci,Ctry);
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
            }

            // look within couples
            for(const enode *node = couples.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const outcome      oc = outcome::study(eq, Kl[ei], Corg, Cend, xmul, xadd);
                switch(oc.state)
                {
                    case components::are_blocked:
                        Xl[ei] = 0;
                        working.tranfer(Ceq[ei],Corg);
                        break;

                    case components::are_running: {
                        Xl[ei] = oc.value;
                        const double H1 = Optimized1D(H0);working.tranfer(Ceq[ei],Ctry);
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
                if(verbose)
                {
                    const equilibrium &eq = *emin;
                    lattice.pad(std::cerr << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
                }
            }
            else
            {
                YACK_CHEM_PRINTLN(fn << "no global step");
            }




            return false;
        }

    }
}
