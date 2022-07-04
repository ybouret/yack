
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include <iomanip>
#include <cmath>

namespace yack
{

    namespace chemical
    {
        bool reactor:: solve(writable<double> &C0)
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::solve");
            if(verbose) {
                lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);
            }

            //------------------------------------------------------------------
            //
            // checking cases
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
                } return onSuccess(C0);

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = C0[i];
                    }
                    break;
            }


            //------------------------------------------------------------------
            //
            // compute singles and |Xi|
            //
            //------------------------------------------------------------------
            double AX = singlesXi();
            YACK_CHEM_PRINTLN(vpfx << "AX=" << AX);
            if(AX<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 />");
                return onSuccess(C0);
            }

            //------------------------------------------------------------------
            //
            // compute couples
            //
            //------------------------------------------------------------------
            couplesXi();
            

            return false;
        }


        double reactor:: singlesXi() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::singlesXi");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xl[ei] = Xtry[ei] = fabs( Xend[ei] = eq.solve1D(K[ei],Corg,Cl[ei]) );
            }
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xend,vpfx);
            return sorted::sum(Xtry,sorted::by_value);
        }

        void reactor:: couplesXi() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::couplesXi");
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xl[ei] = eq.solve1D(Kl[ei],Corg,Cl[ei]);
            }
            if(verbose) lattice(std::cerr << vpfx << "Xi=",Xl,vpfx);
        }

        bool reactor:: onSuccess(writable<double> &C0)
        {
            active.transfer(C0,Corg);
            if(verbose)
            {
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    singles.pad(std::cerr << eq.name,eq);
                    std::cerr << " | ma=" << std::setw(15) << eq.mass_action(K[*eq],C0);
                    std::cerr << " | xi=" << std::setw(15) << eq.solve1D(K[*eq],C0,Ctry);
                    std::cerr << std::endl;
                }
            }
            return true;
        }


    }

}
