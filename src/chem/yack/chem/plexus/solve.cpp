

#include "yack/chem/plexus.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/indexing.hpp"

#include "yack/math/timings.hpp"
#include "yack/math/numeric.hpp"
#include "yack/counting/comb.hpp"


#include <cmath>
#include <iomanip>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        const char plexus::vpfx[] = "//   ";

        void plexus:: regularize() throw()
        {
            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize>");
            blocked.ld(false);

            //------------------------------------------------------------------
            //
            // check each equilibrium
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];

                //--------------------------------------------------------------
                // first try
                //--------------------------------------------------------------
                eq.drvs_action(psi,Ki,Corg,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    //----------------------------------------------------------
                    // move to a safer and still valid place
                    //----------------------------------------------------------
                    (void) eq.solve1D(Ki,Corg,Ctry);
                    transfer(Corg,Ctry);
                    
                    //----------------------------------------------------------
                    // second try
                    //----------------------------------------------------------
                    eq.drvs_action(psi,Ki,Corg,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                    }
                }
            }

            if(verbose)
            {
                lib(std::cerr << vpfx << "Creg    = ",Corg,vpfx);
                eqs(std::cerr << vpfx << "blocked = ",blocked,vpfx);
            }

            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }
        

        bool plexus:: primaryCut() throw()
        {
            if(MP)
            {
                YACK_CHEM_PRINTLN("//   <plexus.primaryCut>");
                bool changed = false;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;      if(blocked[ei]) continue;
                    const limits      &lm  =  eq.primary_limits(Corg,lib.width);

                    if(limited_by_none!=lm.type)
                        eqs.pad(std::cerr << vpfx << " <" << eq.name << ">",eq) << " : " << lm << std::endl;

                    if(lm.should_reduce(xi[ei]))
                    {
                        Omega0[ei][ei] *= 10;
                        changed = true;
                        YACK_CHEM_PRINTLN(vpfx<<" (*) reducing <" << eq.name << ">");
                    }
                }
                YACK_CHEM_PRINTLN("//   <plexus.primaryCut/> [changed=" << changed << "]");
                return changed;
            }
            else
            {
                // no primary species
                return false;
            }
        }

        bool plexus:: compute_dC() throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC>");

            //------------------------------------------------------------------
            //
            // compute and check all delta C
            //
            //------------------------------------------------------------------
            rstack.free();
            ustack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = sorted::dot(xi,NuT[j],xs);
                if(d<0)
                {
                    const double c = Corg[j]; assert(c>=0);
                    if(d<=-c)
                    {
                        YACK_CHEM_PRINTLN(vpfx<< "  (*) overshoot for " << s.name);
                        rstack << c/(-d);
                        ustack << j;
                    }
                }
            }

            //------------------------------------------------------------------
            //
            // check how many species overshot
            //
            //------------------------------------------------------------------
            if(rstack.size())
            {
                //--------------------------------------------------------------
                //
                // set factor for all implied equilibria modifying the species
                //
                //--------------------------------------------------------------
                std::cerr << "rstack=" << rstack << std::endl;
                std::cerr << "ustack=" << ustack << std::endl;
                xs.ld(1);
                while(ustack.size())
                {
                    const size_t          j = ustack.pop_back_value();
                    const readable<int> &nu = NuT[j];
                    for(size_t i=N;i>0;--i)
                    {
                        if(0!=nu[i])
                        {
                            xs[i] = 10;
                        }
                    }
                }
                if(verbose) eqs(std::cerr << vpfx << "xs=",xs,vpfx);

                //--------------------------------------------------------------
                //
                // apply factors
                //
                //--------------------------------------------------------------
                for(size_t i=N;i>0;--i) Omega0[i][i] *= xs[i];

                YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC/> [cut]");
                return false;

            }
            else
            {
                if(verbose) lib(std::cerr << vpfx << "dC=",dC,vpfx);
                rstack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   d = dC[j];
                    if(d<0)
                    {
                        const double c = Corg[j]; assert(c>=0);
                        assert(-d<c);
                        rstack << c/(-d);
                    }
                }
                hsort(rstack,comparison::increasing<double>);
                YACK_CHEM_PRINTLN("//    scalmax0=" << rstack);
                rstack.adjust(1,1.0);
                YACK_CHEM_PRINTLN("//    scalmax1=" << rstack);
                YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC> [ok]");
                return true;
            }
        }

        
        double plexus:: rmsGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            for(size_t i=N;i>0;--i)
            {
                xs[i] = squared(Gamma[i]/Gs[i]);
            }
            return sorted::sum(xs,sorted::by_value) / N;
        }

        double plexus:: operator()(const double u) throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t   j = ***node;
                //Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
                Ctry[j] = max_of( (1.0-u) * Corg[j] + u * Cend[j], 0.0 );
            }
            return rmsGamma(Ctry);
        }

        void plexus:: update(const enode *node) throw()
        {
            const equilibrium &eq  = ***node;
            const size_t       ei  = *eq;
            const double       Ki  = K[ei];
            writable<double>  &Ci  = Ceq[ei];
            writable<double>  &psi = Psi[ei];
            Xi[ei] = eq.solve1D(Ki,Corg,Ci);
            eq.drvs_action(psi,Ki,Ci,Ctmp);
            if( sorted::mod2(psi,Ctmp) <= 0)
            {
                blocked[ei] = true;
                Xi[ei]      = 0;
            }
            else
            {
                blocked[ei] = false;
            }
        }

        static inline
        void compute_target(writable<double>       &Cend,
                            const matrix<double>   &Ceq,
                            const readable<size_t> &comb,
                            vector<double>         &csum) throw()
        {
            const size_t nc = comb.size();
            csum.adjust(nc,0);
            for(size_t j=Cend.size();j>0;--j)
            {
                for(size_t i=nc;i>0;--i)
                {
                    const size_t k = comb[i];
                    csum[i] = Ceq[k][j];
                }
                Cend[j] = sorted::sum(csum,sorted::by_abs_value) / nc;
            }
        }


        bool plexus:: solve(writable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            assert(are_valid(C0));
            static const double SAFE = numeric<double>::golden_i;

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);
            ios::ocstream::overwrite("rms.dat");
            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }

            vector<enode *> en(N,NULL);
            vector<size_t > ix(N,0);
            const size_t    ncl = (1 << N) - 1;
            rmatrix         Ccl(ncl,M);
            vector<double>  Gcl(ncl,0);
            vector<double>  Ucl(ncl,0);
            vector<size_t>  Icl(ncl,0);
            size_t          cycle = 0;
        CYCLE:
            ++cycle;

            // global step
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &Ci  = Ceq[ei];
                writable<double>  &psi = Psi[ei];
                xm[ei] = fabs(Xi[ei]   = eq.solve1D(Ki,Corg,Ci));
                en[ei] = (enode *)node;
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    Gs[ei] = 1.0;
                    blocked[ei] = true;
                }
                else
                {
                    Gs[ei] = sorted::dot(psi,Nu[ei],Ctmp);
                    blocked[ei] = false;
                }
            }

            eqs(std::cerr << vpfx << "Xi=", Xi, vpfx);

            size_t       ipos = 0;
            const double g0   = rmsGamma(Corg);
            std::cerr << "g0=" << g0 << std::endl;
            for(size_t k=1;k<=N;++k)
            {
                combination comb(N,k);
                //std::cerr << "C(" << N << "," << k << ")=" << comb.total << std::endl;
                do
                {
                    ++ipos;
                    compute_target(Cend,Ceq,comb,rstack);
                    const double    g1 = rmsGamma(Cend);
                    triplet<double> uu = {0,-1,1};
                    triplet<double> gg = {g0,-1,g1};

                    minimize::find<double>::run_for(*this, uu, gg, minimize::inside);
                    std::cerr << comb << " #" << std::setw(4) << ipos << " -> " << std::setw(15) << gg.b << " =g(" << uu.b << ")" << std::endl;
                    Gcl[ipos] = gg.b;
                    Ucl[ipos] = uu.b;

                } while(comb.next());
            }


            std::cerr << "ipos=" << ipos << "/" << ncl << std::endl;

            indexing::make(Icl,comparison::increasing<double>,Gcl);
            std::cerr << "Icl=" << Icl << std::endl;


            exit(1);

#if 0
            //------------------------------------------------------------------
            //
            // full metrics
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];
                Xi[ei]    = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0 )
                {
                    blocked[ei] = true;
                    Xi[ei] = Gamma[ei] = 0;
                }
                else
                {
                    blocked[ei] = false;
                }
            }

            //------------------------------------------------------------------
            //
            // compute Omega0
            //
            //------------------------------------------------------------------
            for(size_t i=N;i>0;--i)
            {
                const readable<double> &psi = Psi[i];
                writable<double>       &Omi = Omega0[i];
                if(blocked[i])
                {
                    Omi.ld(0);
                    Omi[i] = 1.0;
                    Gs[i]  = 1.0;
                }
                else
                {
                    const double den = sorted::dot(psi,Nu[i],Ctmp); assert(den<0);
                    Omi[i] = 1.0;
                    Gs[i]  = fabs(den);
                    xm[i]  = 0;
                    for(size_t k=N;k>i;--k)   xm[k] = fabs( Omi[k] = sorted::dot(psi,Nu[k],Ctmp)/den );
                    for(size_t k=i-1;k>0;--k) xm[k] = fabs( Omi[k] = sorted::dot(psi,Nu[k],Ctmp)/den );
                    const double extra  = xd[i] = sorted::sum(xm,sorted::by_value);
                    const double xsafe  = SAFE * extra;
                    if(xsafe>=1) Omi[i] = timings::round_ceil(1.0 + xsafe);

                }
            }

            std::cerr << "blocked = " << blocked << std::endl;
            std::cerr << "extra   = " << xd      << std::endl;
            std::cerr << "Omega   = " << Omega0  << std::endl;
            std::cerr << "Xi      = " << Xi      << std::endl;
            const double rms = sorted::rms2(Xi,xs);
            ios::ocstream::echo("rms.dat","%g %.15g\n", double(cycle), rms);



        EVALUATE_EXTENT:
            std::cerr << "Omega   = " << Omega0  << std::endl;
            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                return false;
            }

            tao::v1::set(xi,Xi);
            LU.solve(iOmega,xi);

            std::cerr << "xi=" << xi << std::endl;

            if( primaryCut()  ) goto EVALUATE_EXTENT;
            if( !compute_dC() ) goto EVALUATE_EXTENT;

            const double    g0 = rmsGamma(Corg);
            const double    x1 = rstack.front();
            triplet<double> x  = { 0,  -1, x1 };
            triplet<double> g  = { g0, -1, (*this)(x.c)  };


            if(true)
            {
                ios::ocstream fp("gam0.dat");
                const size_t NP   = 1000;
                const double xmax = x1;
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = (xmax * i)/NP;
                    fp("%g %g\n",u,(*this)(u));
                }
            }

            minimize::find<double>::run_for((*this), x, g, minimize::inside);
            const double g1 = g.b;
            std::cerr << "g(" << x.b << ")=" << g1 << " / " << g0 << std::endl;


            if(true)
            {
                ios::ocstream fp("gam1.dat");
                const size_t NP = 1000;
                const double xmax = min_of(2*x.b,x1);
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = (xmax * i)/NP;
                    fp("%g %g\n",u,(*this)(u));
                }
            }

            transfer(Corg, Ctry);

            if(cycle>=10)
            {
                exit(1);
            }

            goto CYCLE;
#endif

            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }


#if 0
        bool plexus:: solve(writable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            assert(are_valid(C0));


            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// <plexus.solve>");
            if(verbose) lib(std::cerr << vpfx << "Cini=",C0,vpfx);
            ios::ocstream::overwrite("rms.dat");
            switch(N)
            {

                case 0:
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [empty]");
                    return true;

                case 1: {
                    //----------------------------------------------------------
                    // trivial case
                    //----------------------------------------------------------
                    const equilibrium &eq = ***eqs.head();
                    (void) eq.solve1D(K[*eq],C0,Ctry);
                    transfer(C0,Ctry);
                }
                    YACK_CHEM_PRINTLN("// <plexus.solve/> [1D]");
                    return true;


                default:
                    //----------------------------------------------------------
                    // prepare workspace
                    //----------------------------------------------------------
                    for(size_t j=M;j>0;--j)
                    {
                        Corg[j] = Ctry[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }



            //plexus &self  = *this;
            size_t  cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // regularize @Corg
            //
            //------------------------------------------------------------------
            regularize();


            //------------------------------------------------------------------
            //
            // compute Omega @Corg
            //
            //------------------------------------------------------------------
            buildOmega();



        EVAL_XI:
            //------------------------------------------------------------------
            //
            // compute guess extent
            //
            //------------------------------------------------------------------
            if(verbose)
            {
                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "rhs  =" << xm     << std::endl;
            }

            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {

                YACK_CHEM_PRINTLN("// <plexus.solve/> [singular]");
                return false;
            }
            tao::v1::set(xi,xm);
            LU.solve(iOmega,xi);
            if(verbose)
            {
                eqs(std::cerr<<vpfx<<"xi     = ",xi,vpfx);
            }

            //------------------------------------------------------------------
            //
            // use primary control
            //
            //------------------------------------------------------------------
            if(primaryCut()) goto EVAL_XI;

            


            //------------------------------------------------------------------
            //
            // compute dC
            //
            //------------------------------------------------------------------
            if(!compute_dC())
                goto EVAL_XI;

            if(verbose)
            {
                std::cerr << "Omega=" << Omega0 << std::endl;
                lib(std::cerr<<vpfx<<"dC     = ",dC,vpfx);
                lib(std::cerr<<vpfx<<"Corg   = ",Corg,vpfx);
            }


            {
                size_t NP = 10000;
                ios::ocstream fp("gam.dat");
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = (1.0*i)/NP;
                    fp("%g %.15g\n",u,(*this)(u));
                }
            }


            //------------------------------------------------------------------
            //
            // minimize over dC, since dC is expensive!!
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("// at cycle #" << cycle);
            if(!tryFindEqs())
            {
                goto CYCLE;
            }

            //------------------------------------------------------------------
            //
            // success!
            //
            //------------------------------------------------------------------
            transfer(C0,Corg);
            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }
#endif

    }
}

