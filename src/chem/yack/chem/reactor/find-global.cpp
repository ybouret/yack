
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include <iomanip>
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {
        double reactor:: mixedHamiltonian(const group &g, writable<double> &C) throw()
        {
            assert(g.is_valid());
            assert(g.size>0);
            assert(g.is_ortho());

            //------------------------------------------------------------------
            //
            // load initial value
            //
            //------------------------------------------------------------------
            iota::save(C,Corg);
            for(const gnode *pp=g.head;pp;pp=pp->next)
            {
                //--------------------------------------------------------------
                //
                // transfer only changed components
                //
                //--------------------------------------------------------------
                const equilibrium &eq = **pp;
                eq.transfer(C,Cl[*eq]);
            }

            //------------------------------------------------------------------
            //
            // return newly formed state
            //
            //------------------------------------------------------------------
            return hamiltonian(C);
        }

        void   reactor:: betterMassAction(double &G0) throw()
        {
            assert( look_up.is_valid() );
            const group *gOpt = look_up->head;                // initialize group
            double       hOpt = mixedHamiltonian(*gOpt,Cend); // initialize @Cend

            std::cerr << " + " << std::setw(15) << hOpt << " @" << *gOpt << std::endl;


            for(const group *g=gOpt->next;g;g=g->next)
            {
                const double h = mixedHamiltonian(*g,Ctry);
                if(h<=hOpt)
                {
                    hOpt = h;
                    gOpt = g;
                    active.transfer(Cend,Ctry);
                    std::cerr << " + " << std::setw(15) << h << " @" << *g << std::endl;
                }
                else
                {
                    std::cerr << " - " << std::setw(15) << h << " @" << *g << std::endl;
                }
            }

            std::cerr << " * " << std::setw(15) << hOpt << " @" << *gOpt << std::endl;
            assert( fabs( hamiltonian(Cend) - hOpt) <=0 );
            G0 = hOpt;
        }

    }

}

