

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/math/tao/v2.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include "yack/sort/indexing.hpp"
#include <cmath>
#include <iomanip>

namespace yack
{
    using  namespace math;

    namespace chemical
    {

        const char plexus::vpfx[] = "//   ";

        template <typename T>
        static inline double xdot(const readable<double> &lhs, const readable<T> &rhs, writable<double> &tmp)
        {
            const size_t n = lhs.size();
            for(size_t i=n;i>0;--i)
            {
                tmp[i] = lhs[i] * rhs[i];
            }
            return sorted::sum(tmp,sorted::by_abs_value);
        }




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

        void plexus:: buildOmega() throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega>");

            for(const enode *node=eqs.head();node;node=node->next)
            {
                // prepare
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Omi = Omega0[ei];
                writable<double>  &psi = Psi[ei];

                // initialize
                Omi.ld(0.0);
                Omi[ei] = 1.0;
                if(blocked[ei])
                {
                    // default values
                    Gs[ei] = 1.0;
                    xm[ei] = 0.0;
                }
                else
                {
                    // compute extra-diag terms
                    const double Ki  = K[ei];
                    xm[ei] = eq.grad_action(psi,Ki,Corg,Ctmp);
                    const double den = xdot(psi,Nu[ei],Ctmp); assert(den<0);
                    xm[ei] /= ( Gs[ei]=-den );
                    xs[ei] = 0;
                    for(size_t k=N;  k>ei;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/den);
                    for(size_t k=ei-1;k>0;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/den);
                    const double extra = sorted::sum(xs,sorted::by_value);
                    (void) extra;
                    // TODO: regularize ?
                }
            }

            if(verbose)
            {
                eqs(std::cerr<<vpfx<<"Gs    = ",Gs,vpfx);
                eqs(std::cerr<<vpfx<<"Omega = ",xm,vpfx);
                eqs(std::cerr<<vpfx<<"rhs   = ",xm,vpfx);
            }
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega/>");

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
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
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
                Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
                //Ctry[j] = max_of( (1.0-u) * Corg[j] + u * Cend[j], 0.0 );
            }
            return rmsGamma(Ctry);
        }

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

            plexus        &G = *this;
            vector<size_t> ix(N,0);

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                xm[ei] = Xi[ei] = eq.solve1D(K[ei],Corg,Ceq[ei]);
            }

            indexing::make(ix,comparison::decreasing_abs<double>,Xi);
            const double rms = sorted::sum_squared(xm)/N;
            ios::ocstream::echo("rms.dat", "%g %.15g\n",double(cycle),rms);
            std::cerr << "rms=" << rms << std::endl;

            const size_t ii = ix[1];
            transfer(Corg,Ceq[ii]);
            eqs(std::cerr << vpfx << "Xi=",Xi,vpfx);
            eqs(std::cerr << vpfx << "ix=",ix,vpfx);
            lib(std::cerr << vpfx << "C=",Corg,vpfx);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &psi =  Psi[ei];
                Gamma[ei] = eq.grad_action(psi,K[ei],Corg,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    blocked[ei] = true;
                    Gamma[ei]   = 0;
                }
                else
                {
                    blocked[ei] = false;
                }
            }

            Gamma[ii] = 0;


            eqs(std::cerr << vpfx << "Gamma=",Gamma,vpfx);
            eqs(std::cerr << vpfx << "Psi=",Psi,vpfx);
            eqs(std::cerr << vpfx << "Blocked=",blocked,vpfx);


            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];

                Omi.ld(0);
                if(blocked[i])
                {
                    Omi[i] = 1.0;
                    Gs[i]  = 1.0;
                }
                else
                {
                    for(size_t k=N;k>0;--k) Omi[k] = -xdot(psi,Nu[k],Ctmp);
                    Gs[i] = fabs(Omi[i]);
                }
            }

            eqs(std::cerr << vpfx << "Gs=",Gs,vpfx);


        EVAL_XI:
            std::cerr << "Omega  =" << Omega0 << std::endl;
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singluar]");
                return false;
            }

            tao::v1::set(xi,Gamma);
            LU.solve(iOmega,xi);
            eqs(std::cerr << vpfx << "xi=",xi,vpfx);

            if( primaryCut()) goto EVAL_XI;
            if(!compute_dC()) goto EVAL_XI;

            const double g0 = rmsGamma(Corg);
            std::cerr << "g0=" << g0 << std::endl;

            rstack.free();
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j];
                if(d<0)
                {
                    const double c = Corg[j]; assert(c>=0);
                    assert((-d)<c);
                    rstack << c/(-d);
                }
            }

            double expand = 1.0;
            if(rstack.size())
            {
                hsort(rstack,comparison::increasing<double>);
                const double xmax = min_of(100.0,rstack.front());
                std::cerr << "xmax=" << xmax << std::endl;
                expand = 1.0+(xmax-1.0)/2;
            }
            else
            {
                std::cerr << "unlimited" << std::endl;
                while(G(expand)<g0) expand *= 2;
            }

            std::cerr << "expand=" << expand << std::endl;

            if(true)
            {
                static const size_t NP = 1000;
                ios::ocstream fp("gam.dat");
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = expand*double(i)/NP;
                    fp("%g %.15g\n",u,G(u));
                }
            }

            triplet<double> x = { 0,  -1, expand };
            triplet<double> g = { g0, -1, G(x.c) };

            (void) minimize::find<double>::run_for(G,x,g,minimize::inside);
            std::cerr << "G("<< x.b << ")=" << g.b << std::endl;

            transfer(Corg,Ctry);

            if(cycle>=5) exit(1);

            goto CYCLE;


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

