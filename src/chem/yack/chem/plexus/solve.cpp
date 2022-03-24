

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
                lib(std::cerr << vpfx << "Cini    = ",C0,vpfx);
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

            double alpha = 0;
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
                    alpha = max_of(alpha,sorted::sum(xs,sorted::by_value));
                }
                else
                {
                    Gscal[i] = 1.0;
                }
            }

            YACK_CHEM_PRINTLN("Gscal = " << Gscal);
            YACK_CHEM_PRINTLN("Omega = " << Omega0);
            YACK_CHEM_PRINTLN("rhs   = " << xi);
            YACK_CHEM_PRINTLN("inv(Omega)*rhs");
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega0/>");

            if(alpha>1)
            {
                return alpha-1;
            }
            else
            {
                return 0;
            }
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
            if(verbose) lib(std::cerr << "//   Cini=",C0,"//   ");
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

            vector<size_t> blk;
            



            //------------------------------------------------------------------
            //
            // regularize
            //
            //------------------------------------------------------------------
            regularize(Corg);


            vector<double>       score;
            vector<equilibrium*> owner;

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                if(blocked[ei]) continue;;

                const double       Ki  = K[ei];
                Xi[ei] = eq.solve1D(Ki,Corg,Ceq[ei]);
                score << fabs(Xi[ei]);
                owner << (equilibrium *)&eq;
            }

            hsort(score,owner,comparison::increasing<double>);
            for(size_t i=1;i<=score.size();++i)
            {
                std::cerr << owner[i]->name << " @" << score[i] << std::endl;
            }

            transfer(Corg,Ceq[**owner[1]]);


            computeState(Corg);
            for(size_t i=N;i>0;--i)
            {
                const readable<double> &psi = Psi[i];
                writable<double>       &Omi = Omega0[i];
                Omi.ld(0);
                if(blocked[i])
                {
                    Gamma[i] = 0;
                    xi[i]    = 0;
                    Omi[i]   = 1;
                    Gscal[i] = 1;
                }
                else
                {
                    xi[i] = -Gamma[i];
                    for(size_t k=N;k>0;--k) Omi[k] = xdot(psi,Nu[k],Ctmp);
                    Gscal[i] = fabs(Omi[i]);
                }
            }

            eqs(std::cerr << vpfx << "Gamma=",Gamma,vpfx);
            eqs(std::cerr << vpfx << "Omega=",Omega0,vpfx);

            if(!LU.build(Omega0))
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [singular]");
                return false;
            }

            LU.solve(Omega0,xi);
            eqs(std::cerr << vpfx << "xi=",xi,vpfx);


            // compute dC




            dC.ld(0);
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
                if(d<0)
                {
                    const double c = Corg[j]; assert(c>=0);
                    if(d<-c)
                    {
                        rstack << c/(-d);
                        ustack << j;
                    }
                }
            }

            lib(std::cerr << vpfx << "dC=",dC,vpfx);

            
            hsort(rstack,ustack,comparison::increasing<double>);
            std::cerr << "rstack=" << rstack << std::endl;
            std::cerr << "ustack=" << ustack << std::endl;



            exit(1);

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
            if(verbose) lib(std::cerr << "//   Cini=",C0,"//   ");
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
            // compute scaling
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(vpfx << "<plexus.Xi>");
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];

                xi[ei] = Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                blocked[ei] = (tao::v1::mod2<double>::of(psi) <= 0);
            }
            if(verbose)
            {
                eqs(std::cerr << vpfx << "Xi  = ",Xi, vpfx);
                eqs(std::cerr << vpfx << "Psi = ",Psi,vpfx);

            }
            YACK_CHEM_PRINTLN(vpfx << "<plexus.Xi/>");

            ios::ocstream::echo("rms.dat", "%g %.15g\n", double(cycle), sqrt(xdot(Xi,Xi,xs)/N) );


            //------------------------------------------------------------------
            //
            // compute Omega and regularizing factor
            //
            //------------------------------------------------------------------
            double       alpha = makeOmega0();
            const double rms0  = rmsGamma(Corg);
            std::cerr << "rms0=" << rms0 << std::endl;


        EVAL_XI:
            YACK_CHEM_PRINTLN(vpfx << "alpha=" << alpha);
            iOmega.assign(Omega0);

            for(size_t i=N;i>0;--i)
            {
                iOmega[i][i] += alpha;
            }

            std::cerr << "iOmega=" << iOmega << std::endl;

            if( !LU.build(iOmega) )
            {
                YACK_CHEM_PRINTLN("// <plexus.solve/> [failure]");
                return false;
            }

            LU.solve(iOmega,xi);
            if(verbose)
            {
                eqs(std::cerr << vpfx << "xi  = ",xi, vpfx);
            }

            // build dC
            rstack.free();
            ustack.free();
            dC.ld(0);
            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j] = xdot(xi,NuT[j],xs);
                if(d<0)
                {
                    const double c = Corg[j]; assert(c>=0);
                    if(d<-c)
                    {
                        rstack << c/(-d);
                        ustack << j;
                    }
                }
            }



            if(rstack.size())
            {
                hsort(rstack,ustack,comparison::increasing<double>);
                std::cerr << "rstack=" << rstack << std::endl;
                std::cerr << "ustack=" << ustack << std::endl;
                for(size_t i=N;i>0;--i)
                {
                    xi[i] = Xi[i];
                }
                alpha = max_of(alpha,1.0) * 10;
                goto EVAL_XI;
            }

            if(verbose)
            {
                lib(std::cerr << vpfx << "dC  = ",dC, vpfx);
            }

            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                Ctry[j] = max_of( Corg[j] + dC[j], 0.0);
            }

            const double rms1 = rmsGamma(Ctry);
            std::cerr << "rms1=" << rms1 << "/" << rms0 << " @cycle#" << cycle << std::endl;
            if(rms1>rms0)
            {
                for(size_t i=N;i>0;--i)
                {
                    xi[i] = Xi[i];
                }
                alpha = max_of(alpha,1.0) * 10;
                goto EVAL_XI;
            }

            transfer(Corg,Ctry);
            if(cycle>=100)
            {
                exit(1);
            }

            goto CYCLE;


            YACK_CHEM_PRINTLN("// <plexus.solve/> [success]");
            return true;
        }
#endif

    }

}
