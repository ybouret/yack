
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

        const char plexus::clid[] = "chemical::plexus";

        plexus:: ~plexus() throw()
        {
        }

        plexus:: plexus(library    &lib_,
                        equilibria &eqs_) :
        lib( lib_ ),
        eqs( eqs_ ),
        M( lib.size() ),
        A( lib.active() ),
        N( eqs.size() ),

        ntab(10,N),
        mtab(10,M),

        active(),
        K(     ntab.next() ),
        Gamma( ntab.next() ),
        Xi(    ntab.next() ),
        xi(    ntab.next() ),
        xs(    ntab.next() ),

        Ctmp(  mtab.next() ),
        Ctry(  mtab.next() ),
        dC(    mtab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),

        Psi(Nu.rows,Nu.cols),
        Ceq(Nu.rows,Nu.cols),
        Omega0(N,N),
        iOmega(N,N),
        rstack(M,as_capacity),
        ustack(M,as_capacity),
        LU(N),

        lib_lock(lib_),
        eqs_lock(eqs_)
        {

            YACK_CHEM_PRINTLN("// M=" << M);
            YACK_CHEM_PRINTLN("// A=" << A);
            YACK_CHEM_PRINTLN("// N=" << N);

            // build active species
            for(const snode *node=lib.head();node;node=node->next)
            {
                const species &s = ***node;
                if(s.rank>0)
                {
                    coerce(active) << &s;
                }
            }
            assert(A==active.size);

            // build Nu
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.fill( coerce(Nu[*eq]) );
            }
            coerce(NuT).assign(Nu,transposed);
            YACK_CHEM_PRINTLN("Nu =" << Nu);
            YACK_CHEM_PRINTLN("NuT=" << NuT);

            // check indep equilibria
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


        


        bool plexus:: computeDeltaC(const readable<double> &C) throw()
        {
            dC.ld(0);
            ustack.free();
            rstack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species           &s     = **node;
                const size_t             j     = *s;
                const readable<int>     &nut   = NuT[j];
                for(size_t i=N;i>0;--i)  xs[i] = nut[i] * xi[i];
                const double c  = C[j]; assert(c>=0);
                const double d  = (dC[j] = sorted::sum(xs,sorted::by_abs_value));
                if(d<0)
                {
                    const double md    = -d;
                    if(md>=c)
                    {
                        rstack.push_back_fast(c/md);
                        ustack.push_back_fast(j);
                    }
                }
            }
            std::cerr << "C =" << C << std::endl;
            std::cerr << "dC=" << dC << std::endl;

            if(rstack.size()>0)
            {
                hsort(rstack,ustack,comparison::increasing<double>);
                if(entity::verbose)
                {
                    std::cerr <<"//   <plexus.deltaC/> [failure:";
                    for(size_t i=1;i<=rstack.size();++i)
                    {
                        std::cerr << " " << rstack[i] << "@" << lib[ustack[i]].name;
                    }
                    std::cerr << "]" << std::endl;
                }
                return false;
            }
            else
            {
                YACK_CHEM_PRINTLN("//   <plexus.deltaC/> [success]");
                return true;
            }

        }

    }

}
