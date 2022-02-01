

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/algebra/lu.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        plexus:: ~plexus() throw() {}

        static const char fn[] = "chemical::plexus";

        static inline
        size_t check(const library &lib, const equilibria &eqs)
        {
            if(lib.size()<eqs.size())
            {
                throw exception("%s: too many equilibria",fn);
            }
            return lib.size();
        }

        plexus:: plexus(library &lib_, equilibria &eqs_) :
        lib(lib_),
        eqs(eqs_),
        N(eqs.size()),
        M(check(lib,eqs)),
        A(lib.active()),
        ntab(10,N),
        mtab(10,M),
        K(ntab.next()),
        Gamma(ntab.next()),
        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        Omega(Nu.cols,Nu.rows),
        lib_lock( coerce(lib) )
        {
            std::cerr << "#species=" << M << std::endl;
            std::cerr << "#active =" << A << std::endl;
            std::cerr << "#eqs    =" << N << std::endl;

            // initialize Nu
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                eq.validate();
                eq.fill(coerce(Nu[eq.indx]));
            }

            // finalize
            coerce(NuT).assign(Nu,transposed);

            // check equilibria independence
            if(N>0)
            {
                matrix<apq> G(N,N);
                tao::v3::gram(G,Nu);
                std::cerr << "Nu = " << Nu << std::endl;
                std::cerr << "NuT= " << NuT << std::endl;
                std::cerr << "G  = " << G << std::endl;
                lu<apq> LU(N);
                if(!LU.build(G))
                {
                    throw exception("%s: dependant equilibria detected",fn);
                }

            }

        }

        void plexus:: computeK(const double t)
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                K[eq.indx] = eq.K(t);
            }
        }

        void plexus:: computeGamma(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                Gamma[i] = eq.mass_action(K[i],C);
            }
        }

        void plexus:: computeGammaAndPsi(const readable<double> &C) throw()
        {
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       i  = eq.indx;
                Gamma[i] = eq.drvs_action(Psi[i],K[i],C);
            }
        }

        void plexus:: computeOmega() throw()
        {
            for(size_t i=N;i>0;--i)
            {
                for(size_t j=M;j>0;--j)
                {
                    Omega[j][i] = NuT[j][i] * Gamma[i];
                }
            }
        }


        void plexus:: study()
        {

            for(const enode *E=eqs.head();E;E=E->next)
            {
                const equilibrium   &eq = ***E;
                const size_t         ii = eq.indx;
                const double         gm = Gamma[ii];
                const readable<int> &ni = Nu[ii];

                std::cerr << "<" << eq.name << ">" << std::endl;
                std::cerr << "\tgamma=" << gm << std::endl;

                for(const snode *S = lib.head();S;S=S->next)
                {
                    const species &sp = ***S;
                    const size_t   jj = sp.indx;
                    const int      nu = ni[jj];
                    if(!nu) continue;
                    const double   omega = nu*gm;
                    if(omega<=0)
                    {
                        std::cerr << "\t\tlimited by  " << sp.name << std::endl;
                    }
                    else
                    {
                        std::cerr << "\t\tno limit by " << sp.name << std::endl;
                    }
                }

            }



        }

    }
    
}

