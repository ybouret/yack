
#include "yack/chem/reactor.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include <iomanip>
#include <cmath>

namespace yack
{
    using namespace math;

    namespace chemical
    {


        
        double  reactor:: aggregate(writable<double> &C, const group &g) throw()
        {
            assert(g.is_valid());
            assert(g.is_ortho());
            iota::save(C,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium &eq = **ep;
                eq.transfer(C,Cl[*eq]);
            }
            return hamiltonian(C);
        }

        bool reactor:: normalize(writable<double> &C0) throw()
        {

            //------------------------------------------------------------------
            //
            //
            // initialize
            //
            //
            //------------------------------------------------------------------
            assert(C0.size()>=M);
            YACK_CHEM_MARKUP(vpfx, "reactor::normalize");
            if(verbose) {
                lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);
            }

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

            double G0 = hamiltonian(Corg);

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("---------------- cycle=" << cycle << " ----------------");
            YACK_CHEM_PRINTLN("G0=" << G0);
            lib(std::cerr << "Corg=",Corg,"");

            if(G0<=0)
            {
                return returnSuccessful(C0,cycle);
            }

            double             AX = 0;
            double             G1 = G0;
            const equilibrium *ex = NULL;
            const equilibrium *eg = NULL;

            for(const enode *node=lattice.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       xx = Xl[ei] = eq.solve1D(Kl[ei], Corg, Cend);
                const double       ax = fabs( xx );
                if(ax>AX)
                {
                    AX =  ax;
                    ex = &eq;
                }
                triplet<double> u = { 0, -1, 1 };
                triplet<double> g = { G0, -1, hamiltonian(Cend) };
                optimize::run_for(*this,u,g,optimize::inside);
                active.transfer(Cl[ei],Ctry);
                lattice.pad(std::cerr << "@{" << eq.name << "}",eq) << " = " <<std::setw(15) << xx;
                std::cerr << " G=" << std::setw(15) << g.b << " @u=" << u.b;
                if(g.b<G1)
                {
                    G1=g.b;
                    eg=&eq;
                }
                std::cerr << std::endl;
            }


            YACK_CHEM_PRINTLN("|Xi| = " << AX << "@{" << (ex? ex->name() : "nil") << "}" );
            if(AX<=0)
            {
                assert(NULL==ex);
                return returnSuccessful(C0,cycle);
            }
            assert(NULL!=ex);

            YACK_CHEM_PRINTLN(" G1  = " << G1 << "@{" << (eg? eg->name() : "NIL") << "}" );
            if(!eg)
            {
                std::cerr << "stuck?" << std::endl;
                exit(1);
            }

            assert(NULL!=ex);
            assert(NULL!=eg);

            if(eg!=ex)
            {
                const equilibrium &eq = *eg;
                G0 = G1;
                eq.transfer(Corg,Cl[*eq]);
                goto CYCLE;
            }


            {
                const group *gOpt = look_up->head;
                double       hOpt = aggregate(Cend,*gOpt);

                for(const group *g=gOpt->next;g;g=g->next)
                {
                    const double h = aggregate(Ctry,*g);
                    if(h<=hOpt)
                    {
                        hOpt = h;
                        gOpt = g;
                        active.transfer(Cend,Ctry);
                    }
                }

                assert( fabs(hamiltonian(Cend)-hOpt)<=0  );
                std::cerr << "Gopt=" << hOpt << " for " << *gOpt << std::endl;
                assert(hOpt<G0);
                assert(hOpt<=G1);
                active.transfer(Corg,Cend);
                G0 = hOpt;
                goto CYCLE;
            }
            









            return false;
        }
    }


}
