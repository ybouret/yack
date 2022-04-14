

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
                xs[i] = squared(Gamma[i]*Gs[i]);
                //xs[i] = squared(Gamma[i]);
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

#if 0
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
#endif

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

            rmatrix H(M,M);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                eq.drvs_action(psi, Ki, Corg, Ctmp);
                eq.hessian(H, Ki, Corg, Ctmp);
                std::cerr << "Nu_"  << eq.name << " = " << Nu[ei]  << std::endl;
                std::cerr << "Psi_" << eq.name << " = " << Psi[ei] << std::endl;
                std::cerr << "H_"   << eq.name << " = " << H       << std::endl;
                std::cerr << "G_"    << eq.name << " = " << eq.mass_action(Ki,Corg) << std::endl;
            }

            std::cerr << "Nu=" << Nu << std::endl;

            exit(1);

            vector<bool> used(N,false);

            size_t cycle=0;
        CYCLE:
            ++cycle;

            used.ld(false);
            ustack.adjust(N,0);
            while(true)
            {
                //--------------------------------------------------------------
                //
                // build local metrics
                //
                //--------------------------------------------------------------
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    const double       Ki  = K[ei];
                    writable<double>  &psi = Psi[ei];
                    writable<double>  &Ci  = Ceq[ei];
                    Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                    eq.drvs_action(psi,Ki,Ci,Ctmp);
                    const double den = xd[ei] = sorted::dot(psi,Nu[ei],Ctmp); assert(den<=0);
                    if( fabs(den) <= 0)
                    {
                        Xi[ei]      = 0;
                        blocked[ei] = true;
                        Gs[ei]      = 0;
                    }
                    else
                    {
                        blocked[ei] = false;
                        Gs[ei]      = fabs(1.0/den);
                    }
                }

                //--------------------------------------------------------------
                //
                // try moving along main axis
                //
                //--------------------------------------------------------------
                eqs(std::cerr<<vpfx<<"Xi     = ",Xi,vpfx);
                eqs(std::cerr<<vpfx<<"Psi    = ",Psi,vpfx);
                eqs(std::cerr<<vpfx<<"Gs     = ",Gs,vpfx);

                const double g0 = rmsGamma(Corg);
                std::cerr << "g0=" << g0 << std::endl;

                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq  = ***node;
                    const size_t       ei  = *eq;
                    writable<double>  &Ci  = Ceq[ei];
                    if(blocked[ei]||used[ei])
                    {
                        score[ei] = g0;
                    }
                    else
                    {
                        transfer(Cend,Ci);
                        triplet<double> x = { 0,-1, 1 };
                        triplet<double> g = { g0,-1,rmsGamma(Cend) };
                        if(g.c>=g0)
                        {
                            // backtrack
                            (void) minimize::find<double>::run_for(*this,x,g,minimize::inside);
                            score[ei] = g.b;
                            transfer(Ci,Ctry);
                        }
                        else
                        {
                            // accept => Xi=0
                            score[ei] = g.c;
                        }
                    }
                }



                eqs(std::cerr << vpfx << "score=", score, vpfx);
                indexing::make(ustack, comparison::increasing<double>, score);
                const size_t i1 = ustack[1];
                const double g1 = score[i1];

                if(g1>=g0)
                {
                    std::cerr << "Cannot Make Better..." << std::endl;
                    break; // local topology is computed
                }

                std::cerr << "[[ moving <" << eqs[i1].name << "> ]]" << std::endl;
                transfer(Corg,Ceq[i1]);
                used[i1] = true;
            }
            lib(std::cerr << vpfx << "Corg =",Corg,vpfx);
            eqs(std::cerr<<vpfx<<"Xi     = ",Xi,vpfx);
            eqs(std::cerr<<vpfx<<"Psi    = ",Psi,vpfx);


            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium       &eq  = ***node;
                const size_t             ei  = *eq;
                //writable<double>        &Omi = Omega0[ei];
                const double             Ki  = K[ei];
                writable<double>        &psi = Psi[ei];
                Gamma[ei] = eq.grad_action(psi, Ki, Corg, Ctmp);
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

            for(size_t i=N;i>0;--i)
            {
                writable<double> &Omi = Omega0[i];
                if(blocked[i])
                {
                    Omi.ld(0);
                    Omi[i] = 1.0;
                }
                else
                {
                    const readable<double> &psi    = Psi[i];
                    for(size_t k=N;k>0;--k) Omi[k] = -sorted::dot(psi,Nu[k],Ctmp);
                }

                Gs[i] = 1.0;
            }

            std::cerr << "Gamma=" << Gamma  << std::endl;

        EVAL_XI:
            std::cerr << "Omega=" << Omega0 << std::endl;
            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                std::cerr << "Singular..." << std::endl;
                return false;
            }

            tao::v1::set(xi,Gamma);
            LU.solve(iOmega,xi);

            eqs(std::cerr<<vpfx<<"xi     = ",xi,vpfx);


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

            if(cycle>=100)
            {
                exit(1);
            }
            goto CYCLE;



            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }



    }
}

