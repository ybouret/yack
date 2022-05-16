

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


            // compute 1D solutions
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
                Xi[ei]  = eq.solve1D(Ki,Corg,Ci);
                eq.drvs_action(psi,Ki,Ci,Ctmp);
                const double den = sorted::dot(psi,Nu[ei],Ctmp);
                if( ! (blk = (den>=0) ) )
                {
                    eq.drvs_action(psi0, Ki, Corg, Ctmp);
                    std::cerr << "psi0=" << psi0 << " / " << psi << std::endl;
                    for(size_t k=1;k<ei;++k)
                    {
                        Omi[k] = sorted::dot(psi0,Nu[k],Ctmp)/den;
                    }
                    for(size_t k=ei+1;k<N;++k)
                    {
                        Omi[k] = sorted::dot(psi0,Nu[k],Ctmp)/den;
                    }
                }
            }

            if(verbose)
            {
                eqs(std::cerr << vpfx << "Xi     =",Xi,vpfx);
                eqs(std::cerr << vpfx << "Psi    =",Psi,vpfx);
                eqs(std::cerr << vpfx << "blocked=",blocked,vpfx);
                eqs(std::cerr << vpfx << "Omega  =",Omega0,vpfx);
            }

            iOmega.assign(Omega0,transposed);
            std::cerr << "Omega=" << Omega0 << std::endl;
            std::cerr << "Xi   =" << Xi     << std::endl;




            // compute omega0


            exit(1);


            YACK_CHEM_PRINTLN("// <plexus.solve> [success]");
            return true;

        }



    }
}

