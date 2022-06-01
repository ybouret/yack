
#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/minimize.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        double reactor:: meanGammaSquared(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Xtmp[ei] = squared( eq.mass_action(K[ei],C) );
            }
            return sorted::sum(Xtmp,sorted::by_value)/N;
        }

        double reactor:: operator()(const double u) throw()
        {
            assert(u>=0);
            assert(u<=1);
            const double omu = 1.0 - u;
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                Ctry[j]        = omu * Corg[j] + u * Cend[j];
            }
            return meanGammaSquared(Ctry);
        }

        double reactor:: optDecrease(const double G0)
        {
            triplet<double> u = { 0, -1, 1 };
            triplet<double> g = { G0, -1, meanGammaSquared(Cend) };
            (void) minimize::find<double>::run_for(*this,u,g, minimize::inside);

            return g.b; //!< and Ctry is computed
        }

        bool reactor:: solve(writable<double> &C0) throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);

            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

#if 1
                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D success]");
                    return true;
#endif

                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = Cend[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }


            size_t cycle = 0;

            ++cycle;
            YACK_CHEM_PRINTLN("//   ---------------- cycle#" << cycle << " ----------------");

            const double G0 = meanGammaSquared(Corg);

            // direct
            YACK_CHEM_PRINTLN(vpfx << "SINGLES");
            {
                double sumAbsXi = 0;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;     assert(ei>0); assert(ei<=N);
                    const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                    if(ax>0)
                    {
                        sumAbsXi += ax;
                        Gtot[ei]  = optDecrease(G0);
                        transfer(Ctot[ei],Ctry);
                        eqs.pad(std::cerr << eq.name,eq) << " : " << Gtot[ei] << " @" << Ctot[ei] << std::endl;
                    }
                    else
                    {
                        Gtot[ei] = G0;
                        transfer(Ctot[ei],Corg);
                    }
                }

                //eqs(std::cerr << vpfx << "Xi=", Xtot, vpfx);
                //eqs(std::cerr << vpfx << "G2=", Gtot, vpfx);

                YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);

                if(sumAbsXi<=0)
                {
                    transfer(C0,Corg);
                    YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
                    return true;
                }
            }

            // coupled
            YACK_CHEM_PRINTLN(vpfx << "COUPLES");
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;     assert(ei>N); assert(ei<=Ntot);
                const double       ax = fabs( Xtot[ei] = eq.solve1D(Ktot[ei], Corg, Cend) );
                if(ax>0)
                {
                    Gtot[ei]  = optDecrease(G0);
                    transfer(Ctot[ei],Ctry);
                    couples.pad(std::cerr << eq.name,eq) << " : " << Gtot[ei] << " @" << Ctot[ei] << std::endl;
                }
                else
                {
                    Gtot[ei] = G0;
                    transfer(Ctot[ei],Corg);
                }
            }

            eqs(std::cerr << vpfx << "Xi=", Xtot, vpfx);
            couples(std::cerr << vpfx << "Xi=", Xtot, vpfx);
            //couples(std::cerr << vpfx << "G2=", Gtot, vpfx);

            const enode       *eOpt = eqs.head();
            double             gOpt = Gtot[****eOpt];
            for(const enode *node=eOpt->next;node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            for(const enode *node=couples.head();node;node=node->next)
            {
                const double gTmp = Gtot[****node];
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    eOpt = node;
                }
            }

            const equilibrium &opt = ***eOpt;
            std::cerr << "Winner is " << opt.name << std::endl;


            exit(1);
            YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
            return true;
        }
    }

}


