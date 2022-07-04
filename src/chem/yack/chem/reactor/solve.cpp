
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"

#include <iomanip>
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        double reactor:: hamiltonian(const readable<double> &C) throw()
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtry,sorted::by_value);
        }

        bool reactor:: solve(writable<double> &C0) throw()
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
            // compute remaining couples
            //
            //------------------------------------------------------------------
            couplesXi();

            lookUp();


            return false;
        }

        double reactor:: combinedHamiltonian(const group &g, writable<double> &C) throw()
        {
            assert(g.is_valid());
            assert(g.size>0);
            assert(g.is_ortho());
            iota::save(C,Corg);
            for(const gnode *pp=g.head;pp;pp=pp->next)
            {
                const equilibrium &eq = **pp;
                eq.transfer(C,Cl[*eq]);
            }
            return hamiltonian(C);
        }


        void reactor:: lookUp() throw()
        {
            assert( look_up.is_valid() );

            for(const group *g=look_up->head;g;g=g->next)
            {
                const double hTmp = combinedHamiltonian(*g,Ctry);
                std::cerr << std::setw(15) << hTmp << " @" << *g << std::endl;
            }


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
