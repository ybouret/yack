#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/boolean.h"

#include <iomanip>

namespace yack
{
    using namespace math;

    namespace chemical
    {

        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"solving");
            if(verbose) corelib(*xml << "-- Cini=","", C0);


            //------------------------------------------------------------------
            //
            //
            // initialize depending on #equilibria = N
            //
            //
            //------------------------------------------------------------------
            switch(N)
            {
                case 0: YACK_XMLOG(xml, "-- success [empty]");
                    return true;


                case 1: YACK_XMLOG(xml, "-- success [single]");
                {
                    const equilibrium &eq = ***singles.head();       // standalone
                    outcome::study(eq, K[1], C0, Corg, xmul, xadd);  // find 1D eq
                    return returnSolved(C0,xml);                     // done
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


            unsigned cycle=0;

        CYCLE:
            //------------------------------------------------------------------
            // display where we are
            //------------------------------------------------------------------
            ++cycle;
            YACK_XMLOG(xml,"---------------- #cycle= " << std::setw(3) << cycle << " ----------------");
            if(verbose) corelib(*xml << "-- Corg=","", Corg);

            //------------------------------------------------------------------
            // singles status
            //------------------------------------------------------------------
            size_t             nrun = 0;
            const equilibrium *emax = NULL;

            //------------------------------------------------------------------
            // check all singles and compute scaling to evaluate Hamiltonian
            //------------------------------------------------------------------
            {
                YACK_XMLSUB(xml,"compute topology");
                emax = getTopology(nrun);
                if(!emax)
                {
                    YACK_XMLOG(xml,"-- success");
                    return returnSolved(C0,xml);
                }

                switch(nrun)
                {
                    case 0:
                        YACK_XMLOG(xml,"-- success: all blocked (shouldn't happen)");
                        return returnSolved(C0,xml);

                    case 1:
                        YACK_XMLOG(xml,"-- only {" << emax->name << "} is running");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;

                }
            }

            assert(emax!=NULL);
            assert(nrun>1);


            //------------------------------------------------------------------
            // system is scaled and singles are initialized: compute H0
            //------------------------------------------------------------------
            double H0 = Hamiltonian(Corg);
            YACK_XMLOG(xml,"-- H0 = " << H0 << " M (initial)");
            if(H0<=0)
            {
                YACK_XMLOG(xml,"-- success: H0<=0");
                return returnSolved(C0,xml);
            }

            //------------------------------------------------------------------
            // find out the dominant reaction
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- looking for a dominant minimum");
            bool atGlobalMinimum = false;
            {
                YACK_XMLSUB(xml,"find dominant");
                double                    Hmin = H0;
                const equilibrium * const emin = getDominant(Hmin);
                if(!emin)
                {
                    //----------------------------------------------------------
                    // didn't change Corg
                    //----------------------------------------------------------
                    YACK_XMLOG(xml,"-- at global minimum");
                    atGlobalMinimum = true;
                    assert( fabs(H0-Hamiltonian(Corg))<=0 );
                }
                else
                {
                    //----------------------------------------------------------
                    // found a  decrease
                    //----------------------------------------------------------
                    const equilibrium &eq = *emin;
                    if(verbose)
                    {
                        lattice.pad(std::cerr << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
                    }

                    //----------------------------------------------------------
                    // look for a better combination including the best decrease
                    //----------------------------------------------------------
                    {
                        YACK_XMLSUB(xml,"optimizing");
                        YACK_XMLOG( xml,"-- with @" << eq.name);

                        //------------------------------------------------------
                        // initialize to equilibrium's best
                        //------------------------------------------------------
                        iota::load(Cend,Ceq[*eq]);
                        const group *g    = solving.find_first(eq); assert(g);
                        const group *gmin = g;
                        do {
                            assert(g->is_ortho());
                            if(1==g->size)
                            {
                                assert( & **(g->head) == &eq);
                                continue;
                            }

                            //--------------------------------------------------
                            // build a trial concentration
                            //--------------------------------------------------
                            iota::load(Ctry,Corg);
                            for(const gnode *ep=g->head;ep;ep=ep->next)
                            {
                                const equilibrium &member = **ep;
                                member.transfer(Ctry,Ceq[*member]);
                            }

                            //--------------------------------------------------
                            // compute trial value
                            //--------------------------------------------------
                            const double Htry = Hamiltonian(Ctry);
                            const bool   ok   = (Htry<Hmin);
                            if(verbose)
                            {
                                std::cerr << "\t->  " << std::setw(15) << Htry;
                                std::cerr << (ok? " [+]" : " [-]");
                                std::cerr << " @" << *g << std::endl;
                            }
                            if(ok)
                            {
                                Hmin = Htry;
                                gmin = g;
                                working.transfer(Cend,Ctry);
                            }

                        } while( NULL != ( g = solving.find_next(g,eq)) );

                        //--------------------------------------------------
                        // full update Corg <- Cend
                        //--------------------------------------------------
                        working.transfer(Corg,Cend);
                        if(verbose) {
                            if(gmin)
                            {
                                *xml << "-- found " << *gmin << std::endl;
                            }
                            else
                            {
                                *xml << "-- kept " << eq.name << std::endl;
                            }
                            corelib(*xml << "-- Cnew=","", Corg);
                        }

                    }


                    //----------------------------------------------------------
                    // update and check topology
                    //----------------------------------------------------------
                    {
                        YACK_XMLSUB(xml,"update topology");
                        emax = getTopology(nrun);
                        if(!emax)
                        {
                            YACK_XMLOG(xml,"-- success");
                            return returnSolved(C0,xml);
                        }

                        switch(nrun)
                        {
                            case 0:
                                YACK_XMLOG(xml,"-- success: all blocked (shouldn't happen)");
                                return returnSolved(C0,xml);

                            case 1:
                                YACK_XMLOG(xml,"-- only {" << emax->name << "} is running");
                                working.transfer(Corg,Ceq[**emax] );
                                goto CYCLE;

                            default:
                                break;
                        }

                    }

                    H0 = Hamiltonian(Corg);
                    YACK_XMLOG(xml,"-- H0 = " << H0 << " M (updated)");
                    if(H0<=0)
                    {
                        YACK_XMLOG(xml,"-- success: H0<=0");
                        return returnSolved(C0,xml);
                    }
                }
            }

            //------------------------------------------------------------------
            // try a local step
            //------------------------------------------------------------------
            bool       usingMaximumDOF = true;

            {
                //--------------------------------------------------------------
                //
                // initialize Omega and Gamma
                //
                //--------------------------------------------------------------
                YACK_XMLSUB(xml, "compute extent");
                createOmega();
                unsigned pass = 0;
            COMPUTE_EXTENT:
                ++pass;
                YACK_XMLOG(xml,"-- trial #" << pass << " --");
                //--------------------------------------------------------------
                //
                // try to inverse Omega
                //
                //--------------------------------------------------------------
                if( !solv.build(Omega) )
                {
                    YACK_XMLOG(xml,"-- singular system");
                    if(atGlobalMinimum)
                    {
                        YACK_XMLOG(xml,"-- at global minimum!!");
                        return false;
                    }
                    else
                    {
                        YACK_XMLOG(xml,"-- try again");
                        goto CYCLE;
                    }
                }

                //--------------------------------------------------------------
                //
                // compute extent
                //
                //--------------------------------------------------------------
                iota::neg(xi,Gamma);
                solv.solve(Omega,xi);

                //--------------------------------------------------------------
                //
                // study PRIMARY extents
                //
                //--------------------------------------------------------------
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
                            std::cerr << " (+)";
                        }
                        else
                        {
                            std::cerr << " (-)";
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

                //--------------------------------------------------------------
                //
                // discarded too big extents
                //
                //--------------------------------------------------------------
                if(recomputeStep)
                {
                    YACK_XMLOG(xml,"-- discarding extent(s)");
                    switch(nrun)
                    {
                        case 0:
                            assert(NULL==lastAccepted);
                            YACK_XMLOG(xml,"--  all primary extents are blocked");
                            if(atGlobalMinimum)
                                return returnSolved(C0,xml);
                            else
                                goto CYCLE;

                        case 1:
                            assert(NULL!=lastAccepted);
                            YACK_XMLOG(xml,"-- accepting only @" << lastAccepted->name);
                            working.transfer(Corg, Ceq[ **lastAccepted ]);
                            goto CYCLE;

                        default:
                            break;
                    }

                    usingMaximumDOF = false; // Corg is not fully regular
                    H0 = updateOmega();      // new Hamiltonian from a reduced set of equations
                    YACK_XMLOG(xml,"-- H0 = " << H0 << " M (updated)");
                    goto COMPUTE_EXTENT;
                }
            }


            {
                //--------------------------------------------------------------
                //
                YACK_XMLSUB(xml, "forwarding");
                //
                //--------------------------------------------------------------
                bool   usingFullLength = true;


                //--------------------------------------------------------------
                //
                //
                // compute delta C and checking when dC<0
                //
                //
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml, "deltaC");
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
                        if(verbose)
                        {
                            corelib.pad(*xml << '[' << sp.name <<']',sp) << " = " << std::setw(15) << c;
                            if(d>=0)
                            {
                                std::cerr << " +" << std::setw(15) << d;
                            }
                            else
                            {
                                std::cerr << " -" << std::setw(15) << fabs(d);
                            }
                        }

                        if(d<0 && (-d)>c)
                        {
                            ratio << c/(-d);
                        }

                        if(verbose) std::cerr << std::endl;
                    }

                    if(verbose) corelib(*xml << "dC=", "", dC);
                    exit(0);

                    double umax = 1;
                    if(ratio.size())
                    {
                        hsort(ratio,comparison::increasing<double>);
                        YACK_XMLOG(xml,"ratio  = " << ratio);
                        const double rmax = ratio.front();
                        if(rmax<=1)
                        {
                            umax            = 0.5 * rmax;
                            usingFullLength = false;
                        }
                    }

                    // compute Cend
                    for(const anode *node=working.head;node;node=node->next)
                    {
                        const species &sp = **node; assert(sp.rank>0);
                        const size_t   j  = *sp;
                        Cend[j] = max_of<double>(Corg[j]+umax*dC[j],0);
                    }
                }

                YACK_XMLOG(xml,"-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum));
                YACK_XMLOG(xml,"-- usingMaximumDOF = " << yack_boolean(usingMaximumDOF));
                YACK_XMLOG(xml,"-- usingFullLength = " << yack_boolean(usingFullLength));

                {
                    ios::ocstream fp("ham.dat");
                    const size_t np=1000;
                    for(size_t i=0;i<=np;++i)
                    {
                        const double u = (i)/double(np);
                        fp("%.15g %.15g\n",u,(*this)(u));
                    }
                }


                double H1 = Hamiltonian(Cend);
                YACK_XMLOG(xml, "-- H=" << H0 << " -> " << H1);
                {
                    triplet<double> u = { 0, -1,   1 };
                    triplet<double> f = { H0, -1, H1 };
                    optimize::run_for(*this,u,f,optimize::inside);
                    YACK_XMLOG(xml, "-- H=" << f.b << "@" << u.b);
                    H1 = f.b;
                }

                working.transfer(Corg,Ctry);
                if(H1<H0)
                {
                    goto CYCLE;
                }



            }

            exit(0);


            //exit(0);
            return false;


#if 0
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
            if(verbose) corelib(*xml << "-- Corg=","", Corg);
            YACK_XMLOG(xml,"-- Initializing Topology");
            size_t              nrun = 0;
            const  equilibrium *emax = getTopology(nrun);

            if(!emax)
            {
                YACK_XMLOG(xml,"-- Success[Full]");
                return returnSolved(C0,xml);
            }

            //------------------------------------------------------------------
            //
            //
            // act according to running dimensions
            //
            //
            //------------------------------------------------------------------
            assert(emax);
            YACK_XMLOG(xml,"-- Found most displaced @" << emax->name);


            switch(nrun)
            {
                case 0:
                    YACK_XMLOG(xml,"-- Success[AllBlocked@level-1]");
                    return returnSolved(C0,xml);

                case 1:
                    YACK_XMLOG(xml,"-- Only one running     @level-1");
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
            YACK_XMLOG(xml,"-- H0 = " << H0 << " M (initial)");
            if(H0<=0)
            {
                YACK_XMLOG(xml,"-- Success[H0<=0]");
                return returnSolved(C0,xml);
            }

            //------------------------------------------------------------------
            //
            //
            // look within pre-computed singles
            //
            //
            //------------------------------------------------------------------

            YACK_XMLOG(xml, "-- Looking for a dominant minimum");
            double                    Hmin = H0;
            const equilibrium * const emin = getDominant(Hmin);
            if(!emin)
            {
                //--------------------------------------------------------------
                //
                // stay @Corg, doesn't change H0
                //
                //------------------------------------------------------------------
                YACK_XMLOG(xml,"-- No global dominant => at global Minimum");
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
                {
                    YACK_XMLSUB(xml,"Optimizing");
                    YACK_XMLOG( xml,"-- Looking for a better combination with @" << eq.name);

                    //--------------------------------------------------------------
                    // initialize to equilibrium's best
                    //--------------------------------------------------------------
                    iota::load(Cend,Ceq[*eq]);
                    const group *g    = solving.find_first(eq); assert(g);
                    const group *gmin = g;
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
                            std::cerr << "\t--> Htry=" << std::setw(15) << Htry;
                            std::cerr << (ok? " [+]" : " [-]");
                            std::cerr << " @" << *g << std::endl;
                        }
                        if(ok)
                        {
                            Hmin = Htry;
                            gmin = g;
                            working.transfer(Cend,Ctry);
                        }

                    } while( NULL != ( g = solving.find_next(g,eq)) );

                    //--------------------------------------------------------------
                    // full update Corg <- Cend
                    //--------------------------------------------------------------
                    working.transfer(Corg,Cend);
                    if(verbose) {
                        *xml << "-- " << *gmin << std::endl;
                        corelib(*xml << "-- Cnew=","", Corg);
                    }

                }


                //--------------------------------------------------------------
                // study moidified topology
                //--------------------------------------------------------------
                YACK_XMLOG(xml,"-- Recomputing Topology");
                emax = getTopology(nrun);
                if(!emax)
                {
                    YACK_XMLOG(xml,"-- Success[Full]");
                    return returnSolved(C0,xml);
                }

                switch(nrun)
                {
                    case 0:
                        YACK_XMLOG(xml,"-- Success[AllBlocked@level-2]");
                        return returnSolved(C0,xml);

                    case 1:
                        YACK_XMLOG(xml,"-- @" << emax->name << " is the only one running @level-2");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        break;
                }

                H0 = Hamiltonian(Corg);
                YACK_XMLOG(xml,"-- H0 = " << H0 << " M (updated)");
                if(H0<=0)
                {
                    YACK_XMLOG(xml,"-- Success[H0<=0]");
                    return returnSolved(C0,xml);
                }
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

            {
                //------------------------------------------------------------------
                //
                // initialize Omega and Gamma
                //
                //------------------------------------------------------------------
                YACK_XMLSUB(xml, "ComputeExtent");
                createOmega();
                unsigned pass = 0;
            COMPUTE_EXTENT:
                ++pass;
                YACK_XMLOG(xml,"-- Trial #" << pass);
                //------------------------------------------------------------------
                //
                // try to inverse Omega
                //
                //------------------------------------------------------------------
                if( !solv.build(Omega) )
                {
                    YACK_XMLOG(xml,"-- singular system");
                    if(atGlobalMinimum)
                    {
                        YACK_XMLOG(xml,"-- at global minimum!!");
                        return false;
                    }
                    else
                    {
                        YACK_XMLOG(xml,"-- try again");
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
                    YACK_XMLOG(xml,"-- discarding extent(s)");
                    switch(nrun)
                    {
                        case 0:
                            assert(NULL==lastAccepted);
                            YACK_XMLOG(xml,"[Success[AllBlocked@PrimaryExtents]]");
                            return returnSolved(C0,xml);
                            goto CYCLE;

                        case 1:
                            assert(NULL!=lastAccepted);
                            YACK_XMLOG(xml,"-- accepting only @" << lastAccepted->name);
                            working.transfer(Corg, Ceq[ **lastAccepted ]);
                            goto CYCLE;

                        default:
                            break;
                    }

                    usingMaximumDOF = false; // Corg is not fully regular
                    H0 = updateOmega();      // new Hamiltonian
                    YACK_XMLOG(xml,"-- H0 = " << H0 << " M (updated)");
                    goto COMPUTE_EXTENT;
                }
            }

            {
                YACK_XMLSUB(xml, "Forwarding");
                bool   usingFullLength = true;


                //--------------------------------------------------------------
                //
                //
                // compute delta C and checking when dC<0
                //
                //
                //--------------------------------------------------------------
                {
                    YACK_XMLSUB(xml, "DeltaC");
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

                    if(verbose) corelib(*xml << "dC=", "", dC);
                    double umax = 1;
                    if(ratio.size())
                    {
                        hsort(ratio,comparison::increasing<double>);
                        YACK_XMLOG(xml,"ratio  = " << ratio);
                        const double rmax = ratio.front();
                        if(rmax<=1)
                        {
                            umax            = 0.5 * rmax;
                            usingFullLength = false;
                        }
                    }

                    for(const anode *node=working.head;node;node=node->next)
                    {
                        const species &sp = **node; assert(sp.rank>0);
                        const size_t   j  = *sp;
                        Cend[j] = max_of<double>(Corg[j]+umax*dC[j],0);
                    }
                }

                YACK_XMLOG(xml,"-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum));
                YACK_XMLOG(xml,"-- usingMaximumDOF = " << yack_boolean(usingMaximumDOF));
                YACK_XMLOG(xml,"-- usingFullLength = " << yack_boolean(usingFullLength));



                {
                    ios::ocstream fp("ham.dat");
                    const size_t np=1000;
                    for(size_t i=0;i<=np;++i)
                    {
                        const double u = (i)/double(np);
                        fp("%.15g %.15g\n",u,(*this)(u));
                    }
                }


                const double H1 = Hamiltonian(Cend);
                YACK_XMLOG(xml, "-- H=" << H0 << " -> " << H1);
                {
                    triplet<double> u = { 0, -1,   1 };
                    triplet<double> f = { H0, -1, H1 };
                    optimize::run_for(*this,u,f,optimize::inside);
                    YACK_XMLOG(xml, "-- H=" << f.b << "@" << u.b);
                }

                if(!usingMaximumDOF)
                {
                    YACK_XMLOG(xml, "-- overshooting/extent");
                    working.transfer(Corg,Ctry);
                    goto CYCLE;
                }

                if(!usingFullLength)
                {
                    YACK_XMLOG(xml, "-- overshooting/deltaC");
                    working.transfer(Corg,Ctry);
                    goto CYCLE;
                }


                working.transfer(Corg,Ctry);
                goto CYCLE;
            }



            exit(0);

            return false;
#endif

        }


    }
}
