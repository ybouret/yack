#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/type/boolean.h"
#include "yack/math/iota.hpp"
#include "yack/ios/xmlog.hpp"
#include "yack/type/boolean.h"
#include "yack/math/numeric.hpp"

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
                emax = getTopology(nrun,xml);
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
            //YACK_XMLOG(xml, "-- looking for a dominant minimum");
            bool atGlobalMinimum = false;
            {
                YACK_XMLSUB(xml,"find dominant");
                double                    Hmin = H0;
                const equilibrium * const emin = getDominant(Hmin,xml);
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
                        lattice.pad(*xml << "--> @" << eq.name,eq) << " -> " << std::setw(15) << Hmin << std::endl;
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
                                *xml << "--> " << std::setw(15) << Htry;
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
                        emax = getTopology(nrun,xml);
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
                            std::cerr << "| (+)";
                        }
                        else
                        {
                            std::cerr << "| (-)";
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
                const bool   usingFullLength =  evaluate_dC(xml);


                {
                    std::cerr << "\t\t[saving ham.dat]" << std::endl;
                    ios::ocstream fp("ham.dat");
                    const size_t np=1000;
                    for(size_t i=0;i<=np;++i)
                    {
                        const double u = (i)/double(np);
                        fp("%.15g %.15g\n",u,(*this)(u));
                    }
                }


                bool converged = true;
                {
                    YACK_XMLSUB(xml, "scanning");

                    // initializing search on [0:1]
                    triplet<double> u = { 0, -1,   1 };
                    double H1 = (*this)(u.c);
                    while(H1>=H0)
                    {
                        if( (u.c /= 2) < numeric<double>::epsilon )
                        {
                            std::cerr << "NO DECREASE" << std::endl;
                        }
                        H1 = (*this)(u.c);
                    }
                    const double u_r = u.c;
                    YACK_XMLOG(xml, "-- H=" << H0 << " -> " << H1 << "@u=" << u.c);

                    // optimizing
                    assert(H1<H0);
                    {
                        triplet<double> f = { H0, -1, H1 };
                        optimize::run_for(*this,u,f,optimize::inside);
                        H1 = f.b;
                    }

                    if(u.b<u_r)
                    {
                        YACK_XMLOG(xml, "-- H=" << H0 << " -> " << H1 << "@u=" << u.b);
                    }

                    // study numeric convergence while updading Corg
                    for(const anode *node=working.head;node;node=node->next)
                    {
                        const species &s = **node;
                        const size_t   j = *s;
                        const double   c_old = Corg[j];
                        const double   c_new = Ctry[j];

                        if( fabs(c_old-c_new) > 0 ) converged = false;
                        Corg[j] = c_new;
                    }

                }
                YACK_XMLOG(xml,"-- [Results for cycle #" << cycle << "]");
                YACK_XMLOG(xml,"-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum));
                YACK_XMLOG(xml,"-- usingMaximumDOF = " << yack_boolean(usingMaximumDOF));
                YACK_XMLOG(xml,"-- usingFullLength = " << yack_boolean(usingFullLength));
                YACK_XMLOG(xml,"-- converged       = " << yack_boolean(converged) );

                if(!usingMaximumDOF)
                {
                    goto CYCLE;
                }


                goto CYCLE;

                exit(0);



            }


        }


    }
}
