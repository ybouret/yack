
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
            assert(C.size()==Corg.size());
            iota::save(C,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium      &eq = **ep;
                const readable<double> &Ci = Cl[*eq]; assert(eq.other_are_unchanged(Ci,Corg));
                eq.transfer(C,Ci);
            }
            const double res = hamiltonian(C);
            return res;
        }



        
        bool    reactor:: normalize(writable<double> &C0) throw()
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
            if(verbose) lib(std::cerr<<vpfx<<"Cini=",C0,vpfx);


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

            double   G0    = hamiltonian(Corg);
            unsigned cycle = 0;

        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("---------------- cycle=" << cycle << " ----------------");
            YACK_CHEM_PRINTLN("G0=" << G0);
            if(verbose) lib(std::cerr << "Corg=",Corg,"");

            if(G0<=0)
            {
                YACK_CHEM_PRINTLN(" <success:: null hamiltonian @init/>");
                return returnSuccessful(C0,cycle);
            }

            bool gOK = true;
            {
                double             AX = 0;
                double             G1 = G0;
                const equilibrium *ex = NULL;
                const equilibrium *eg = NULL;

                //--- scan all solutions
                for(const enode *node=lattice.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    const double       xx = (Xl[ei] = eq.solve1D(Kl[ei], Corg, Cend)); assert( eq.other_are_unchanged(Corg,Cend) );
                    const double       ax = fabs( xx );
                    if(ax>AX)
                    {
                        AX =  ax;
                        ex = &eq;
                    }
                    triplet<double> u = { 0, -1, 1 };
                    triplet<double> g = { G0, -1, hamiltonian(Cend) };
                    optimize::run_for(*this,u,g,optimize::inside);

                    std::cerr << "NEED TO CHECK ON SIDE!!" << std::endl;
                    exit(2);

                    iota::load(Cl[ei],Ctry);
                    assert( eq.other_are_unchanged(Cl[ei],Corg) );

                    lattice.pad(std::cerr << " @{" << eq.name << "}",eq) << " = " <<std::setw(15) << xx;
                    std::cerr << " | G = " << std::setw(15) << g.b << " @u=" << u.b;
                    if(g.b<G1)
                    {
                        G1=g.b;
                        eg=&eq;
                    }
                    std::cerr << std::endl;
                }

                // check |Xi| status
                YACK_CHEM_PRINTLN("|Xi|  = " << std::setw(15) << AX << " @{" << (ex? ex->name() : "nil") << "}" );
                if(AX<=0)
                {
                    assert(NULL==ex);
                    YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @init/>");
                    return returnSuccessful(C0,cycle);
                }
                assert(NULL!=ex);

                // check global decrease status
                YACK_CHEM_PRINTLN(" G1   = " <<  std::setw(15) << G1 << " @{" << (eg? eg->name() : "NIL") << "}" );
                if(!eg)
                {
                    gOK = false;
                }
                else
                {
                    const equilibrium &eq   = *eg;
                    const group       *gOpt = look_up->find_first( eq ); assert(gOpt);
                    tableau           &Copt = Cend;
                    double             hOpt = aggregate(Copt,*gOpt);
                    for(const group   *gTmp = gOpt->next;gTmp;gTmp=gTmp->next)
                    {
                        if(!gTmp->contains(eq)) continue;
                        const double hTmp = aggregate(Ctry,*gTmp);
                        const bool   good = (hTmp<hOpt);
                        std::cerr << (good?"(+)":"(-)") << " G = " << std::setw(15) << hTmp  << " @" << *gTmp << " " << Ctry << std::endl;

                        if(good)
                        {
                            gOpt = gTmp;
                            hOpt = hTmp;
                            active.transfer(Copt,Ctry);
                        }
                    }

                    std::cerr << "best=" << *gOpt << std::endl;
                    G0 = hOpt;
                    active.transfer(Corg,Copt);

                    if(G0<=0)
                    {
                        YACK_CHEM_PRINTLN(" <success:: null hamiltonian @move/>");
                        return returnSuccessful(C0,cycle);
                    }

                    // recomputing AX and 'Xi'
                    AX = 0;
                    for(const enode *node=singles.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        const size_t       ei = *eq;
                        const double       xx = (Xl[ei] = eq.solve1D(Kl[ei], Corg, Cl[ei]));
                        const double       ax = fabs( xx );
                        if(ax>AX)
                        {
                            AX = ax;
                        }
                    }
                    singles(std::cerr << vpfx << "Xi_singles=",Xl,vpfx);
                    if(AX<=0)
                    {
                        YACK_CHEM_PRINTLN(" <success:: |Xi| = 0 @move/>");
                        return returnSuccessful(C0,cycle);
                    }

                }
                YACK_CHEM_PRINTLN(vpfx << "[gOK=" << yack_boolean(gOK) << "]");
            }

            


            return false;
        }
    }


}
