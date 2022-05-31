

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


        double plexus:: rmsGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            for(size_t i=N;i>0;--i)
            {
                xs[i] = squared(Gamma[i]*Gs[i]);
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


        static inline bool is_null(const readable<double> &vec) throw()
        {
            for(size_t i=vec.size();i>0;--i)
            {
                if( fabs(vec[i]) > 0 ) return false;
            }
            return true;
        }

        void plexus:: regularize() throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.regularize>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                double            &gam = Gamma[ei];
                gam = eq.grad_action(psi, Ki, Corg, Ctmp);
                if( is_null(psi) )
                {
                    (void) eq.solve1D(Ki, Corg, Ctry);
                    transfer(Corg,Ctry);
                }
            }
            if(verbose) lib(std::cerr << vpfx << "Creg=",Corg,vpfx);
            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }

        void plexus:: blockEq(const size_t ei) throw()
        {
            assert(!blocked[ei]);

            blocked[ei] = true;
            Psi[ei].ld(0);
            NuAT.ld_col(ei,0);
            writable<double> &Omi = Omega0[ei];
            Omi.ld(0);
            Omi[ei]   = 1.0;
            Gs[ei]    = 0.0;
            Xi[ei]    = 0;
            Gamma[ei] = 0;
        }

        double plexus:: rawOpt(const double G0) throw()
        {
            triplet<double> u = {0,-1,1};
            triplet<double> g = {G0,-1,rmsGamma(Cend)};
            //tao::v1::set(Cend,Cusr);
            (void)minimize::find<double>::run_for(*this,u,g,minimize::inside);
            return g.b;
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


            rmatrix        Copt(NPR,M);
            vector<double> Xopt(NPR,0);
            vector<double> Gopt(NPR,0);

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("//   -------- CYCLE=" << cycle << " --------");
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);


            // select reaction
            double             absXiMax = 0;
            double             sumAbsXi = 0;
            const equilibrium *eOpt = NULL;
            {
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;      // equilibrium
                    const size_t       ei = *eq;          // index
                    const double       Ki  = K[ei];       // current constant
                    Xi[ei] = eq.solve1D(Ki,Corg,Ceq[ei]);
                    Gs[ei] = 1.0;
                    const double atmp = fabs(Xi[ei]);
                    sumAbsXi += atmp;
                    if(atmp>absXiMax)
                    {
                        absXiMax = atmp;
                        eOpt     = &eq;
                    }
                }
                eqs(std::cerr << "Xi=",Xi);
            }
            std::cerr << "|Xi|=" << sumAbsXi << std::endl;

            if(sumAbsXi<=0)
            {
                tao::v1::set(C0,Corg);
                YACK_CHEM_PRINTLN("// <plexus.solve> [SUCCESS]");
                return true;
            }

            if(cycle>=5)
            {
                exit(1);
            }

            assert(sumAbsXi>0);
            assert(absXiMax>0);
            assert(eOpt!=NULL);

            std::cerr << "using root=" << eOpt->name << std::endl;
            const double G0 = rmsGamma(Corg);
            std::cerr << "G0=" << G0 << std::endl;

            {
                const size_t iOpt = **eOpt;
                Xopt[1] = Xi[iOpt];
                tao::v1::set(Cend,Ceq[iOpt]);
                double Gmin = Gopt[1] = rawOpt(G0);
                size_t imin = 1;
                tao::v1::set(Copt[1],Ctry);

                std::cerr << " (*) " << eOpt->name << " only : " << Gopt[1] << " @" << Copt[1] << std::endl;

                size_t npr = 1;
                for(const mixed *mx=pre[iOpt].head;mx;mx=mx->next)
                {
                    ++npr;
                    Xopt[npr] = mx->solve1D(mx->value,Corg,Cend);
                    const double Gtmp = Gopt[npr] = rawOpt(G0);
                    tao::v1::set(Copt[npr],Ctry);
                    std::cerr << " (*) " << mx->name << " : " << Gopt[npr] << " @" << Copt[npr] << std::endl;
                    if(Gtmp<Gmin)
                    {
                        Gmin = Gtmp;
                        imin = npr;
                    }
                }

                std::cerr << "Gmin=" << Gmin <<", imin=" << imin << std::endl;
                tao::v1::set(Corg,Copt[imin]);
            }

            // local step with rounding control by Xi
            YACK_CHEM_PRINTLN("//   <plexus.omega>");
            size_t num_blocked = 0;
            size_t num_running = N;
            NuAT.assign(NuT);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;      // equilibrium
                const size_t       ei = *eq;          // index
                const double       Ki  = K[ei];       // current constant
                writable<double>  &psi = Psi[ei];     // gradient
                writable<double>  &Cei = Ceq[ei];     // C @ equilibrium
                writable<double>  &Omi = Omega0[ei];  // Omega[ei]
                double            &gam = Gamma[ei];   // Gamma[ei]
                double            &xxx = Xi[ei];      // Xi[ei]

                blocked[ei] = false;
                gam         = eq.grad_action(psi,Ki,Corg,Ctmp);  // compute Gamma[ei] and Psi[ei]
                xxx         = eq.solve1D(Ki,Corg,Cei);           // compute Xi[ei] and equilibrium Ceqi
                const double axx = fabs(xxx);                    // compute |Xi|
                const double Oii = sorted::dot(psi,Nu[ei],Ctmp); // Omega[ei][ei]
                if(verbose) {
                    eqs.pad(std::cerr << vpfx << "  <" << eq.name << ">",eq) << " :";
                }
                if( Oii>=0 )
                {
                    // blocked equilibrium
                    blockEq(ei);
                    ++num_blocked;
                    --num_running;
                    if(verbose) std::cerr << " [blocked]";
                }
                else
                {
                    // regular equilibrium
                    assert(Oii<0);
                    Gs[ei]      = 1.0;
                    if(axx<=0) gam=0; // roundoff correction
                    Omi[ei]     = Oii;
                    for(size_t k=N;k>ei;--k)   Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = sorted::dot(psi,Nu[k],Ctmp);

                    if(verbose) std::cerr << " Gamma=" << std::setw(14) << gam << "; Xi=" << std::setw(14) << xxx;
                }

                if(verbose) std::cerr << std::endl;
            }
            YACK_CHEM_PRINTLN("//   <plexus.omega/>");


            // evaluate step

            exit(1);

            goto CYCLE;

#if 0

            size_t cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("//   -------- CYCLE=" << cycle << " --------");
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);

            // global step

            {
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;      // equilibrium
                    const size_t       ei = *eq;          // index
                    const double       Ki  = K[ei];       // current constant
                    Xi[ei] = eq.solve1D(Ki,Corg,Ceq[ei]);
                    Gs[ei] = 1.0;
                }
                eqs(std::cerr << "Xi=",Xi);
                rmatrix          &Copt = Psi;
                writable<double> &Gopt = xd;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;      // equilibrium
                    const size_t       ei = *eq;          // index
                    transfer(Cend,Ceq[ei]);

                    triplet<double> u = { 0,-1,1};
                    triplet<double> g = { (*this)(u.a), -1, (*this)(u.c) };
                    minimize::find<double>::run_for(*this, u, g, minimize::inside);
                    Gopt[ei] = g.b;
                    transfer(Copt[ei],Ctry);
                    std::cerr << eq.name << " " << Gopt[ei] << "@" << u.b << "; " << Ctry << std::endl;
                }
            }

            // local step with rounding control by Xi
            YACK_CHEM_PRINTLN("//   <plexus.omega>");
            size_t num_blocked = 0;
            size_t num_running = N;
            NuAT.assign(NuT);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;      // equilibrium
                const size_t       ei = *eq;          // index
                const double       Ki  = K[ei];       // current constant
                writable<double>  &psi = Psi[ei];     // gradient
                writable<double>  &Cei = Ceq[ei];     // C @ equilibrium
                writable<double>  &Omi = Omega0[ei];  // Omega[ei]
                double            &gam = Gamma[ei];   // Gamma[ei]
                double            &xxx = Xi[ei];      // Xi[ei]

                blocked[ei] = false;
                gam         = eq.grad_action(psi,Ki,Corg,Ctmp);  // compute Gamma[ei] and Psi[ei]
                xxx         = eq.solve1D(Ki,Corg,Cei);           // compute Xi[ei] and equilibrium Ceqi
                const double axx = fabs(xxx);                    // compute |Xi|
                const double Oii = sorted::dot(psi,Nu[ei],Ctmp); // Omega[ei][ei]
                if(verbose) {
                    eqs.pad(std::cerr << vpfx << "  <" << eq.name << ">",eq) << " :";
                }
                if( Oii>=0 )
                {
                    // blocked equilibrium
                    blockEq(ei);
                    ++num_blocked;
                    --num_running;
                    std::cerr << " [blocked]";
                }
                else
                {
                    // regular equilibrium
                    assert(Oii<0);
                    Gs[ei]      = 1.0;
                    if(axx<=0) gam=0; // roundoff correction
                    Omi[ei]     = Oii;
                    for(size_t k=N;k>ei;--k)   Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = sorted::dot(psi,Nu[k],Ctmp);

                    std::cerr << " Gamma=" << std::setw(14) << gam << "; Xi=" << std::setw(14) << xxx;
                }

                if(verbose) std::cerr << std::endl;
            }
            YACK_CHEM_PRINTLN("//   <plexus.omega/>");




#if 0
            hsort(xd, ix, comparison::decreasing<double>);
            for(size_t k=1;k<=N;++k)
            {
                std::cerr << ix[k]->name << " -> " << Xi[ **ix[k] ] << std::endl;
            }

            {
                const equilibrium &eq = *ix[1];
                const size_t       ii = *eq;
                std::cerr << "Using <" << eq.name << ">" << std::endl;
                transfer(Cend,Ceq[ii]);
                std::cerr << "Cend=" << Cend << std::endl;

                {
                    ios::ocstream fp("gam.dat");
                    const size_t NP=10000;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double u =  double(i)/NP;
                        fp("%g %g\n",u, sqrt( (*this)(u) ) );
                    }
                }

                triplet<double> u = { 0,-1,1};
                triplet<double> g = { (*this)(u.a), -1, (*this)(u.c) };
                minimize::find<double>::run_for(*this, u, g, minimize::inside);

                std::cerr << g.b << "@" << u.b << std::endl;



            }
#endif

            exit(1);
            YACK_CHEM_PRINTLN("// <plexus.solve> [success]");
            return true;
#endif

#if 0
            //------------------------------------------------------------------
            //
            // one-time regularize
            //
            //------------------------------------------------------------------
            regularize();


            size_t cycle = 0;
        CYCLE:
            ++cycle;
            YACK_CHEM_PRINTLN("//   -------- CYCLE=" << cycle << " --------");
            if(verbose) lib(std::cerr << vpfx << "Corg=",Corg,vpfx);
            //------------------------------------------------------------------
            //
            // compute full state
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.omega>");
            size_t num_blocked = 0;
            size_t num_running = N;
            double sumAbsXi    = 0;
            NuAT.assign(NuT);

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;      // equilibrium
                const size_t       ei = *eq;          // index
                const double       Ki  = K[ei];       // current constant
                writable<double>  &psi = Psi[ei];     // gradient
                writable<double>  &Cei = Ceq[ei];     // C @ equilibrium
                writable<double>  &Omi = Omega0[ei];  // Omega[ei]
                double            &gam  = Gamma[ei];  // Gamma[ei]
                double            &xxx = Xi[ei];      // Xi[ei]
                
                blocked[ei] = false;
                gam         = eq.grad_action(psi,Ki,Corg,Ctmp);  // compute Gamma[ei] and Psi[ei]
                xxx         = eq.solve1D(Ki,Corg,Cei);           // compute Xi[ei] and equilibrium Ceqi
                const double axx = fabs(xxx); sumAbsXi += axx;   // update |Xi|
                const double Oii = sorted::dot(psi,Nu[ei],Ctmp); // Omega[ei][ei]
                if(verbose) {
                    eqs.pad(std::cerr << vpfx << "  <" << eq.name << ">",eq) << " :";
                }
                if( Oii>=0 )
                {
                    // blocked equilibrium
                    blockEq(ei);
                    ++num_blocked;
                    --num_running;
                    std::cerr << " [blocked]";
                }
                else
                {
                    // regular equilibrium
                    assert(Oii<0);
                    Gs[ei]      = 1.0/(-Oii);
                    if(axx<=0) gam=0; // roundoff correction
                    Omi[ei]     = Oii;
                    for(size_t k=N;k>ei;--k)   Omi[k] = sorted::dot(psi,Nu[k],Ctmp);
                    for(size_t k=ei-1;k>0;--k) Omi[k] = sorted::dot(psi,Nu[k],Ctmp);

                    std::cerr << " Gamma=" << std::setw(14) << gam << "; Xi=" << std::setw(14) << xxx;
                }

                if(verbose) std::cerr << std::endl;
            }
            YACK_CHEM_PRINTLN("//   <plexus.omega/>");


            if(sumAbsXi<=0)
            {
                transfer(C0,Corg);
                YACK_CHEM_PRINTLN("// <plexus.solve> [success |Xi|=0]");
                return true;
            }

        EVALUATE_XI:
            YACK_CHEM_PRINTLN("//   <plexus.xi>");
            //------------------------------------------------------------------
            //
            // evaluate xi from Omega0 and gam
            //
            //------------------------------------------------------------------
            {
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    eqs.pad(std::cerr << vpfx << "<" << eq.name << ">",eq) << ":";
                    if(blocked[ei])
                    {
                        std::cerr << " [blocked]";
                    }
                    else
                    {
                        std::cerr << " Gamma=" << std::setw(14) << Gamma[ei] << "; Xi=" << std::setw(14) << Xi[ei];
                    }
                    std::cerr << "; Omega=" << Omega0[ei];

                    std::cerr << std::endl;
                }
                std::cerr << "Omega=" << Omega0 << std::endl;

            }

            iOmega.assign(Omega0);
            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("//   <plexus.xi/> [failure]");
                YACK_CHEM_PRINTLN("// <plexus.solve> [SINGULAR]");
                return false;
            }

            tao::v1::neg(xi,Gamma);
            LU.solve(iOmega,xi);
            eqs(std::cerr << vpfx << "xi     =",     xi,vpfx);


            //------------------------------------------------------------------
            //
            // check xi
            //
            //------------------------------------------------------------------
            {
                bool recompute = false;
                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    double            &xx = xi[ei]; if(blocked[ei]) { xx = 0.0; continue; }
                    const limits      &lm = eq.primary_limits(Corg,lib.width);

                    if(verbose) eqs.pad(std::cerr << vpfx << "  (*) " << eq.name, eq) << " : " << lm << " | =>  ";
                    if(lm.should_reduce(xx))
                    {
                        if( verbose) std::cerr << "[REJECT " << xx << "]";
                        blockEq(ei);
                        recompute = true;
                        ++num_blocked;
                        --num_running;
                    }
                    else
                    {
                        if(verbose) std::cerr << "<ACCEPT   " << xx << ">";
                    }
                    if(verbose) std::cerr << std::endl;
                }

                if(recompute)
                {
                    YACK_CHEM_PRINTLN("//   <plexus.xi/> [recompute]");
                    goto EVALUATE_XI;
                }
                else
                {
                    YACK_CHEM_PRINTLN("//   <plexus.xi/> [validated]");
                }
            }

            YACK_CHEM_PRINTLN("//   <plexus.dC #running=" << num_running << "/" << N << ">");

            if(1==num_running)
            {
                //--------------------------------------------------------------
                //
                // the solution is known!
                //
                //--------------------------------------------------------------
                size_t ei = 0;
                for(ei=N;ei>0;--ei)
                {
                    if(!blocked[ei]) break;
                }
                assert(ei>=1&&ei<=N);
                transfer(Corg,Ceq[ei]);
                YACK_CHEM_PRINTLN("//   <plexus.dC/> [single]");
                goto CYCLE;
            }
            else
            {
                //--------------------------------------------------------------
                //
                // dC = NuAT * xi
                //
                //--------------------------------------------------------------
                std::cerr << "NuAT=" << NuAT << std::endl;
                std::cerr << "Gs  =" << Gs   << std::endl;

                rstack.free();
                ustack.free();
                for(const anode *node=active.head;node;node=node->next)
                {
                    const size_t   j = ***node;
                    const double   d = dC[j] = sorted::dot(xi,NuAT[j],xs);
                    const double   c = Corg[j]; assert(c>=0);
                    if(d<0)
                    {
                        rstack << c/(-d);
                        ustack << j;
                    }
                }

                std::cerr << "C =" << Corg  << std::endl;
                std::cerr << "dC=" << dC    << std::endl;
                double expand = 2.0;
                const size_t ncut = rstack.size();
                if(ncut>0)
                {
                    rstack.keep_only_front();
                    ustack.keep_only_front();
                    hsort(rstack,ustack, comparison::increasing<double> );
                    expand = min_of(expand,0.999*rstack[1]);
                }
                std::cerr << "rstack=" << rstack << std::endl;
                std::cerr << "ustack=" << ustack << std::endl;

                {
                    ios::ocstream fp("gam.dat");
                    const size_t NP=10000;
                    for(size_t i=0;i<=NP;++i)
                    {
                        const double u = expand*double(i)/NP;
                        fp("%g %g\n",u, sqrt( (*this)(u) ) );
                    }
                }

                const double g0 = rmsGamma(Corg);
                triplet<double> u = { 0,  -1, expand };
                triplet<double> g = { g0, -1, (*this)(expand) };

                (void) minimize::find<double>::run_for(*this,u,g,minimize::inside);
                const double g1 = g.b;
                std::cerr << "g: " << g0 << " => " << g1 << " @" << u.b << std::endl;

                if(g1>=g0)
                {
                    std::cerr << "NO GAIN!!!" << std::endl;
                    exit(1);
                }
                else
                {
                    transfer(Corg,Ctry);
                }

                YACK_CHEM_PRINTLN("//   <plexus.dC/> [multiple]");
            }

            if(cycle>=10)
                exit(1);

            goto CYCLE;


            YACK_CHEM_PRINTLN("// <plexus.solve> [success]");
            return true;
#endif

        }



    }
}

