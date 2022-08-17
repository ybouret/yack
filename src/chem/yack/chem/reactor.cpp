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

        const char reactor:: clid[]  = "chemical::reactor";
        bool      &reactor:: verbose = entity::verbose;

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
            static const char fn[] = "[reactor] ";
            equilibrium::display_time = t;
            YACK_CHEM_PRINTLN(fn << "---------------- build ----------------");
            YACK_CHEM_PRINTLN(fn << "@" << t );

            YACK_CHEM_PRINTLN(fn << "corelib = " << corelib);
            YACK_CHEM_PRINTLN(fn << "singles = " << singles);


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

                if(verbose)
                {
                    singles(std::cerr << "Nu = ", "Nu_", Nu);
                }


                //--------------------------------------------------------------
                // testing system
                //--------------------------------------------------------------
                {
                    lu<apq>     apLU(N);
                    matrix<apq> Gram(N,N);
                    iota::gram(Gram,Nu);
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
                YACK_CHEM_PRINTLN(fn << "lattice = " << lattice);
            }

            YACK_CHEM_PRINTLN(fn << "---------------- built ----------------" << std::endl );


        }

    }

}

#include "yack/chem/outcome.hpp"
#include <iomanip>

namespace yack
{
    namespace chemical
    {

        bool reactor:: solved(writable<double> &C0)
        {
            working.tranfer(C0,Corg);
            if(verbose)
            {
                corelib(std::cerr << "Cend=", "", C0);
            }
            return true;
        }

        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor.solve] ";
            YACK_CHEM_PRINTLN(fn);

            //------------------------------------------------------------------
            //
            // depending on topology
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [empty]");
                    return true;

                case 1: {
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [single]");
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

            unsigned cycle = 0;
        CYCLE:
            //------------------------------------------------------------------
            //
            // study singles, initialize phase space
            //
            //------------------------------------------------------------------
            ++cycle;
            YACK_CHEM_PRINTLN(fn << "---------------- #cycle= " << std::setw(3) << cycle << " ----------------");
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

                if(verbose)
                {
                    singles.pad(std::cerr << "\t (+) " << '<' << eq.name << '>', eq) << " : " << oc << std::endl;
                }

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
                YACK_CHEM_PRINTLN(fn << "SUCCESS [fully solved]");
                return solved(C0);
            }

            assert(emax);
            YACK_CHEM_PRINTLN(fn << "emax=" << emax->name);

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [all blocked]");
                    return solved(C0);

                case 1:
                    YACK_CHEM_PRINTLN(fn << "is the only one running...");
                    working.tranfer( Corg, Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }



            return false;
        }

    }
}
