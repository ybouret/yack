

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/algebra/lu.hpp"
#include"yack/sort/sum.hpp"

#include <cmath>

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

        Corg(mtab.next()),
        Ctry(mtab.next()),
        dC(mtab.next()),

        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        Omega(M,M),
        stack(M,as_capacity),
        
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



        namespace {

            struct zGamma
            {
                plexus &sys;

                inline double operator()(double u)
                {
                    tao::v1::muladd(sys.Ctry,sys.Corg,u,sys.dC);
                    sys.computeGamma(sys.Ctry);
                    sys.stack.free();
                    for(size_t i=sys.N;i>0;--i)
                    {
                        sys.stack.push_back(sys.Gamma[i]);
                    }
                    return sorted::sum_squared(sys.stack)*0.5;
                }

            };
        }


        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);
            if(N>0)
            {
                tao::v1::load(Corg,C);
                std::cerr << "Corg=" << Corg << std::endl;

                matrix<double> W(M,M);
                lu<double>     LU(M);
                zGamma         G = { *this };

                // initialize
                computeGammaAndPsi(Corg);
                tao::v3::mmul(Omega,NuT,Psi);
                double tau = 1;

                // search for implicit solution
                tao::v3::smul(W,-tau,Omega);
                for(size_t i=M;i>0;--i) W[i][i] += 1.0;

                std::cerr << "W=" << W << std::endl;

                if(!LU.build(W))
                {
                    std::cerr << "not inversible" << std::endl;
                    exit(1);
                }

                tao::v2::mul(dC,NuT,Gamma);
                std::cerr << "NuTG=" << dC << std::endl;
                LU.solve(W,dC);
                std::cerr << "dC0 =" << dC << std::endl;
                tao::v1::mul(dC,tau,dC);
                std::cerr << "dC  =" << dC << std::endl;

                // scan for alpha max
                stack.free();
                for(const snode *S=lib.head();S;S=S->next)
                {
                    const species &s = ***S;
                    // drop spectator
                    if(s.rank<=0)
                    {
                        assert( fabs(s(dC)) <= 0);
                        continue;
                    }

                    // process active
                    assert(s(C)>=0);
                    const size_t j = s.indx;
                    const double dS = dC[j];
                    if(dS<0)
                    {
                        std::cerr << "could be limited by " << s.name << std::endl;
                        stack << -C[j]/dS;
                    }
                }

                std::cerr << "stack:" << stack << std::endl;
                double  alpha_max=100;
                if(stack.size())
                {
                    hsort(stack,comparison::increasing<double>);
                    alpha_max=stack[1];
                }
                std::cerr << "alpha_max: " << alpha_max << std::endl;

                std::cerr << "G(0)=" << G(0) << std::endl;

                {
                    ios::ocstream fp("gamma.dat");
                    for(double u=0;u<=0.99*alpha_max;u+=0.01)
                    {
                        fp("%.15g %.15g\n",u,G(u));
                    }
                }


            }
        }


    }
    
}

