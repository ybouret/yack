
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
        dC(    mtab.next() ),

        Nu(N,N>0?M:0),
        NuT(Nu.cols,Nu.rows),

        Psi(Nu.rows,Nu.cols),
        Ceq(Nu.rows,Nu.cols),
        Omega(N,N),
        
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

        void plexus:: computeXi(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                writable<double>  &Ci = Ceq[ei];
                for(size_t j=M;j>0;--j)
                    Ci[j] = C[j];
                Xi[ei] = eq.solve1D(K[ei],C,Ci);
            }
        }


        static inline
        double xdot(const readable<double> &psi,
                    const readable<int>    &nu,
                    const size_t            M,
                    writable<double>       &arr)
        {
            for(size_t j=M;j>0;--j)
            {
                arr[j] = psi[j] * nu[j];
            }
            return sorted::sum(arr,sorted::by_abs_value);
        }

        void plexus:: computeExtent()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium      &eq = ***node;
                const size_t            ei = *eq;
                const readable<double> &Ci = Ceq[ei];
                eq.drvs_action(Psi[ei],K[ei],Ci,Ctmp);
            }
            std::cerr << "Psi0=" << Psi << std::endl;


            Omega.ld(0);

            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega[i]; Omi[i] = 1;
                const readable<double> &psi = Psi[i];
                const double            den = xdot(psi,Nu[i],M,Ctmp);
                if( fabs(den)>0 )
                {
                    for(size_t k=N;k>i;--k)
                    {
                        Omi[k] = xdot(psi,Nu[k],M,Ctmp);
                    }
                    for(size_t k=i-1;k>0;--k)
                    {
                        Omi[k] = xdot(psi,Nu[k],M,Ctmp);
                    }
                }
            }


            std::cerr << "Omega=" << Omega << std::endl;
            if(!LU.build(Omega))
                throw exception("Singular Omega...");

            tao::v1::set(xi,Xi);
            LU.solve(Omega,xi);
            std::cerr << "xi=" << xi << std::endl;
        }

        void plexus:: correctExtent(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium      &eq = ***node;
                const size_t            ei = *eq;
                const limits           &lim = eq.primary_limits(C);
                eqs.pad(std::cerr << eq.name,eq) << " " << lim << std::endl;
                std::cerr << xi[ei] << " -> ";
                lim.clamp(xi[ei]);
                std::cerr << xi[ei] << std::endl;
            }
        }

        void plexus:: computeDeltaC() throw()
        {
            for(size_t j=M;j>0;--j)
            {
                const readable<int>     &nut = NuT[j];
                for(size_t i=N;i>0;--i)  xs[i] = nut[i] * xi[i];
                dC[j] = sorted::sum(xs,sorted::by_abs_value);
            }
            std::cerr << "dC=" << dC << std::endl;
        }

    }

}
