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

        K(  ntab.next()  ),
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
                    working.tranfer(C0,Corg);
                    return true;
                } break;

                default:
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                    }
            }


            // compute initial state
            for(const enode *node = lattice.head(); node; node=node->next )
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                const outcome      oc = outcome::study(eq, Kl[ei], Corg, Ci, xmul, xadd);
                switch(oc.state)
                {
                    case components::are_blocked: blocked[ei] = true;  break;
                    case components::are_running: blocked[ei] = false; break;
                }
                Xl[ei] = oc.value;
            }






            return false;
        }

    }
}
