

#include "yack/chem/plexus.hpp"
#include "yack/apex.hpp"
#include "yack/math/tao/v1.hpp"
#include "yack/sort/sum.hpp"
#include "yack/exception.hpp"
#include "yack/math/timings.hpp"
#include "yack/sequence/stats.hpp"
#include "yack/math/opt/minimize.hpp"
#include <cmath>

namespace yack
{
    using  namespace math;

    namespace chemical
    {


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
                double            &gam = Gamma[ei];

                //--------------------------------------------------------------
                // first try
                //--------------------------------------------------------------
                gam  = eq.grad_action(psi,Ki,C0,Ctmp);
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
                    gam     = eq.grad_action(psi,Ki,C0,Ctmp);
                    if( tao::v1::mod2<double>::of(psi) <= 0 )
                    {
                        blocked[ei] = true;
                        gam         = 0;
                    }
                }
            }

            if(verbose)
            {
                lib(std::cerr << "//     Cini    = ",C0,"//     ");
                std::cerr <<     "//     blocked = " << blocked << std::endl;
            }

            //------------------------------------------------------------------
            //
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN("//   <plexus.regularize/>");
        }

        void plexus:: makeOmega0() throw()
        {
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega0>");

#if 0
            // first pass, compute Psi'
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];

                if(blocked[ei])
                {
                    psi.ld(0);
                    xi[ei] = Xi[ei] = 0;
                }
                else
                {
                    xi[ei] = Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                    eq.drvs_action(psi,Ki,Ci,Ctmp);
                }
            }

            eqs(std::cerr << "//   Xi  = ",Xi, "//   ");
            eqs(std::cerr << "//   Psi = ",Psi,"//   ");
            eqs(std::cerr << "//   Nu  = ",Nu, "//   ");
#endif

            for(size_t i=N;i>0;--i)
            {
                writable<double>       &Omi = Omega0[i];
                const readable<double> &psi = Psi[i];
                Omi.ld(0);
                Omi[i] = 1.0;
                xm[i]  = 0;
                if(!blocked[i])
                {
                    {
                        const double diag = xdot(psi,Nu[i],Ctmp); assert(diag<0);
                        for(size_t k=N;  k>i;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/diag);
                        /*                     */ xs[i] = 0;
                        for(size_t k=i-1;k>0;--k) xs[k] = fabs(Omi[k] = xdot(psi,Nu[k],Ctmp)/diag);
                    }
                    const double xtra = sorted::sum(xs,sorted::by_value);
                    xm[i] = xtra;
                    //Omi[i] += xtra;
                }
            }

            YACK_CHEM_PRINTLN("Extra = " << xm);
            YACK_CHEM_PRINTLN("Omega = " << Omega0);
            YACK_CHEM_PRINTLN("rhs   = " << xi);
            YACK_CHEM_PRINTLN("inv(Omega)*rhs");
            YACK_CHEM_PRINTLN("//   <plexus.makeOmega0/>");

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

            //------------------------------------------------------------------
            //
            // compute scaling
            //
            //------------------------------------------------------------------

            vector<double>       score;
            vector<equilibrium*> owner;

            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];

                Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                const bool bad = blocked[ei] = (tao::v1::mod2<double>::of(psi) <= 0);
                if(!bad)
                {
                    score << fabs(Xi[ei]);
                    owner << (equilibrium *) &eq;
                }
            }

            hsort(score,owner,comparison::increasing<double>);
            for(size_t i=1;i<=score.size();++i)
            {
                std::cerr << "Xi_" << owner[i]->name << " = " << score[i] << std::endl;
            }

            size_t ii = **owner[1];
            transfer(Corg,Ceq[ii]);

            lib(std::cerr << "Cint=",Corg);
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                const double       Ki  = K[ei];
                //writable<double>  &psi = Psi[ei];
                writable<double>  &Ci  = Ceq[ei];
                Xi[ei] = eq.solve1D(Ki,Corg,Ci);
            }
            eqs(std::cerr << "Xi=",Xi);








            exit(1);



            return false;

        }


    }

}
