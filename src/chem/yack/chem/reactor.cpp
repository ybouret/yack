#include "yack/chem/reactor.hpp"
#include "yack/chem/mix/independency.hpp"
#include "yack/chem/mix/couples.hpp"
#include "yack/exception.hpp"
#include "yack/math/iota.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {
        const char   reactor::clid[]  = "chemical::reactor";
        const char   reactor::vpfx[]  = "//  ";
        const bool & reactor::verbose = entity::verbose;

        reactor:: ~reactor() throw()
        {
        }

        reactor:: reactor(library     &lib_,
                          equilibria  &eqs_,
                          const double t0) :
        lib(lib_),
        sub(lib),
        singles(eqs_),
        couples(),
        lattice(),
        look_up(NULL),

        M( lib.size()     ),
        N( singles.size() ),
        L(0),

        active(lib),
        Nu(N,N>0?M:0),
        NuA(Nu.rows,Nu.cols),
        Psi(Nu.rows,Nu.cols),
        Omega0(N,N),
        iOmega(N,N),

        LU( new rsolver(N) ),
        ntab(10,N),
        mtab(10,M),
        ltab(10,L),

        K( ntab.next() ),
        Xend( ntab.next() ),
        Xtry( ntab.next() ),
        Gamma( ntab.next() ),
        xi( ntab.next() ),
        blocked( ntab.next(), transmogrify ),

        Corg( mtab.next() ),
        Cend( mtab.next() ),
        Ctry( mtab.next() ),
        Cstp( mtab.next() ),

        ratio(M,as_capacity),

        Kl( ltab.next() ),
        Xl( ltab.next() ),
        //Ok( ltab.next(), transmogrify ),
        Cl(),

        libLock(lib_),
        eqsLock(eqs_)
        {

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("--------  library  --------");
            YACK_CHEM_PRINTLN(lib);
            YACK_CHEM_PRINTLN("--------  singles  --------");
            YACK_CHEM_PRINTLN(singles);

            if(N>0)
            {
                //--------------------------------------------------------------
                //
                // build topology and first set of constants
                //
                //--------------------------------------------------------------
                singles.build( coerce(Nu), K, t0);
                YACK_CHEM_PRINTLN("--------  topology --------");
                YACK_CHEM_PRINTLN("Nu="<<Nu);
                YACK_CHEM_PRINTLN("K="<<K);


                //--------------------------------------------------------------
                //
                // check independency
                //
                //--------------------------------------------------------------
                if(!independency::of(Nu)) throw exception("%s found dependent equilibria",clid);

                //--------------------------------------------------------------
                //
                // make couples from copy of library
                //
                //--------------------------------------------------------------
                coupling::build(coerce(couples),singles,Nu,K,sub);
                YACK_CHEM_PRINTLN("--------  couples  --------");
                YACK_CHEM_PRINTLN(couples);

                //--------------------------------------------------------------
                //
                // make lattice
                //
                //--------------------------------------------------------------
                coerce(lattice) << singles << couples;
                YACK_CHEM_PRINTLN("--------  lattice  --------");
                YACK_CHEM_PRINTLN(lattice);

                //--------------------------------------------------------------
                //
                // compute lattice properties
                //
                //--------------------------------------------------------------
                if( 0 < (coerce(L) = lattice.size()) )
                {
                    ltab.make(L);
                    //Ok.relink<bool>();  assert(L==Ok.size());
                    assert(L==Kl.size());
                    assert(L==Xl.size());

                    Cl.make(L,M);

                    iota::save(Kl,K);
                    for(const enode *node=couples.head();node;node=node->next)
                    {
                        const equilibrium &eq = ***node;
                        Kl[*eq] = eq.K(t0);
                    }
                    YACK_CHEM_PRINTLN("Kl="<<Kl);

                    coerce(look_up) = groups::create_from(lattice);

                    YACK_CHEM_PRINTLN("--------  look_up  --------");
                    YACK_CHEM_PRINTLN(look_up);
                }
            }
            YACK_CHEM_PRINTLN("--------  [ready]  --------" << std::endl);


        }


    }
}

