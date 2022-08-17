#include "yack/chem/reactor.hpp"
#include "yack/chem/eqs/composite.hpp"
#include "yack/math/iota.hpp"
#include "yack/math/algebra/lu.hpp"

#include "yack/apex.hpp"
#include "yack/system/imported.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        const char reactor:: clid[] = "chemical::reactor";

        reactor:: ~reactor() throw() {}

        reactor:: reactor(const library    &usrLib,
                          const equilibria &usrEqs,
                          const double      t) :
        corelib(usrLib),
        singles(usrEqs),
        working(corelib.head()),
        M(corelib.size()),
        N(singles.size()),
        L(0),
        couples(),
        lattice(singles),
        xmul(),
        xadd(),
        worklib(corelib),
        mtab(12,M),
        ntab(12,N),
        ltab(12,L),

        Nu(N,N>0?M:0),
        Psi(Nu.rows,Nu.cols),

        Corg( mtab.next() ),
        Cend( mtab.next() ),
        Ctry( mtab.next() ),

        K(     ntab.next()  ),
        sigma( ntab.next()  ),

        Kl( ltab.next()  ),
        Xl( ltab.next()  ),
        blocked( ltab.next(), transmogrify),
        Ceq(),

        lockLib( coerce(usrLib) ),
        lockEqs( coerce(usrEqs) )
        {
            equilibrium::display_time = t;

            if(N>0)
            {
                //--------------------------------------------------------------
                // initialize K and Nu
                //--------------------------------------------------------------
                for(const enode *node=singles.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    K[ei] = eq.K(t);
                    eq.fill( coerce(Nu[ei]) );
                }

                std::cerr << "Nu=" << Nu << std::endl;

                {
                    lu<apq>     apLU(N);
                    matrix<apq> Gram(N,N);

                    iota::gram(Gram,Nu);
                    std::cerr << "Gram=" << Gram << std::endl;

                    if(!apLU.build(Gram)) throw imported::exception(clid,"singular equilibria");

                }

                //--------------------------------------------------------------
                // compute couples
                //--------------------------------------------------------------
                composite::scatter( coerce(couples), worklib, singles, K, xmul);

                //--------------------------------------------------------------
                // complete lattice
                //--------------------------------------------------------------
                coerce(lattice).add(couples);
                coerce(L) = lattice.size();

                //--------------------------------------------------------------
                // rebuild ltab
                //--------------------------------------------------------------
                ltab.make(L);
                blocked.relink<bool>();
                Ceq.make(L,M);

                assert(L==Kl.size());
                assert(L==blocked.size());

                //--------------------------------------------------------------
                // initialize Kl
                //--------------------------------------------------------------
                for(size_t i=N;i>0;--i)
                {
                    Kl[i] = K[i];
                }

                //--------------------------------------------------------------
                // complete Kl
                //--------------------------------------------------------------
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei =  *eq;
                    Kl[ei] = eq.K(-1);
                }
                lattice(std::cerr << "Kl=", "K_", Kl);

            }

        }

    }

}

#include "yack/chem/outcome.hpp"

namespace yack
{
    namespace chemical
    {

        bool reactor:: solved(writable<double> &C0)
        {
            working.tranfer(C0,Corg);

            corelib(std::cerr << "Cend=", "", C0);

            return true;
        }

        bool reactor:: solve(writable<double> &C0)
        {

            //------------------------------------------------------------------
            //
            // depending on topology
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    std::cerr << "empty" << std::endl;
                    return true;

                case 1: {
                    std::cerr << "single" << std::endl;
                    const equilibrium &eq = ***singles.head();
                    outcome::study(eq, K[1], C0, Corg, xmul, xadd);
                    return solved(C0);
                } break;

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                    }
            }

        CYCLE:
            //------------------------------------------------------------------
            //
            // study singles, initialize phase space
            //
            //------------------------------------------------------------------
            double              amax = 0;
            size_t              nrun = 0;
            const  equilibrium *emax = NULL;
            outcome             ppty;
            
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const outcome      oc = outcome::study(eq, Kl[ei], Corg, Ci, xmul, xadd);

                singles.pad(std::cerr << '<' << eq.name << '>', eq) << " : " << oc << std::endl;

                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;
                        Xl[ei]      = 0;
                        sigma[ei]   = 0;
                        break;

                    case components::are_running: {
                        blocked[ei] = false;
                        
                        ++nrun;
                        const double ax = fabs( Xl[ei] = oc.value );
                        if(ax>amax)
                        {
                            amax =  ax;
                            emax = &eq;
                            ppty =  oc;
                        }
                    } break;
                }
            }

            if(!emax)
            {
                std::cerr << "fully solved" << std::endl;
                return solved(C0);
            }

            assert(emax);
            std::cerr << "emax=" << emax->name << std::endl;

            switch(nrun)
            {
                case 0:
                    std::cerr << "Totally Blocked" << std::endl;
                    return solved(C0);

                case 1:
                    std::cerr << "Only one running..." << std::endl;
                    working.tranfer( Corg, Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }





            return false;
        }

    }
}
