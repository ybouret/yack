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
        Ntot(N),
        couples(),

        ntab(10,N),
        mtab(10,M),

        active(),
        K(     ntab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),

        Corg( mtab.next() ),
        Ctmp( mtab.next() ),
        Cend( mtab.next() ),
        Ctry( mtab.next() ),
        dC(   mtab.next() ),

        Xtmp( ntab.next() ),

        Ktot(),
        Xtot(),
        Gtot(),
        Ctot(),

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
            coerce(NC)    = build_couples();
            coerce(Ntot) += NC;
            if(Ntot)
            {
                Ktot.adjust(Ntot,0);
                Xtot.adjust(Ntot,0);
                Gtot.adjust(Ntot,0);
                Ctot.make(Ntot,M);

                for(size_t i=N;i>0;--i) Ktot[i] = K[i];
                for(const enode *node=couples.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq; assert(ei>N); assert(ei<=Ntot);
                    Ktot[ei] = eq.K(t0);
                }
                YACK_CHEM_PRINTLN("  Ktot  = " << Ktot);
            }


            //------------------------------------------------------------------
            //
            // done
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("<" << clid << "/>");

        }


        void reactor:: transfer(writable<double> &targetC, const readable<double> &sourceC) const throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                targetC[j] = sourceC[j];
            }
        }

        bool reactor:: are_valid(const readable<double>  &C) const throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                if(C[j]<0) return false;
            }
            return true;
        }


    }

}

