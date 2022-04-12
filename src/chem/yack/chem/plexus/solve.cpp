

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
                Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
                //Ctry[j] = max_of( (1.0-u) * Corg[j] + u * Cend[j], 0.0 );
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

        bool plexus:: solve(writable<double> &C0) throw()
        {
            assert(C0.size()>=M);
            assert(are_valid(C0));
            static const double XSAFE = 1.1;

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

            vector<size_t>  ix(N,0);
            vector<enode *> en(N,0);

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // predictor global step
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &Ci  = Ceq[ei];
                Xi[ei]    = eq.solve1D(Ki,Corg,Ci);
                en[ei]    = (enode *)node;
            }

            const double rms = sorted::mod2(Xi,xs) / N;
            ios::ocstream::echo("rms.dat", "%g %.15g\n", double(cycle), rms);

            indexing::make(ix,comparison::decreasing_abs<double>,Xi);
            eqs(std::cerr<<vpfx<<"Xi_p=",Xi,vpfx);
            std::cerr << "ix=" << ix << std::endl;

            {
                // updating chosen
                const size_t       ii = ix[1];
                const enode       *ep = en[ii];
                const equilibrium &eq = ***ep;
                const size_t       ei = *eq;
                std::cerr << "moving " << eq.name << std::endl;
                transfer(Corg,Ceq[ei]);
                Xi[ei] = 0;
                writable<double> &psi = Psi[ei];
                eq.drvs_action(psi,K[ei],Corg,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    blocked[ei] = true;
                }

                // updating other ones
                for(const enode *node=ep->prev;node;node=node->prev) update(node);
                for(const enode *node=ep->next;node;node=node->next) update(node);
            }

            lib(std::cerr<<vpfx<<"C_p=",Corg,vpfx);
            eqs(std::cerr<<vpfx<<"Xi_c=",Xi,vpfx);
            eqs(std::cerr<<vpfx<<"blocked=",blocked,vpfx);

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
                    xd[i]  = 0;
                }
                else
                {
                    const double den = sorted::dot(psi,Nu[i],Ctmp); assert(den<0);
                    Omi[i] = 1.0;
                    Gs[i]  = fabs(den);
                    xm[i]  = 0;
                    for(size_t k=N;k>i;--k)   xm[k] = fabs( Omi[k] = sorted::dot(psi,Nu[k],Ctmp)/den );
                    for(size_t k=i-1;k>0;--k) xm[k] = fabs( Omi[k] = sorted::dot(psi,Nu[k],Ctmp)/den );
                    xd[i] = sorted::sum(xm,sorted::by_value);
                }
            }
            eqs(std::cerr<<vpfx<<"Omega=",Omega0,vpfx);
            eqs(std::cerr<<vpfx<<"Extra=",xd,vpfx);

            {
                double wmax = 1;
                for(size_t i=N;i>0;--i)
                {
                    const double xtra = XSAFE * xd[i];
                    if(xtra>=1)
                    {
                        wmax = max_of( Omega0[i][i] = timings::round_ceil(xtra), wmax );
                    }
                }
                std::cerr << "wmax=" << wmax << std::endl;
                for(size_t i=N;i>0;--i)
                {
                    Omega0[i][i] = wmax;
                }
            }


            std::cerr << "Omega=" << Omega0 << std::endl;
        EVAL_XI:
            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singluar]");
                return false;
            }

            tao::v1::set(xi,Xi);
            LU.solve(iOmega,xi);

            eqs(std::cerr<<vpfx<<"xi=",xi,vpfx);

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
                    }
                }
                YACK_CHEM_PRINTLN("//   <plexus.primaryCut/> [changed=" << changed << "]");

                if(changed)
                {
                    for(size_t i=N;i>0;--i)
                    {
                        Omega0[i][i] *= 2;
                    }
                    goto EVAL_XI;
                }
            }


            {
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

                std::cerr << "rstack=" << rstack << std::endl;
                YACK_CHEM_PRINTLN("//   <plexus.computeDeltaC>");
                if(rstack.size())
                {
                    for(size_t i=N;i>0;--i)
                    {
                        Omega0[i][i] *= 2;
                    }
                    goto EVAL_XI;
                }
            }

            lib(std::cerr<<vpfx<<"dC=",dC,vpfx);

            double expand = 1;
            {
                rstack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    const double   d = dC[j];
                    if(d<0)
                    {
                        const double c = Corg[j]; assert(c>=0);
                        assert(-d<=c);
                        rstack << c/(-d);
                    }
                }
                hsort(rstack,comparison::increasing<double>);
                while(rstack.size()>1) rstack.pop_back();
                std::cerr << "expand=" << rstack << std::endl;
                if(rstack.size()) expand = rstack.front();
            }


            const double g0 = rmsGamma(Corg);
            std::cerr << "g0=" << g0 << std::endl;
            triplet<double> x = {  0, -1, expand };
            triplet<double> g = { g0, -1, (*this)(x.c) };
            std::cerr << "g1=" << g.c << std::endl;

            if(true)
            {
                ios::ocstream fp("gam.dat");
                const size_t NP = 1000;
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = (x.c*i)/NP;
                    fp("%.15g %.15g\n",u,(*this)(u));
                }
            }

            minimize::find<double>::run_for(*this,x,g,minimize::inside);
            std::cerr << "g(" << x.b << ")=" << g.b << std::endl;

            transfer(Corg,Ctry);
            if(cycle>=30)
            {
                exit(1);
            }
            goto CYCLE;



            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }



    }
}

