

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/math/opt/minimize.hpp"
#include "yack/math/look-for.hpp"
#include <cmath>

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




        void plexus:: regularize(writable<double> &C0) throw()
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
                eq.drvs_action(psi,Ki,C0,Ctmp);
                if( tao::v1::mod2<double>::of(psi) <= 0)
                {
                    //----------------------------------------------------------
                    // move to a safer and still valid place
                    //----------------------------------------------------------
                    (void) eq.solve1D(Ki,C0,Ctry);
                    transfer(C0,Ctry);
                    
                    //----------------------------------------------------------
                    // second try
                    //----------------------------------------------------------
                    eq.drvs_action(psi,Ki,C0,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                    }
                }
            }

            if(verbose)
            {
                lib(std::cerr << vpfx << "Creg    = ",C0,vpfx);
                eqs(std::cerr << vpfx << "blocked = ",blocked,vpfx);
            }

            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }

        double plexus:: makeOmega0() throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega0>");

            double extra = 0;
            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];
                Omi.ld(0);
                Omi[i] = 1.0;
                if(!blocked[i])
                {
                    xs[i] = 0;
                    {
                        const double diag = xdot(psi,Nu[i],Ctmp); assert(diag<0);
                        Gscal[i] = -diag;
                        for(size_t k=N;  k>i;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/diag);
                        for(size_t k=i-1;k>0;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/diag);
                    }
                    extra = max_of(extra,sorted::sum(xs,sorted::by_value));
                }
                else
                {
                    Gscal[i] = 1.0;
                }
            }

            YACK_CHEM_PRINTLN("Gscal = " << Gscal);
            YACK_CHEM_PRINTLN("Omega = " << Omega0);
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega0/>");

            return extra;
        }

        double plexus:: rmsGamma(const readable<double> &C) throw()
        {
            computeGamma(C);
            for(size_t i=N;i>0;--i)
            {
                xs[i] = squared(Gamma[i]/Gscal[i]);
            }
            return sorted::sum(xs,sorted::by_value) / N;
        }

        double plexus:: operator()(const double u) throw()
        {
            for(const anode *node=active.head;node;node=node->next)
            {
                const size_t   j = ***node;
                Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
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

            size_t cycle = 0;

        CYCLE:
            ++cycle;
            //------------------------------------------------------------------
            //
            // regularize
            //
            //------------------------------------------------------------------

            regularize(Corg);


            //------------------------------------------------------------------
            //
            // compute metrics Psi@Corg
            //
            //------------------------------------------------------------------
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];

                if(blocked[ei])
                {
                    Psi[ei].ld(0);
                    Xi[ei] = 0;
                }
                else
                {
                    Xi[ei] = eq.solve1D(Ki,Corg,Ceq[ei]);
                    eq.drvs_action(Psi[ei],Ki,Corg,Ctmp);
                }
            }

            ios::ocstream::echo("rms.dat","%g %.15g\n", double(cycle), sqrt(tao::v1::mod2<double>::of(Xi)/N) );


            //------------------------------------------------------------------
            //
            // compute Omega
            //
            //------------------------------------------------------------------
            const double extra = makeOmega0();
            std::cerr << "extra=" << extra << std::endl;

        EVAL_XI:
            eqs(std::cerr << vpfx << "Omega=",Omega0,vpfx);
            iOmega.assign(Omega0);

            if(!LU.build(iOmega))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                return false;
            }

            tao::v1::set(xi,Xi);
            LU.solve(iOmega,xi);
            if(verbose) eqs(std::cerr << vpfx << "xi=",xi,vpfx);


            //------------------------------------------------------------------
            //
            // primary control if present
            //
            //------------------------------------------------------------------
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
                    std::cerr << "Should Reduce @" << eq.name << std::endl;
                }

            }
            if(changed)
            {
                goto EVAL_XI;
            }


            //------------------------------------------------------------------
            //
            // compute dC
            //
            //------------------------------------------------------------------
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
                        std::cerr << "underflow for " << s.name << std::endl;
                        for(size_t i=N;i>0;--i)
                        {
                            Omega0[i][i] *= 10;
                        }
                        goto EVAL_XI;
                    }
                }
            }

            if(verbose) lib(std::cerr << vpfx << "dC=",dC,vpfx);

            {
                ios::ocstream fp("gam.dat");
                for(double u=0;u<=1;u+=0.001)
                {
                    fp("%g %g\n",u,(*this)(u));
                }
            }


            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Ctry[j] = max_of(Corg[j]+dC[j],0.0);
            }
            transfer(Corg,Ctry);

            if(cycle>=1)
            {
                exit(1);
            }

            goto CYCLE;

            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;

        }

    }
}

