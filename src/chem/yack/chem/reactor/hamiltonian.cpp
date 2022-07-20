
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include <cmath>
#include <iomanip>

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
                ratio.push_back_fast( squared(eq.mass_action(K[ei],C)/sigma[ei] ) );
            }

            assert(ratio.size()>0);
            return  sqrt(sorted::sum(ratio,sorted::by_value)/ratio.size());

        }


        double reactor:: Htry(const double G0) throw()
        {
            triplet<double> u = { 0,  -1 , 1 };
            triplet<double> g = { G0, -1, Hamiltonian(Cend)  };
            optimize::run_for( *this, u, g, optimize::inside );
            return g.b;
        }


        double reactor:: operator()(const double u) throw()
        {
            const double v = 1.0 - u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                double Cmin = Corg[j];
                double Cmax = Cend[j];
                if(Cmin>=Cmax) cswap(Cmin,Cmax);
                Ctry[j] = clamp(Cmin,v * Corg[j] + u * Cend[j],Cmax);
            }
            return Hamiltonian(Ctry);
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
            YACK_CHEM_PRINTLN(vpfx << " (#) G = " << std::setw(15) << hOpt  << " @" << *gOpt);

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
                YACK_CHEM_PRINTLN( vpfx << accepting(good) << "G = " << std::setw(15) << hTmp  << " @" << *gTmp );
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
            YACK_CHEM_PRINTLN(vpfx << " Gopt  = " << std::setw(15) << hOpt << " @" << *gOpt << " <--");
            active.transfer(Corg,Copt);
            return hOpt;
        }

    }
}


