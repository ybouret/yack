

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
                Ctry[j] = max_of(Corg[j] + u * dC[j],0.0);
                //Ctry[j] = max_of( (1.0-u) * Corg[j] + u * Cend[j], 0.0 );
            }
            return rmsGamma(Ctry);
        }


        size_t plexus:: computeOmega0() throw()
        {
            size_t num_blocked = 0;
            for(const enode *node=eqs.head();node;node=node->next)
            {
                const equilibrium &eq   = ***node;
                const size_t       ei   = *eq;
                const double       Ki   = K[ei];
                writable<double>  &Ci   = Ceq[ei];
                writable<double>  &psi  = Psi[ei];
                bool              &blk  = blocked[ei];
                writable<double>  &Omi  = Omega0[ei];
                writable<double>  &psi0 = dC;

                Omi.ld(0);
                Omi[ei] = 1;
                Xi[ei] = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);      //! psi at Ceq
                const double den =   sorted::dot(psi,Nu[ei],Ctmp);
                if( ! (blk = (den>=0) ) )
                {
                    Gs[ei] = 1.0/(-den);                    //! scaling
                    eq.drvs_action(psi0, Ki, Corg, Ctmp);   //! psi at Corg
                    double xtra = 0;
                    for(size_t k=1;k<ei;++k)
                    {
                        xtra += fabs( Omi[k] = sorted::dot(psi0,Nu[k],Ctmp)/den );
                    }
                    for(size_t k=ei+1;k<N;++k)
                    {
                        xtra += fabs( Omi[k] = sorted::dot(psi0,Nu[k],Ctmp)/den );
                    }
                    xd[ei] = xtra;
                }
                else
                {
                    Xi[ei]  = 0;
                    xd[ei]  = 0;
                    Gs[ei]  = 0;
                    ++num_blocked;
                }
            }
            return num_blocked;
        }


        bool is_ddom(const readable<double> &extra) throw()
        {
            for(size_t i=extra.size();i>0;--i)
            {
                if(extra[i]>=1.0) return false;
            }
            return true;
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

            size_t cycle=0;
        CYCLE:
            ++cycle;
            std::cerr << "@cycle=" << cycle << std::endl;
            //------------------------------------------------------------------
            //
            // compute 1D solutions and initialize
            //
            //------------------------------------------------------------------
            size_t num_blocked = computeOmega0();



            //------------------------------------------------------------------
            //
            // evaluate Xi
            //
            //------------------------------------------------------------------
        EVAL_XI:
            if(verbose)
            {
                eqs(std::cerr << vpfx << "Xi     =",Xi,vpfx);
                eqs(std::cerr << vpfx << "blocked=",blocked,vpfx);
                eqs(std::cerr << vpfx << "Omega  =",Omega0,vpfx);
                eqs(std::cerr << vpfx << "extra  =",xd,vpfx);
            }


            if( is_ddom(xd) )
            {
                YACK_CHEM_PRINTLN("// Diagonally Dominant Omega: use LU");
                iOmega.assign(Omega0);
                if(!LU.build(iOmega))
                {
                    YACK_CHEM_PRINTLN("// <plexus.solve> [SINGULAR]");
                    return false;
                }
                tao::v1::set(xi,Xi);
                LU.solve(iOmega,xi);
                eqs(std::cerr << vpfx << "xi  =",xi,vpfx);

                for(const enode *node=eqs.head();node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = *eq;
                    if(blocked[ei]) continue;
                    const limits      &lm = eq.primary_limits(Corg,lib.width);
                    double            &xx = xi[ei];
                    eqs.pad(std::cerr << eq.name,eq) << " : "  << lm << std::endl;
                    if( lm.should_reduce(xx) )
                    {
                        xx      = 0;
                        Xi[ei]  = 0;
                        xd[ei]  = 0;
                        Omega0[ei].ld(0);
                        Omega0[ei][ei] = 1.0;
                        ++num_blocked;
                        std::cerr << "<< blocking " << eq.name << ">>" << std::endl;
                        goto EVAL_XI;
                    }
                }

                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "Xi   =" << Xi    << std::endl;
                std::cerr << "xi   =" << xi    << std::endl;

            }
            else
            {
                YACK_CHEM_PRINTLN("// Diagonally Weak Omega: use Gradient");
                iOmega.assign(Omega0,transposed);
                for(size_t i=N;i>0;--i)
                {
                    xi[i] = blocked[i] ? 0 : sorted::dot(iOmega[i],Xi,xs);
                }
                eqs(std::cerr << vpfx << "beta  =",xi,vpfx);
                std::cerr << "beta=" << xi << std::endl;
                std::cerr << "Omega=" << Omega0 << std::endl;
                std::cerr << "Xi   =" << Xi << std::endl;
                exit(1);
            }


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
                    rstack << (Corg[j]/(-d));
                    ustack << j;
                }

            }
            std::cerr << "dC=" << dC << std::endl;


            const size_t nc     = rstack.size();
            double       expand = 10.0;
            if(nc)
            {
                hsort(rstack,ustack,comparison::increasing<double>);
                for(size_t i=1;i<=nc;++i)
                {
                    const species &s = lib[ ustack[i] ];
                    lib.pad(std::cerr << vpfx << s.name,s) << " => " << rstack[i] << std::endl;
                }
                expand = min_of(expand,rstack[1]);
            }

            std::cerr << "expand=" << expand << std::endl;
            {
                ios::ocstream fp("gam.dat");
                size_t NP = 100;
                for(size_t i=0;i<=NP;++i)
                {
                    const double u = (i*expand)/NP;
                    fp("%g %g\n",u, sqrt((*this)(u)));
                }
            }

            const double    g0 = rmsGamma(Corg);
            triplet<double> u = { 0, -1, expand };
            triplet<double> g = { g0, -1, (*this)(u.c) };

            (void)minimize::find<double>::run_for(*this,u,g,minimize::inside);
            const double g1 = g.b;
            if(g1<g0)
            {
                std::cerr << "should accept step" << std::endl;
            }
            else
            {
                std::cerr << "bad step!!" << std::endl;
            }


            exit(1);



            for(const anode *node=active.head;node;node=node->next)
            {
                const species &s = **node;
                const size_t   j = *s;
                const double   d = dC[j];
                Ctry[j] = max_of<double>(0.0,Corg[j]+d);
            }

            transfer(Corg,Ctry);

            if(cycle>=1)
                exit(1);

            goto CYCLE;




            YACK_CHEM_PRINTLN("// <plexus.solve> [success]");
            return true;

        }



    }
}

