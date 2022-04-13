

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

#if 0
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
#endif

        
        double plexus:: rmsGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            for(size_t i=N;i>0;--i)
            {
                //xs[i] = squared(Gamma[i]/Gs[i]);
                xs[i] = squared(Gamma[i]);
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

#if 0
        void plexus:: update(const enode *node) throw()
        {
            const equilibrium &eq  = ***node;
            const size_t       ei  = *eq;
            const double       Ki  = K[ei];
            writable<double>  &Ci  = Ceq[ei];
            writable<double>  &psi = Psi[ei];
            Xi[ei] = eq.solve1D(Ki,Corg,Ci);
            eq.drvs_action(psi,Ki,Ci,Ctmp);
            if( tao::v1::mod2<double>::of(psi,Ctmp) <= 0)
            {
                blocked[ei] = true;
                Xi[ei]      = 0;
            }
            else
            {
                blocked[ei] = false;
            }
        }
#endif

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
                        Corg[j] = Ctry[j] = Cend[j] = C0[j];
                        dC[j]   = 0;
                    }
                    break;
            }

            vector<size_t>       ix(N,0);
            vector<equilibrium*> en(N,NULL);

            size_t cycle=0;
        CYCLE:
            ++cycle;
            double g0 = rmsGamma(Corg);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];
                Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                en[ei] = (equilibrium *)&eq;
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    Xi[ei]      = 0;
                    xd[ei]      = g0;
                    std::cerr << eq.name << " is blocked" << std::endl;
                }
                else
                {
                    if(fabs(Xi[ei])>0)
                    {
                        transfer(Cend,Ci);
                        triplet<double> x = { 0, -1, 1 };
                        triplet<double> g = { g0, -1, rmsGamma(Cend) };
                        (void) minimize::find<double>::run_for(*this,x,g,minimize::inside);
                        xd[ei] = g.b;
                        eqs.pad(std::cerr << "g_" << eq.name,eq) << " = " << std::setw(14) << g.b << " @" << x.b << std::endl;
                        transfer(Ci,Ctry);
                    }
                    else
                    {
                        xd[ei] = g0;
                        std::cerr << eq.name << " is steady" << std::endl;
                    }
                }

            }

            eqs(std::cerr<<vpfx<<"blocked = ",blocked,vpfx);
            eqs(std::cerr<<vpfx<<"Xi_p    = ",Xi,vpfx);
            eqs(std::cerr<<vpfx<<"g       = ",xd,vpfx);
            std::cerr << "g0=" << g0 << std::endl;

            indexing::make(ix,comparison::increasing<double>,xd);
            {
                const size_t        ii = ix[1];
                const equilibrium  &eq = *en[ii];
                std::cerr << "[[ Moving " << eq.name << " ]]" << std::endl;
                transfer(Corg,Ceq[ii]);
                const double g1 = xd[ii];
                if(g1>=g0)
                {
                    std::cerr << "BAD!!" << std::endl;
                    exit(1);
                }
                g0 = g1;
            }

            if(cycle>=10)
            {
                exit(1);
            }

            goto CYCLE;


            // correction
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];
                Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    Xi[ei]      = 0;
                    blocked[ei] = true;
                }
                else
                {
                    blocked[ei] = false;
                }
            }

            eqs(std::cerr<<vpfx<<"Xi_c    = ",Xi,vpfx);



            exit(1);

#if 0

            size_t cycle=0;
            ios::ocstream::echo("rms.dat","%.15g %.15g\n", double(cycle), rmsGamma(Corg));
        CYCLE:
            ++cycle;
            //  regularizing
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                eq.drvs_action(psi,Ki,Corg,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    std::cerr << "trying to move " << eq.name << std::endl;
                    eq.solve1D(Ki,Corg,Cend);
                    transfer(Corg,Cend);
                    eq.drvs_action(psi,Ki,Corg,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0)
                    {
                        blocked[ei] = true;
                    }
                }
                else
                {
                    blocked[ei] = false;
                }
            }

            eqs(std::cerr<<vpfx<<"blocked = ",blocked,vpfx);

            // recomputing
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &psi = Psi[ei];
                writable<double>  &Omi = Omega0[ei];

                if(blocked[ei])
                {
                    Omi.ld(0.0);
                    Omi[ei]   = 1.0;
                    Gamma[ei] = 0.0;
                }
                else
                {
                    Gamma[ei] = eq.grad_action(psi,K[ei],Corg,Ctmp);
                    for(size_t k=N;k>0;--k)
                    {
                        Omi[k] = -sorted::dot(psi,Nu[k],Ctmp);
                    }
                }
            }
            std::cerr << "Gamma=" << Gamma << std::endl;
            std::cerr << "Psi  =" << Psi   << std::endl;


            //------------------------------------------------------------------
            //
            // Evaluate extent
            //
            //------------------------------------------------------------------
        EVAL_XI:
            std::cerr << "Omega=" << Omega0 << std::endl;
            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singluar]");
                return false;
            }

            tao::v1::set(xi,Gamma);
            LU.solve(iOmega,xi);

            eqs(std::cerr<<vpfx<<"xi=",xi,vpfx);

            //------------------------------------------------------------------
            //
            // primary cut
            //
            //------------------------------------------------------------------
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
                        changed = true;
                        YACK_CHEM_PRINTLN(vpfx<<" (*) need to reduce <" << eq.name << ">");
                        Omega0[ei][ei] *= 10;
                    }
                }
                YACK_CHEM_PRINTLN("//   <plexus.primaryCut/> [changed=" << changed << "]");

                if(changed)
                {
                    goto EVAL_XI;
                }
            }

            //------------------------------------------------------------------
            //
            // compute and check all delta C
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC>");
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
                goto EVAL_XI;
            }
            YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC/> [ok!]");

            //------------------------------------------------------------------
            //
            // compute max amplification
            //
            //------------------------------------------------------------------
            {
                double expand = 1;
                rstack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   d = dC[j];
                    if(d<0)
                    {
                        const double c = Corg[j]; assert(c/(-d)>=1);
                        rstack << c/(-d);
                    }
                }
                if(rstack.size())
                {
                    hsort(rstack,comparison::increasing<double>);
                    std::cerr << "rstack=" << rstack << std::endl;
                    const double xmax = rstack.front();
                    expand = min_of(1.0 + 0.9*(xmax-1.0),100.0);
                }
                std::cerr << "expand=" << expand << std::endl;

                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    Cend[j] = Corg[j] + expand * dC[j];
                }

                if(true)
                {
                    ios::ocstream fp("gam.dat");
                    const size_t NP = 1000;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double u = double(i)/NP;
                        fp("%.15g %.15g\n", u, (*this)(u));
                    }
                }

                {
                    const double    g0 = rmsGamma(Corg);
                    triplet<double> x = { 0, -1,1 };
                    triplet<double> g = { g0,-1,rmsGamma(Cend) };
                    (void) minimize::find<double>::run_for(*this,x,g,minimize::inside);
                    const double g1 = g.b;
                    std::cerr << "@cycle" << cycle << " : g = " << g0 << " -> " << g1 << std::endl;
                    ios::ocstream::echo("rms.dat","%.15g %.15g\n", double(cycle), g1);
                }
            }



            transfer(Corg,Ctry);
            lib(std::cerr<<vpfx<<"Corg=",Corg,vpfx);

            if(cycle>=20)
            {
                exit(1);
            }
            goto CYCLE;


#endif

            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }



    }
}

