
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


        bool reactor:: happensUnderflow() throw()
        {
            // test all singles
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                if(eq.changed(Corg, Xend[ei], Ctry)) return false;
            }

            // nothing has changed!
            return true;
        }



        double reactor:: upgradeSinglesXi() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::singlesXi::update");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Cl[ei];
                Xend[ei] = eq.solve1D(K[ei],Corg,Ci) ;
            }
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xend,vpfx);
            return sorted::sum(Xtry,sorted::by_value);
        }



        double   reactor:: studyEquilibrium(const equilibrium &eq, double &G0) throw()
        {
            const size_t       ei = *eq;                           // get index
            const double       xx = eq.solve1D(Kl[ei],Corg,Cend);  // full step
            double             G1 = hamiltonian(Cend);             // full step hamiltonian
            triplet<double>    u  = { 0, -1, 1 };                  // prepare look up [0..1] * xx
            triplet<double>    g  = { G0, -1, G1 };                // values
            optimize::run_for(*this,u,g,optimize::inside);         // optimization step
            iota::save(Cl[ei],Ctry);                               // save state
            lattice.pad(std::cerr << eq.name,eq) <<  "@xi=" << std::setw(15) << xx << " G: " << G0 << " --> " << G1 << " -> " << g.b << "@u=" << u.b << std::endl;
            assert(g.b<=G0);
            G0 = g.b;
            return xx;
        }

        double reactor:: computeSinglesXi(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::computeSinglesXi");
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                double             G1 = G0;
                Xtry[ei] = fabs(  Xl[ei] = Xend[ei] = studyEquilibrium(eq,G1) );
            }
            if(verbose) singles(std::cerr << vpfx << "Xi=",Xend,vpfx);
            return sorted::sum(Xtry,sorted::by_value);
        }

        void reactor:: computeCouplesXi(const double G0) throw()
        {
            YACK_CHEM_MARKUP(vpfx, "reactor::computeCouplesXi");
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                double             G1 = G0;
                Xl[ei] = studyEquilibrium(eq,G1);
            }

            if(verbose)
            {
                lattice(std::cerr << vpfx << "Xi=",Xl,vpfx);
            }

        }
    }

}

