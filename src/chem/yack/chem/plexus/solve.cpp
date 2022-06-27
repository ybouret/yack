#include "yack/chem/plexus.hpp"
#include "yack/chem/eqs/cross.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex/integer.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/optimize.hpp"
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        //----------------------------------------------------------------------
        //
        // hamiltonian of the system
        //
        //----------------------------------------------------------------------
        double plexus:: hamiltonian(const readable<double> &C)
        {
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtry[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtry,sorted::by_value)/N;
        }

        //----------------------------------------------------------------------
        //
        // hamiltonion( (1-u) * Corg + u * Cend )
        //
        //----------------------------------------------------------------------
        double plexus:: operator()(const double u)
        {
            const double v = 1.0-u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j] = v * Corg[j] + u * Cend[j];
            }
            return hamiltonian(Ctry);
        }

        double plexus:: optimizeGlobalExtent(const double G0, const equilibrium &eq) throw()
        {
            const size_t ei = *eq;
            const double ax = fabs( Xl[ei] = eq.solve1D(Kl[ei],Corg,Cend) );
            if(ax>0)
            {
                triplet<double> u = { 0,  -1, 1 };
                triplet<double> g = { G0, -1, hamiltonian(Cend) };

                lattice.pad(std::cerr << eq.name,eq) << " : " << g.a << " --> " << g.c << std::endl;

                iota::load(Cs[ei],Cend);


            }
            else
            {
                iota::load(Cs[ei],Cend);
            }
            return ax;
        }


        //----------------------------------------------------------------------
        //
        // extent of all possible reactions
        //
        //----------------------------------------------------------------------
        double plexus:: computeLatticeExtent()
        {
            const double G0 = hamiltonian(Corg);
            YACK_CHEM_PRINTLN(vpfx << " computeLatticeExtent@G0=" << G0);
            //------------------------------------------------------------------
            // summing |Xi| on singles
            //------------------------------------------------------------------
            double sumAbsXi = 0;
            for(const enode *node=singles.head();node;node=node->next)
            {
                sumAbsXi += optimizeGlobalExtent(G0,***node);
            }

            if(sumAbsXi<=0)
            {
                //--------------------------------------------------------------
                // cleaning up couples
                //--------------------------------------------------------------
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    Xl[ei] = 0;
                    iota::load(Cs[ei],Corg);
                }

            }
            else
            {
                //--------------------------------------------------------------
                // computing couples solution
                //--------------------------------------------------------------
                for(const enode *node=couples.head();node;node=node->next)
                {
                    //const equilibrium &eq = ***node;
                    //const size_t       ei = *eq;
                    //Xl[ei] = eq.solve1D(Kl[ei],Corg,Cs[ei]);
                    (void) optimizeGlobalExtent(G0,***node);
                }

            }

            if(verbose) lattice(std::cerr << vpfx << "Xi=", Xl, vpfx);

            return sumAbsXi;

        }

        bool plexus:: solve( writable<double> &C0 ) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "PlexusSolve");
            assert(C0.size()>=M);
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            //------------------------------------------------------------------
            //
            // initializing
            //
            //------------------------------------------------------------------
            switch(N)
            {
                    //----------------------------------------------------------
                case 0:   // no equilibrium
                    //----------------------------------------------------------
                    return true;

                    //----------------------------------------------------------
                case 1: { // special case
                    //----------------------------------------------------------

                    const equilibrium &eq = *** singles.head();
                    (void) eq.solve1D(K[*eq],C0,Corg);
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS_1D/>");
                } return true;

                    //----------------------------------------------------------
                default: // generic case, prepare consistent workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Cend[j] = Ctry[j] = C0[j];
                    }
            }

            //------------------------------------------------------------------
            //
            // check status of |Xi|
            //
            //------------------------------------------------------------------
            const double sumAbsXi = computeLatticeExtent();
            YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);

            

            


            return false;

        }

    }

}
