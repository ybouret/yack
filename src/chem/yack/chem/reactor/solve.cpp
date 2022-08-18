#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"

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

        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor.solve] ";
            YACK_CHEM_PRINTLN(fn);

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


            return false;
        }

    }
}
