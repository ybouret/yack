#include "yack/chem/reactor.hpp"
#include "yack/exception.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"

namespace yack
{
    using namespace math;
    
    namespace chemical
    {

        const bool & reactor::verbose  = entity::verbose;
        
        const char   reactor::clid[]   = "chemical::reactor";
        const char   reactor::vpfx[]   = "//   ";

        reactor:: ~reactor() throw()
        {
        }

        reactor:: reactor(library    &lib_,
                          equilibria &eqs_,
                          const double t0) :
        lib( lib_ ),
        eqs( eqs_ ),
        sub(lib),
        M( lib.size()    ),
        MA(lib.active()  ),
        MP(lib.primary() ),
        N( eqs.size()    ),
        NC(0),
        couples(),

        ntab(10,N),
        mtab(10,M),

        active(),
        K(     ntab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),



        lib_lock(lib_),
        eqs_lock(eqs_)
        {

            YACK_CHEM_PRINTLN("<" << clid << ">");
            YACK_CHEM_PRINTLN("  M  = " << M);
            YACK_CHEM_PRINTLN("  MA = " << MA);
            YACK_CHEM_PRINTLN("  MP = " << MP);
            YACK_CHEM_PRINTLN("  N  = " << N);

            //------------------------------------------------------------------
            //
            // build active species
            //
            //------------------------------------------------------------------
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    coerce(active) << &s;
                }
            }
            assert(MA==active.size);

            //------------------------------------------------------------------
            //
            // build Nu
            //
            //------------------------------------------------------------------
            equilibrium::display_time = t0;
            YACK_CHEM_PRINTLN(eqs);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                if(!eq.is_minimal()) throw exception( "%s is not minimal", eq.name() );
                if(!eq.is_neutral()) throw exception( "%s is not neutral", eq.name() );
                eq.fill( coerce(Nu[*eq]) );
                K[*eq] = eq.K(t0);

            }
            coerce(NuT).assign(Nu,transposed);
            YACK_CHEM_PRINTLN("  Nu =" << Nu);
            YACK_CHEM_PRINTLN("  NuT=" << NuT);
            YACK_CHEM_PRINTLN("  K  =" << K);

            //------------------------------------------------------------------
            //
            // check indep equilibria
            //
            //------------------------------------------------------------------
            if(N>0)
            {
                matrix<apq> G(N,N);
                lu<apq>     alu(N);
                tao::v3::gram(G,Nu);
                if( !alu.build(G) ) throw exception("%s: dependent equilibria",clid);
            }


            //------------------------------------------------------------------
            //
            // couples
            //
            //------------------------------------------------------------------
            coerce(NC) = build_couples();


            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("<" << clid << "/>");

        }





    }

}

