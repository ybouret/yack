#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor.solve] ";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            if(verbose) corelib(*xml << "Cini=","", C0);


            //------------------------------------------------------------------
            //
            //
            // initialize depending on #equilibria = N
            //
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0: YACK_XMLOUT(xml, "success_empty");
                    return true;


                case 1: YACK_XMLOUT(xml, "success_single");
                {
                    const equilibrium &eq = ***singles.head();       // standalone
                    outcome::study(eq, K[1], C0, Corg, xmul, xadd);  // find 1D eq
                    return returnSolved(C0);                         // done
                } break;


                default:
                    //----------------------------------------------------------
                    // initialize consistent state
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                        dC[i]   = 0;
                    }
            }

            YACK_XMLSUB(xml,"<solve>");

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml,"---------------- #cycle= " << std::setw(3) << cycle << " ----------------");
            //------------------------------------------------------------------
            //
            //
            // check all singles and compute scaling to evaluate Hamiltonian
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOUT(xml,"singles topology@level-1");
            size_t              nrun = 0;
            outcome             ppty;
            const  equilibrium *emax = getTopology(nrun,ppty);

            if(!emax)
            {
                YACK_XMLOUT(xml,"success_fully_solved");
                return returnSolved(C0);
            }

            //------------------------------------------------------------------
            //
            //
            // act according to running dimensions
            //
            //
            //------------------------------------------------------------------
            assert(emax);
            //YACK_CHEM_PRINTLN(fn << "  found most displaced @" << emax->name);

            switch(nrun)
            {
                case 0:
                    //YACK_CHEM_PRINTLN(fn << "  <success> [all-blocked @level-1]");
                    return returnSolved(C0);

                case 1:
                    //YACK_CHEM_PRINTLN(fn << "  is the only one running @level-1");
                    working.transfer(Corg,Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // Compute Hamiltonian @Corg
            //
            //
            //------------------------------------------------------------------
            assert(nrun>1);
            double H0 = Hamiltonian(Corg);
            //YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (initial)");


            //------------------------------------------------------------------
            //
            //
            // look within pre-computed singles
            //
            //
            //------------------------------------------------------------------

            //YACK_CHEM_PRINTLN(fn << "  <looking for a dominant minimum>");
            double                    Hmin = H0;
            const equilibrium * const emin = getDominant(Hmin);
            if(!emin)
            {
                //--------------------------------------------------------------
                //
                // stay @Corg, doesn't change H0
                //
                //------------------------------------------------------------------
                //YACK_CHEM_PRINTLN(fn << "  <no global dominant>");
                assert( fabs(H0-Hamiltonian(Corg))<=0 );
            }
            else
            {
                //--------------------------------------------------------------
                //
                // found a global decrease
                //
                //---------------------------------------------------------------
                const equilibrium &eq = *emin;
                if(verbose)
                {
                    lattice.pad(std::cerr << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
                }

                //--------------------------------------------------------------
                //
                // look for a better combination including the best decrease
                //
                //--------------------------------------------------------------
                //YACK_CHEM_PRINTLN( fn << "    <looking for a better combination with @" << eq.name << ">");

                //--------------------------------------------------------------
                // initialize to equilibrium's best
                //--------------------------------------------------------------
                iota::load(Cend,Ceq[*eq]);
                const group *g = solving.find_first(eq); assert(g);
                do {
                    assert(g->is_ortho());
                    if(1==g->size)
                    {
                        const equilibrium &member = **(g->head);
                        assert( &member == &eq);
                        continue;
                    }

                    //----------------------------------------------------------
                    // build a trial concentration
                    //----------------------------------------------------------
                    iota::load(Ctry,Corg);
                    for(const gnode *ep=g->head;ep;ep=ep->next)
                    {
                        const equilibrium &member = **ep;
                        member.transfer(Ctry,Ceq[*member]);
                    }

                    //----------------------------------------------------------
                    // compute trial value
                    //----------------------------------------------------------
                    const double Htry = Hamiltonian(Ctry);
                    const bool   ok   = (Htry<Hmin);
                    if(verbose)
                    {
                        std::cerr << "    Htry=" << std::setw(15) << Htry;
                        std::cerr << (ok? " [+]" : " [-]");
                        std::cerr << " @" << *g << std::endl;
                    }
                    if(ok)
                    {
                        Hmin = Htry;
                        working.transfer(Cend,Ctry);
                    }

                } while( NULL != ( g = solving.find_next(g,eq)) );

                //--------------------------------------------------------------
                // full update Corg <- Cend
                //--------------------------------------------------------------
                working.transfer(Corg,Cend);
                if(verbose) corelib(std::cerr << fn << "Cnew=","", Corg);

                //--------------------------------------------------------------
                // study moidified topology
                //--------------------------------------------------------------
                //YACK_CHEM_PRINTLN(fn << "   <singles topology @level-2>");
                emax = getTopology(nrun,ppty);
                switch(nrun)
                {
                    case 0:
                        //YACK_CHEM_PRINTLN(fn << "    <success> [all-blocked @level-2]");
                        return returnSolved(C0);

                    case 1:
                        //YACK_CHEM_PRINTLN(fn << "    @" << emax->name << " is the only one running @level-2");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;
                }

                H0 = Hamiltonian(Corg);
                //YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (updated)");
            }

            //------------------------------------------------------------------
            //
            //
            // at this point, we should try to solve the system
            //
            //
            //------------------------------------------------------------------
            const bool atGlobalMinimum = (NULL==emin);
            bool       usingMaximumDOF = true;

            //------------------------------------------------------------------
            //
            // initialize Omega and Gamma
            //
            //------------------------------------------------------------------
            createOmega();

            if(verbose)
            {
                std::cerr << "Psi="   << Psi   << std::endl;
                std::cerr << "NuA="   << NuA   << std::endl;
                std::cerr << "Omega=" << Omega << std::endl;
                std::cerr << "Gamma=" << Gamma << std::endl;
            }

            //------------------------------------------------------------------
            //
            // try to inverse Omega
            //
            //------------------------------------------------------------------
            if( !solv.build(Omega) )
            {
                //YACK_CHEM_PRINTLN(fn << "   <singular system>");
                if(atGlobalMinimum)
                {
                    //YACK_CHEM_PRINTLN(fn << "   <at global minimum>");
                    return false;
                }
                else
                {
                    //YACK_CHEM_PRINTLN(fn << "   <try again>");
                    goto CYCLE;
                }
            }

            //------------------------------------------------------------------
            //
            // compute extent
            //
            //------------------------------------------------------------------
            iota::neg(xi,Gamma);
            solv.solve(Omega,xi);
            singles(std::cerr << "xi=","",xi);

            //------------------------------------------------------------------
            //
            // study PRIMARY extents
            //
            //------------------------------------------------------------------
            bool               recomputeStep = false;
            const equilibrium *lastAccepted  = NULL;
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium      &eq  = ***node;
                const size_t            ei  = *eq;    if(blocked[ei]) continue;
                const double            xx  = xi[ei];
                const xlimits          &lm  = eq.primary_limits(Corg,corelib.maxlen);
                const bool              ok  = lm.acceptable(xx);
                if(verbose)
                {
                    if(ok)
                    {
                        std::cerr << " (+) accepted";
                    }
                    else
                    {
                        std::cerr << " (-) rejected";
                    }
                    singles.pad(std::cerr << ' ' << eq.name,eq) << " @" << std::setw(15) << xx <<": ";
                    std::cerr << lm << std::endl;
                }

                if(!ok)
                {
                    // discarding
                    recomputeStep   = true;
                    --nrun;
                    deactivated(ei);
                }
                else
                {
                    // register as good
                    lastAccepted = &eq;
                }
            }

            //------------------------------------------------------------------
            //
            // discarded too big extents
            //
            //------------------------------------------------------------------
            if(recomputeStep)
            {
                //YACK_CHEM_PRINTLN(fn << "  <discarding extents>");
                switch(nrun)
                {
                    case 0:
                        assert(NULL==lastAccepted);
                        //YACK_CHEM_PRINTLN(fn << "  <success> [all-blocked @primary extents]");
                        return returnSolved(C0);
                        goto CYCLE;

                    case 1:
                        assert(NULL!=lastAccepted);
                        //YACK_CHEM_PRINTLN(fn << "  <accepting only @" << lastAccepted->name << ">");
                        working.transfer(Corg, Ceq[ **lastAccepted ]);
                        goto CYCLE;

                    default:
                        break;
                }

                usingMaximumDOF = false; // Corg is not fully regular
                H0 = updateOmega();      // new Hamiltonian
                //YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (updated)");
            }



            exit(0);

            return false;
        }

        





#if 0
        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor.solve] ";
            YACK_CHEM_PRINTLN(fn);
            if(verbose)
            {
                corelib(std::cerr << "Cini=","", C0);
            }

            //------------------------------------------------------------------
            //
            // initialize depending on #equilibria
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [empty]");
                    return true;

                case 1: {
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [single]");
                    const equilibrium &eq = ***singles.head();
                    outcome::study(eq, K[1], C0, Corg, xmul, xadd);
                    return returnSolved(C0);
                } break;

                default:
                    // initialize consistent state
                    YACK_CHEM_PRINTLN(fn << "COMPUTE [#" << N << "]");
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = C0[i];
                        dC[i]   = 0;
                    }
            }


            unsigned cycle = 0;
        CYCLE:

            ++cycle;
            YACK_CHEM_PRINTLN(fn << "---------------- #cycle= " << std::setw(3) << cycle << " ----------------");

            //------------------------------------------------------------------
            //
            //
            // study singles, initialize phase space
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << "[singles topology @level-1]");
            size_t              nrun = 0;
            outcome             ppty;
            const  equilibrium *emax = setTopology(nrun,ppty);

            if(!emax)
            {
                YACK_CHEM_PRINTLN(fn << "SUCCESS [fully solved]");
                return returnSolved(C0);
            }

            //------------------------------------------------------------------
            //
            //
            // check running dimensions
            //
            //
            //------------------------------------------------------------------
            assert(emax);
            YACK_CHEM_PRINTLN(fn << "found most displaced @" << emax->name);

            switch(nrun)
            {
                case 0:
                    YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked @level-1]");
                    return returnSolved(C0);

                case 1:
                    YACK_CHEM_PRINTLN(fn << "is the only one running @level-1");
                    working.transfer( Corg, Ceq[**emax] );
                    goto CYCLE;

                default:
                    break;
            }

            //------------------------------------------------------------------
            //
            //
            // Compute Hamiltonian @Corg
            //
            //
            //------------------------------------------------------------------
            double H0 = Hamiltonian(Corg);
            YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (initial)");
            

            //------------------------------------------------------------------
            //
            //
            // look within pre-computed singles
            //
            //
            //------------------------------------------------------------------

            YACK_CHEM_PRINTLN(fn << "[looking for a dominant minimum]");
            double             Hmin = H0;
            const equilibrium *emin = getDominant(Hmin);

            if(emin)
            {
                //--------------------------------------------------------------
                //
                // found a global decrease
                //
                //---------------------------------------------------------------
                const equilibrium &eq = *emin;
                if(verbose)
                {
                    lattice.pad(std::cerr << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
                }

                //--------------------------------------------------------------
                //
                // initialize search for optimized combination
                //
                //--------------------------------------------------------------
                YACK_CHEM_PRINTLN( fn << " [looking for a better combination]");
                const group *g = solving.find_first(eq); assert(g);
                iota::load(Cend,Ceq[*eq]);
                do {
                    //----------------------------------------------------------
                    // skip group with a blocked member
                    //----------------------------------------------------------
                    assert(g->is_ortho());
                    if(isTurnedOff(g))
                    {
                        YACK_CHEM_PRINTLN("    Htry: skipping " << *g);
                        continue;
                    }

                    //----------------------------------------------------------
                    // build a trial concentration
                    //----------------------------------------------------------
                    iota::load(Ctry,Corg);
                    for(const gnode *ep=g->head;ep;ep=ep->next)
                    {
                        const equilibrium &member = **ep;
                        member.transfer(Ctry,Ceq[*member]);
                    }

                    //----------------------------------------------------------
                    // compute trial value
                    //----------------------------------------------------------
                    const double Htry = Hamiltonian(Ctry);
                    const bool   ok   = (Htry<Hmin);
                    if(verbose)
                    {
                        std::cerr << "    Htry=" << std::setw(15) << Htry;
                        std::cerr << (ok? " [+]" : " [-]");
                        std::cerr << " @" << *g << std::endl;
                    }
                    if(ok)
                    {
                        Hmin = Htry;
                        working.transfer(Cend,Ctry);
                    }

                } while( NULL != ( g=solving.find_next(g,eq)  ) );

                //----------------------------------------------------------
                // full update @Corg
                //----------------------------------------------------------
                YACK_CHEM_PRINTLN( fn << " [moving at optimized combination]");
                working.transfer(Corg,Cend);
                if(verbose)
                {
                    corelib(std::cerr << "Cnew=","", Corg);
                }
                YACK_CHEM_PRINTLN(fn << "[singles topology @level-2]");
                emax = setTopology(nrun,ppty);
                switch(nrun)
                {
                    case 0:
                        YACK_CHEM_PRINTLN(fn << "SUCCESS [all-blocked @level-2]");
                        return returnSolved(C0);

                    case 1:
                        YACK_CHEM_PRINTLN(fn << "is the only one running @level-2");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;
                }

                H0 = Hamiltonian(Corg);
                YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (updated)");
            }
            else
            {
                //--------------------------------------------------------------
                //
                // stay @Corg, doesn't change H0
                //
                //------------------------------------------------------------------
                YACK_CHEM_PRINTLN(fn << "[no global dominant]");
                assert( fabs(H0-Hamiltonian(Corg))<=0 );
            }

            //------------------------------------------------------------------
            //
            //
            // computing Omega
            //
            //
            //------------------------------------------------------------------
            YACK_CHEM_PRINTLN(fn << "[create Omega@nrun=" << nrun << "]");
            bool maxDOF = true;
            createOmega();

            //------------------------------------------------------------------
            //
            //
            // computing xi
            //
            //
            //------------------------------------------------------------------
        COMPUTE_STEP:
            //std::cerr << "Psi=" << Psi << std::endl;
            //std::cerr << "NuA=" << NuA << std::endl;
            //std::cerr << "Omega=" << Omega << std::endl;
            //std::cerr << "Gamma=" << Gamma << std::endl;

            iOmega.assign(Omega);
            if( !solv.build(iOmega,xadd) )
            {
                YACK_CHEM_PRINTLN(fn << "[singular Omega]");
                exit(1);
            }

            iota::neg(xi,Gamma);
            solv.solve(iOmega,xi);



            //------------------------------------------------------------------
            //
            //
            // checking PRIMARY extents
            //
            //
            //------------------------------------------------------------------
            bool recomputeStep = false;
            for(const enode *node = singles.head(); node; node=node->next)
            {
                const equilibrium      &eq  = ***node;
                const size_t            ei  = *eq; if(blocked[ei]) continue;
                const double            xx  = xi[ei];
                const xlimits          &lm  = eq.primary_limits(Corg,corelib.maxlen);
                const bool              ok  = lm.acceptable(xx);
                if(verbose)
                {
                    if(ok)
                    {
                        std::cerr << " (+) accepted";

                    }
                    else
                    {
                        std::cerr << " (-) rejected";

                    }
                    singles.pad(std::cerr << ' ' << eq.name,eq) << " @" << std::setw(15) << xx <<": ";
                    std::cerr << lm << std::endl;
                }

                if(!ok)
                {
                    recomputeStep = true;
                    maxDOF        = false;
                    --nrun;
                    deactivated(ei);
                }
            }

            //------------------------------------------------------------------
            //
            //
            // take action upon PRIMARY extents
            //
            //
            //------------------------------------------------------------------
            if(recomputeStep)
            {
                H0 = updateOmega();
                YACK_CHEM_PRINTLN(fn << "[update Omega@nrun=" << nrun << "]");
                YACK_CHEM_PRINTLN( fn << "    H0 = " << H0 << " M (updated)");

                switch(nrun)
                {
                    case 0:
                        YACK_CHEM_PRINTLN(fn << "[full overshoot!!]");
                        goto CYCLE;

                    case 1: {
                        YACK_CHEM_PRINTLN(fn << "[only one left to move!!]");
                        for(const enode *ep=singles.head();ep;ep=ep->next)
                        {
                            const equilibrium &eq = ***ep;
                            const size_t       ei = *eq;
                            if(blocked[ei]) continue;

                            working.transfer(Corg,Ceq[ei]);
                            YACK_CHEM_PRINTLN(fn << "[ @<" << eq.name << "> : H=" << Hamiltonian(Corg) << "]");
                            goto CYCLE;
                        }
                        throw imported::exception(fn,"couldn't find single equilibrium!!");
                    }

                    default:
                        YACK_CHEM_PRINTLN(fn << "[recomputing step]");
                        break;
                }
                goto COMPUTE_STEP;
            }



            //------------------------------------------------------------------
            //
            //
            // compute delta C and checking when dC<0
            //
            //
            //------------------------------------------------------------------
            ratio.free();
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                xadd.ldz();
                for(size_t i=N;i>0;--i)
                {
                    xadd.ld( NuA[i][j] * xi[i] );
                }
                const double d = (dC[j] = xadd.get());
                const double c = Corg[j];
                if(d<0 && (-d)>c)
                {
                    ratio << c/(-d);
                }
            }

            corelib(std::cerr << "dC=", "", dC);
            bool   maxLen = true;
            double umax   = 1;
            if(ratio.size())
            {
                hsort(ratio,comparison::increasing<double>);
                std::cerr << "ratio  = " << ratio << std::endl;
                const double rmax = ratio.front();
                if(rmax<=1)
                {
                    umax   = 0.5 * rmax;
                    maxLen = false;
                }
                exit(1);
                //umax = ratio.front();
            }
            std::cerr << "maxLen = " << maxLen << std::endl;

            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                Cend[j] = max_of<double>(Corg[j]+umax*dC[j],0);
            }
            
            {
                ios::ocstream fp("ham.dat");
                const size_t np=1000;
                for(size_t i=0;i<=np;++i)
                {
                    const double u = (umax*i)/double(np);
                    fp("%.15g %.15g\n",u,(*this)(u));
                }
            }

            const double H1 = Hamiltonian(Cend);
            std::cerr << "H=" << H0 << " -> " << H1 << std::endl;
            {
                triplet<double> u = { 0, -1, umax };
                triplet<double> f = { H0, -1, H1  };
                optimize::run_for(*this,u,f,optimize::inside);
                std::cerr << "H=" << f.b << "@" << u.b << std::endl;
            }


            if(!maxDOF)
            {
                YACK_CHEM_PRINTLN(fn << "[overshooting extents]");
                working.transfer(Corg,Ctry);
                goto CYCLE;
            }

            if(!maxLen)
            {
                YACK_CHEM_PRINTLN(fn << "[overshooting increments]");
                working.transfer(Corg,Ctry);
                goto CYCLE;
            }


            YACK_CHEM_PRINTLN(fn << "[May Test Convergence!!]");

            exit(0);

            return false;
            

        }

#endif

    }
}
