

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sort/sum.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/math/opt/minimize.hpp"

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
        Omega(NuT.rows,NuT.cols),
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
                std::cerr << "Nu  = " << Nu << std::endl;
                std::cerr << "NuT = " << NuT << std::endl;
                std::cerr << "G   = " << G << std::endl;
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



#if 0
        namespace {

            struct zGamma
            {
                plexus &sys;

                inline double operator()(double u)
                {
                    tao::v1::muladd(sys.Ctry,sys.Corg,u,sys.dC);
                    //std::cerr << "Ctry@" << sys.Ctry << std::endl;
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
#endif

        void plexus:: solve(writable<double> &C)
        {
            assert(C.size()>=M);


            if(N>0)
            {

                // scaling
                vector<double> xi(N,0);
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       i  = eq.indx;
                    xi[i] = eq.scale(K[i],C,Ctry);
                }
                std::cerr << "xi=" << xi << std::endl;
                


            }

#if 0
            if(N>0)
            {
                vector<double> xi(N,0);
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       i  = eq.indx;
                    xi[i] = eq.scale(K[i],C,Ctry);
                }
                std::cerr << "xi_s=" << xi << std::endl;
                tao::v2::mul(dC,NuT,xi);
                std::cerr << "dC  =" << dC << std::endl;
                stack.free();
                stack << 1;
                for(size_t j=M;j>0;--j)
                {
                    const double d = -dC[j];
                    if(d>0)
                    {
                        const double c = C[j];
                        //std::cerr << "Checking " << lib[j] << std::endl;
                        if(d>c)
                        {
                            stack << (c/d);
                        }
                    }
                }
                hsort(stack,comparison::increasing<double>);
                std::cerr << "stack= " << stack << std::endl;

                for(size_t j=M;j>0;--j)
                {
                    for(size_t i=N;i>0;--i)
                    {
                        Omega[j][i] = Nu[i][j] * xi[i];
                    }
                }
                std::cerr << "Omega=" << Omega << std::endl;
                rmatrix W(N,N);
                tao::v3::mmul(W,Psi,Omega);
                std::cerr << "W=" << W << std::endl;

                lu<double> LU(N);
                if(!LU.build(W))
                {
                    std::cerr << "singular..." << std::endl;
                }
                else
                {
                    vector<double> lam(N,0);
                    tao::v1::neg(lam,Gamma);
                    LU.solve(W,lam);
                    std::cerr << "lam=" << lam << std::endl;
                    tao::v2::mul(dC,Omega,lam);
                    std::cerr << "dC=" << dC << std::endl;
                    for(size_t j=M;j>0;--j)
                    {
                        C[j] += dC[j];
                    }
                    std::cerr << "Cnew=" << C << std::endl;
                    computeGamma(C);
                    std::cerr << "Gam1=" << Gamma << std::endl;
                }

            }
#endif
        }


    }
    
}

