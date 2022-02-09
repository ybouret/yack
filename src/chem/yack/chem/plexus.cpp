

#include "yack/chem/plexus.hpp"
#include "yack/exception.hpp"
#include "yack/math/tao/v3.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/math/algebra/lu.hpp"
#include "yack/sort/sum.hpp"
#include "yack/sort/indexing.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"

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
        xi(ntab.next()),

        Nu(N, (N>0) ? M : 0),
        NuT(Nu.cols,Nu.rows),
        Psi(Nu.rows,Nu.cols),
        Omega(NuT.rows,NuT.cols),
        rstack(M,as_capacity),
        ustack(M,as_capacity),

        LU(N),

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
                lu<apq> alu(N);
                if(!alu.build(G))
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


        double plexus:: objectiveGamma() throw()
        {
            rstack.free();
            for(size_t i=N;i>0;--i)
            {
                rstack.push_back( squared(Gamma[i]) );
            }
            return sorted::sum(rstack,sorted::by_value)*0.5;
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
                rmatrix W(N,N);

                //--------------------------------------------------------------
                //
                // initialize
                //--------------------------------------------------------------

                tao::v1::load(Corg,C);
                lib(std::cerr << "C0=",Corg);
                computeGammaAndPsi(Corg);
                size_t iter = 0;

                ITER:
                ++iter;
                ustack.free();
                // ensure maximum regularity
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       i  = eq.indx;
                    const readable<double> &psi = Psi[i];
                    if( tao::v1::mod2<double>::of(psi) > 0)
                        continue;
                    std::cerr << " moving " << eq.name << std::endl;
                    eq.solve(K[i],Corg,Ctry);
                    computeGammaAndPsi(Corg);
                    if( tao::v1::mod2<double>::of(psi) > 0)
                        continue;
                    ustack << i;
                }
                lib(std::cerr << "C1=",Corg);
                const double G0 = objectiveGamma();
                std::cerr << "singular=" << ustack << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
                std::cerr << "Psi  =" << Psi   << std::endl;
                std::cerr << "Nu   =" << Nu    << std::endl;
                std::cerr << "G0   =" << G0    << std::endl;


                tao::v3::mmul_trn(W,Psi,Nu);
                for(size_t i=ustack.size();i>0;--i)
                {
                    const size_t k = ustack[i];
                    W[k][k]  = 1;
                    Gamma[k] = 0;
                }
                std::cerr << "W    =" << W << std::endl;

                if(!LU.build(W))
                {
                    throw exception("Singular...");
                }

                // Newton's step
                tao::v1::neg(xi,Gamma);
                LU.solve(W,xi);
                std::cerr << "xi0  =" << xi << std::endl;

                // checking legal
                for(size_t i=N;i>0;--i)
                {
                    double &x = xi[i];
                    if(x*Gamma[i]<=0) x=0;
                }
                std::cerr << "xi1  =" << xi << std::endl;

                // estimating dC
                tao::v2::mul(dC,NuT,xi);
                std::cerr << "dC   =" << dC << std::endl;

                
                rstack.free();
                ustack.free();
                for(const snode *node=lib.head();node;node=node->next)
                {
                    const species &sp = ***node;
                    const size_t   j  = sp.indx;
                    const double   d  = dC[j];
                    if(d<0)
                    {
                        std::cerr << "  need to check " << sp << std::endl;
                        const double b = -d;
                        const double c = Corg[j]; assert(c>=0);
                        if(b>=c)
                        {
                            rstack << c/b;
                            ustack << j;
                        }
                    }
                }

                if(rstack.size())
                {
                    std::cerr << "rstack=" << rstack << std::endl;
                    std::cerr << "ustack=" << ustack << std::endl;
                    hsort(rstack,ustack,comparison::increasing<double>);
                    const double scale = rstack.front();
                    size_t       count = 1;
                    while(count<rstack.size() && rstack[count+1] <= scale) ++count;
                    while(rstack.size()>count)
                    {
                        rstack.pop_back();
                        ustack.pop_back();
                    }
                    std::cerr << "rstack=" << rstack << std::endl;
                    std::cerr << "ustack=" << ustack << std::endl;
                    for(const snode *node=lib.head();node;node=node->next)
                    {
                        const species &sp = ***node;
                        const size_t   j  = sp.indx;
                        Corg[j] = max_of<double>(Corg[j]+scale*dC[j],0);
                    }
                    for(size_t i=count;i>0;--i)
                    {
                        Corg[ ustack[i] ] = 0;
                    }
                }
                else
                {
                    std::cerr << "full step" << std::endl;
                    for(const snode *node=lib.head();node;node=node->next)
                    {
                        const species &sp = ***node;
                        const size_t   j  = sp.indx;
                        Corg[j] = max_of<double>(Corg[j]+dC[j],0);
                    }
                }
                lib(std::cerr << "C2=",Corg);
                computeGammaAndPsi(Corg);
                const double G1 = objectiveGamma();
                std::cerr << "G1=" << G1 << "/G0=" << G0 << std::endl;

                if(iter<10)
                    goto ITER;



#if 0
                // scaling
                vector<double> xi_s(N,0);
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       i  = eq.indx;
                    xi_s[i] = eq.scale(K[i],C,Ctry);
                }
                std::cerr << "xi_s =" << xi_s << std::endl;
#endif

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

