
#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        const char   plexus::clid[] = "chemical::plexus";

        const bool & plexus::verbose  = entity::verbose;

        plexus:: ~plexus() throw()
        {
        }

        plexus:: plexus(library    &lib_,
                        equilibria &eqs_) :
        lib( lib_ ),
        eqs( eqs_ ),
        M( lib.size() ),
        MA(lib.active() ),
        MP(lib.primary()),
        N( eqs.size() ),

        ntab(10,N),
        mtab(10,M),

        active(),
        K(     ntab.next() ),
        Gamma( ntab.next() ),
        xi(    ntab.next() ),
        xs(    ntab.next() ),
        xm(    ntab.next() ),
        Xi(    ntab.next() ),
        Gs(    ntab.next() ),
        xd(    ntab.next() ),

        Corg(  mtab.next() ),
        Ctmp(  mtab.next() ),
        Ctry(  mtab.next() ),
        Cend(  mtab.next() ),
        dC(    mtab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),

        Psi(Nu.rows,Nu.cols),
        Ceq(Nu.rows,Nu.cols),
        Omega0(N,N),
        iOmega(N,N),
        blocked(N,false),
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),

        lib_lock(lib_),
        eqs_lock(eqs_)
        {

            YACK_CHEM_PRINTLN("// M  = " << M);
            YACK_CHEM_PRINTLN("// MA = " << MA);
            YACK_CHEM_PRINTLN("// MP = " << MP);

            YACK_CHEM_PRINTLN("// N  = " << N);

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
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.fill( coerce(Nu[*eq]) );
            }
            coerce(NuT).assign(Nu,transposed);
            YACK_CHEM_PRINTLN("Nu =" << Nu);
            YACK_CHEM_PRINTLN("NuT=" << NuT);

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

        }


        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[*eq] = eq.K(t);
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                Gamma[ei] = eq.mass_action(K[ei],C);
            }
        }

        void plexus:: computePsi(const readable<double> &C) throw()
        {
            Psi.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                eq.drvs_action(Psi[ei],K[ei],C,Ctmp);
            }
        }

        void plexus:: computeState(const readable<double> &C) throw()
        {
            Psi.ld(0);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki = K[ei];
                Gamma[ei] = eq.grad_action(Psi[ei],Ki,C,Ctmp);
            }
        }



        void plexus:: transfer(writable<double> &targetC, const readable<double> &sourceC) const throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t j = ***node;
                targetC[j] = sourceC[j];
            }
        }

        bool plexus:: are_valid(const readable<double>  &C) throw()
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
