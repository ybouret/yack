#include "yack/chem/plexus.hpp"
#include "yack/chem/eqs/cross.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"
#include "yack/apex/integer.hpp"
#include "yack/sort/sum.hpp"
#include "yack/math/opt/optimize.hpp"
#include <cmath>
#include <iomanip>

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
#if 0
                ios::acstream fp("hamiltonian.dat");
                const size_t NP = 1000;
                for(size_t i=0;i<=NP;++i)
                {
                    const double uu = i/double(NP);
                    fp("%.15g %.15g %u\n", uu, (*this)(uu), unsigned(ei));
                }
                fp << '\n';
#endif

                triplet<double> u = { 0,  -1, 1 };
                triplet<double> g = { G0, -1, hamiltonian(Cend) };
                optimize::run_for(*this,u,g,optimize::inside);
                lattice.pad(std::cerr << vpfx << eq.name,eq) << " : " << G0 << " --> " << g.b << " @" << u.b << std::endl;
                iota::load(Cs[ei],Ctry);
            }
            else
            {
                iota::load(Cs[ei],Corg);
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
            YACK_CHEM_MARKUP(vpfx, "computeLatticeExtent");
            const double G0 = hamiltonian(Corg);
            //ios::ocstream::overwrite("hamiltonian.dat");

            //------------------------------------------------------------------
            //
            // summing |Xi| on singles
            //
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
                    (void) optimizeGlobalExtent(G0,***node);
                }

            }

            if(verbose) lattice(std::cerr << vpfx << "Xi=", Xl, vpfx);
            YACK_CHEM_PRINTLN(vpfx << "@G0=" << G0);
            return sumAbsXi;

        }

        double plexus:: optimizedCombination(const cluster &cc) throw()
        {
            iota::load(Ctry,Corg);
            for(const vnode *node=cc.head;node;node=node->next)
            {
                const equilibrium      &eq  = **node;
                const readable<double> &Ceq = Cs[*eq];
                eq.transfer(Ctry,Ceq);
            }
            return hamiltonian(Ctry);
        }


        void plexus:: searchGlobalDecrease() throw()
        {
            YACK_CHEM_MARKUP(vpfx, "searchGlobalDecrease");
            const  cluster *cc   = com.head;
            const  cluster *cOpt = cc;
            double          gOpt = optimizedCombination(*cOpt);
            iota::load(Cend,Ctry);
            for(cc=cc->next;cc;cc=cc->next)
            {
                const double gTmp  = optimizedCombination(*cc);
                if(gTmp<gOpt)
                {
                    gOpt = gTmp;
                    cOpt = cc;
                    iota::load(Cend,Ctry);
                }
            }
            if(verbose)
            {
                std::cerr << vpfx << " => "  << *cOpt << std::endl;
                std::cerr << vpfx << " => @" <<  gOpt << " from " << hamiltonian(Corg) << std::endl;
                lib(std::cerr << vpfx << " => Copt=",Cend,vpfx);
            }
            iota::load(Corg,Cend);

        }


        size_t plexus:: computeOmega() throw()
        {
            size_t num_running = N;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium   &eq  = ***node;
                const size_t         ei  = *eq;
                writable<double>    &psi = Psi[ei];
                writable<double>    &Omi = Omega0[ei];
                double              &Gam = Gamma[ei];

                Gam = eq.grad_action(psi,K[ei],Corg,Ctry);
                const double d = sorted::dot(psi,Nu[ei],Ctry);
                std::cerr << "d=" << d << std::endl;
                if(d>=0)
                {
                    --num_running;
                    Omi.ld(0);
                    Omi[ei] = 1.0;
                    Gam     = 0.0;
                }
                else
                {
                    if( fabs(Xl[ei]) <= 0) Gam = 0.0; // regularize a little
                    Omi[ei] = -d;
                    for(const enode *scan=node->prev;scan;scan=scan->prev)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                    for(const enode *scan=node->next;scan;scan=scan->next)
                    {
                        const size_t       k   = ****scan;
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctry);
                    }
                }
            }

            singles(std::cerr << "Gamma=",Gamma,"");
            singles(std::cerr << "Omega=",Omega0,"");



            return 0;
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
            // check status of |Xi| while computing all extents
            //
            //------------------------------------------------------------------
            const double sumAbsXi = computeLatticeExtent();
            YACK_CHEM_PRINTLN(vpfx << "|Xi|=" << sumAbsXi);


            if(sumAbsXi<=0)
            {
                YACK_CHEM_PRINTLN(vpfx << "  <SUCCESS/>");
                return true;
            }

            //------------------------------------------------------------------
            //
            // then choose the most promising cluster
            //
            //------------------------------------------------------------------
            searchGlobalDecrease();


            computeOmega();


            return false;

        }

    }

}
