#include "yack/chem/reactor.hpp"
#include "yack/chem/outcome.hpp"
#include "yack/system/imported.hpp"
#include "yack/type/utils.hpp"
#include "yack/math/opt/optimize.hpp"
#include "yack/math/opt/locate.hpp"
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

        double reactor:: Hamiltonian(const group &g)
        {
            //------------------------------------------------------------------
            // build a trial concentration
            //------------------------------------------------------------------
            iota::load(Ctry,Corg);
            for(const gnode *ep=g.head;ep;ep=ep->next)
            {
                const equilibrium &member = **ep;
                member.transfer(Ctry,Ceq[*member]);
            }

            //------------------------------------------------------------------
            // compute it's hamiltonian
            //------------------------------------------------------------------
            return Hamiltonian(Ctry);
        }


        const readable<double> & reactor:: Optimized_C(const double H0)
        {
            triplet<double> U = { 0,  -1, 1.0 };
            triplet<double> H = { H0, -1, Hamiltonian(Cend) };
            optimize::run_for(*this, U, H, optimize::inside);
            return Ctry;
        }


        const equilibrium * reactor:: topoSingles(size_t &nrun, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"topoSingles");

            //------------------------------------------------------------------
            //
            // initialize
            //
            //------------------------------------------------------------------
            const equilibrium *emax = NULL;
            double             amax = 0;
            outcome            ppty;
            nrun = 0;

            //------------------------------------------------------------------
            //
            // loop over singles
            //
            //------------------------------------------------------------------
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Ceq[ei];
                const double       Ki  = K[ei];
                const outcome      oc  = outcome::study(eq, Ki, Corg, Ci, xmul, xadd);
                writable<double>  &psi = Psi[ei];

                // Ceq is now computed
                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true; // update state
                        Xl[ei]      = 0;    // for consistency
                        sigma[ei]   = 0;    // for consistency
                        psi.ld(0);          // for consistency
                        break;

                    case components::are_running: {
                        ++nrun;
                        const double ax = fabs( Xl[ei] = *oc );
                        if(ax>amax)
                        {
                            amax =  ax;
                            emax = &eq;
                            ppty =  oc;
                        }
                        eq.grad_action(psi,Ki,Ci,xmul);
                        blocked[ei] = false;
                        sigma[ei]   = xadd.dot(psi, Nu[ei]);
                        if(sigma[ei]>=0) throw imported::exception(clid,"corrupted <%s>",eq.name());
                    } break;
                }

                if(verbose) {
                    lattice.pad(*xml << "-- (+) " << '<' << eq.name << '>', eq) << " : " << oc << " @sigma= " << sigma[ei] << std::endl;
                }
            }

            if(emax)
            {
                if(extent::is_degenerated==ppty.grade)
                {
                    return NULL;
                }
                else
                {
                    return emax;
                }
            }
            else
            {
                return NULL;
            }

        }


        void reactor:: topoLattice(const double H0, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"topoLattice");

            //------------------------------------------------------------------
            //
            // precomputed single
            //
            //------------------------------------------------------------------
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  =     *eq; if(blocked[ei]) continue;
                writable<double>  &Ci  = Ceq[ei];

                working.transfer(Cend,Ci);            // look within Corg:Ceq[ei]
                working.transfer(Ci,Optimized_C(H0)); // store optimized value

            }

            //------------------------------------------------------------------
            //
            // full computation for couples
            //
            //------------------------------------------------------------------
            for(const enode *node=couples.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Ci  = Ceq[ei];
                const double       Ki  = Kl[ei];
                const outcome      oc  = outcome::study(eq, Ki, Corg, Cend, xmul, xadd);

                // Cend holds the equilibrium value
                switch(oc.state)
                {
                    case components::are_blocked:
                        blocked[ei] = true;  // blocked
                        iota::load(Ci,Corg); // at Cend=Corg
                        break;

                    case components::are_running: {
                        blocked[ei] = false;            // running
                        iota::load(Ci,Optimized_C(H0)); // store optimized in Corg:Cend
                    } break;
                }

                if(verbose) {
                    lattice.pad(*xml << "-- (+) " << '<' << eq.name << '>', eq) << " : " << oc << std::endl;
                }
            }
        }


        bool reactor:: foundGlobal(const double H0, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"studyGlobal");

            //------------------------------------------------------------------
            //
            // initialize search over groups
            //
            //------------------------------------------------------------------
            writable<double> &Cmin = Cend; // will hold winner C
            const group      *gmin = NULL; // will hold minimal group
            double            Hmin  = H0;  // initial Hamiltonian
            working.transfer(Cmin,Corg);   // initial C

            //------------------------------------------------------------------
            //
            // Loop over groups
            //
            //------------------------------------------------------------------
            for(const group *g=solving.head;g;g=g->next)
            {
                assert(g->is_ortho());
                const double Htry = Hamiltonian(*g);
                if(Htry<Hmin)
                {
                    YACK_XMLOG(xml,"-->" <<  std::setw(15) << Htry << " @" << *g);
                    gmin = g;
                    Hmin = Htry;
                    working.transfer(Cend,Ctry);
                }

            }

            if(gmin)
            {
                assert( fabs(Hamiltonian(Cmin)-Hmin) <=0 );
                working.transfer(Corg,Cmin);
                YACK_XMLOG(xml,"--[" <<  std::setw(15) << Hmin << " @" << *gmin << " ]");
                return false;
            }
            else
            {
                YACK_XMLOG(xml,"-- at global minimum " << *gmin);
                return true;
            }

        }


        void reactor:: prepareStep()
        {
            NuA.assign(Nu);
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;

                if(blocked[ei])
                {
                    NuA[ei].ld(0);
                    Psi[ei].ld(0);
                    Gamma[ei] = 0;
                }
                else
                {
                    const double Ki  = K[ei];
                    eq.grad_action( Psi[ei],Ki,Corg,xmul);
                    Gamma[ei] = (fabs(Xl[ei]) <= 0) ? 0 : eq.mass_action(Ki,Corg,xmul);
                }
            }
        }

        void reactor:: createOmega()
        {
            
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Omi = Omega[ei];

                Omi.ld(0);

                if(blocked[ei]) {
                    Omi[ei] = 1.0;
                }
                else {
                    const readable<double> &psi = Psi[ei];
                    for(const enode *scan=singles.head();scan;scan=scan->next) {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej]);
                    }
                }
            }
        }



        bool reactor:: solve(writable<double> &C0)
        {
            static const char fn[] = "[reactor]";
            const xmlog       xml(fn,std::cerr,entity::verbose);
            YACK_XMLSUB(xml,"solving");
            if(verbose) corelib(*xml << "-- Cini=","", C0);

            optimize::verbose = verbose;
            locate::verbose   = verbose;

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


                default: YACK_XMLOG(xml, "-- start with #eqs=" << N);
                    //----------------------------------------------------------
                    // initialize consistent state
                    //----------------------------------------------------------
                    for(size_t i=M;i>0;--i)
                    {
                        Corg[i] = Cend[i] = Ctry[i] = Csav[i] = C0[i];
                        dC[i]   = 0;
                    }
            }


            unsigned cycle = 0;

        CYCLE:
            ++cycle;
            YACK_XMLOG(xml,"-------- cycle " << cycle << " --------");
            if(verbose)
            {
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s = **node;
                    const size_t   j = *s;
                    corelib.pad(*xml << "[" << s.name << "]",s) << " = " << std::setw(15) << Corg[j] << std::endl;
                }
            }

            //------------------------------------------------------------------
            //
            //
            // getting topology from singles
            //
            //
            //------------------------------------------------------------------
            size_t             nrun = 0;
            const equilibrium *emax = topoSingles(nrun,xml);;

            if(!emax)
            {
                YACK_XMLOG(xml, "-- success");
                return returnSolved(C0,xml);
            }


            switch (nrun) {
                case 0:
                    YACK_XMLOG(xml, "-- all blocked (@level-1)");
                    return returnSolved(C0,xml);

                case 1:
                    YACK_XMLOG(xml, "-- only {" << emax->name << "} (@level-1)");
                    working.transfer(Corg,Ceq[**emax] );
                    goto CYCLE;

                default:
                    YACK_XMLOG(xml, "-- #dof = " << nrun << " (@level-1)");
                    break;
            }



            //------------------------------------------------------------------
            //
            //
            // initialize Hamiltonian
            //
            //
            //------------------------------------------------------------------
            double H0  = Hamiltonian(Corg);
            YACK_XMLOG(xml, "-- H0   = " << std::setw(15) << H0 << " M (initialized)");
            if(H0<=0)
            {
                YACK_XMLOG(xml, "-- success: H0<=0 (@level-1)");
                return returnSolved(C0,xml);
            }


            //------------------------------------------------------------------
            //
            //
            // query lattice status (pre-computed singles and couples)
            //
            //
            //------------------------------------------------------------------
            topoLattice(H0,xml);


            //------------------------------------------------------------------
            //
            //
            // optimise global step
            //
            //
            //------------------------------------------------------------------
            working.transfer(Csav,Corg);
            const bool atGlobalMinimum = foundGlobal(H0,xml);
            if(!atGlobalMinimum)
            {
                YACK_XMLOG(xml,"-- updating topology...");
                emax = topoSingles(nrun,xml);
                YACK_XMLOG(xml,"-- ...updated");
                if(!emax)
                {
                    YACK_XMLOG(xml, "-- success");
                    return returnSolved(C0,xml);
                }


                switch (nrun) {
                    case 0:
                        YACK_XMLOG(xml, "-- all blocked (@level-2)");
                        return returnSolved(C0,xml);

                    case 1:
                        YACK_XMLOG(xml, "-- only {" << emax->name << "} (@level-2)");
                        working.transfer(Corg,Ceq[**emax] );
                        goto CYCLE;

                    default:
                        YACK_XMLOG(xml, "-- #dof = " << nrun << " (@level-2)");
                        break;
                }

                H0  = Hamiltonian(Corg);
                YACK_XMLOG(xml, "-- H0   = " << std::setw(15) << H0 << " M (updated)");
                if(H0<=0)
                {
                    YACK_XMLOG(xml, "-- success: H0<=0 (@level-2)");
                    return returnSolved(C0,xml);
                }
            }


            //------------------------------------------------------------------
            //
            //
            // prepare local step
            //
            //
            //------------------------------------------------------------------
            YACK_XMLOG(xml, "-- preparing local step");
            prepareStep();
            bool usingMaximumDOF = true;

            //------------------------------------------------------------------
            //
            // compute Jacobian
            //
            //------------------------------------------------------------------
            unsigned trial = 0;
        COMPUTE_EXTENT:
            ++trial;
            YACK_XMLOG(xml, "-- computing extent [trial #" << trial << "]");
            createOmega();
            
            //------------------------------------------------------------------
            //
            // compute extent
            //
            //------------------------------------------------------------------
            if( !solv.build(Omega,xadd) )
            {
                if( atGlobalMinimum )
                {
                    YACK_XMLOG(xml, "-- singular system");
                    return false;
                }
                YACK_XMLOG(xml, "-- try moving...");
                goto CYCLE;
            }

            iota::neg(xi,Gamma);
            solv.solve(Omega,xi,xadd);


            //------------------------------------------------------------------
            //
            // check PRIMARY limits
            //
            //------------------------------------------------------------------
            bool               recomputeOmega = false;
            const equilibrium *accepted       = NULL;
            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = *eq;      if(blocked[ei]) continue;
                const double       xx = xi[ei];
                const xlimits     &lm = eq.primary_limits(Corg,corelib.maxlen);
                const bool         ok = lm.acceptable(xx);

                if(verbose)
                {
                    std::cerr << (ok?"[+]":"[-]");
                    singles.pad(std::cerr<< ' ' << eq.name,eq) << ": " << std::setw(15) << xx << ' ';
                    std::cerr << lm << std::endl;
                }

                if(!ok)
                {
                    assert(nrun>0);
                    assert(!blocked[ei]);
                    --nrun;
                    NuA[ei].ld(0);
                    Psi[ei].ld(0);
                    blocked[ei] = true;
                    Xl[ei] = Gamma[ei] = sigma[ei] = 0;
                    recomputeOmega = true;
                }
                else
                {
                    accepted = &eq;
                }
            }

            if(recomputeOmega)
            {
                YACK_XMLOG(xml, "-- reducing complexity");
                YACK_XMLOG(xml, "-- #dof = " << nrun );
                switch(nrun)
                {
                    case 0:
                        assert(!accepted);
                        goto CYCLE; // try again

                    case 1:
                        assert(accepted); assert( !blocked[ **accepted ] );
                        working.transfer(Corg, Ceq[ **accepted ]);
                        goto CYCLE; // try again

                    default:
                        break;
                }

                usingMaximumDOF = false;
                H0              = Hamiltonian(Corg);
                YACK_XMLOG(xml, "-- H0   = " << std::setw(15) << H0 << " M (reduced)");

                goto COMPUTE_EXTENT;
            }

            //------------------------------------------------------------------
            //
            // compute and check deltaC
            //
            //------------------------------------------------------------------
            const bool usingFullLength = forwardingC(xml);
            YACK_XMLOG(xml,"-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum));
            YACK_XMLOG(xml,"-- usingMaximumDOF = " << yack_boolean(usingMaximumDOF));
            YACK_XMLOG(xml,"-- usingFullLength = " << yack_boolean(usingFullLength));

            if(false)
            {
                YACK_XMLOG(xml,"-- saving ham.dat");
                ios::ocstream fp("ham.dat");
                const size_t  np = 1000;
                for(size_t i=0;i<=np;++i)
                {
                    const double u = i/double(np);
                    fp("%.15g %.15g\n", u, (*this)(u) );
                }

            }

            //------------------------------------------------------------------
            //
            // avoid overshoot
            //
            //------------------------------------------------------------------
            double H1 = Hamiltonian(Cend);
            if(usingFullLength)
            {
                // do not overshoot
                triplet<double> U = { 0, -1 , 1};
                triplet<double> H = { H0, -1, H1 };
                optimize::run_for(*this, U, H, optimize::inside);
                if(H.b<1)
                {
                    YACK_XMLOG(xml,"-- moving at H(" << U.b << ")=" << H.b);
                    working.transfer(Cend,Ctry);
                    H1 = H.b;
                }
            }


            //------------------------------------------------------------------
            //
            // check status
            //
            //------------------------------------------------------------------
            if(!usingMaximumDOF)
            {
                //--------------------------------------------------------------
                // not a fully differentiable state
                //--------------------------------------------------------------
                working.transfer(Corg,Cend);
                goto CYCLE;
            }


            bool converged = true;
            {
                YACK_XMLSUB(xml,"checkStatus");
                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &s     = **node;
                    const size_t   j     = *s;
                    const double   c_old = Csav[j];
                    const double   c_new = Cend[j];
                    const double   delta =  fabs(c_old-c_new);
                    const double   limit =  numeric<double>::ftol * max_of( fabs(c_old), fabs(c_new) );
                    if(verbose)
                    {
                        corelib.pad(*xml << "[" << s.name << "]",s);
                        std::cerr << " : "  << std::setw(15) << c_old;
                        std::cerr << " -> " << std::setw(15) << c_new;
                        std::cerr << " |" << delta << "/" << limit << "|";
                        std::cerr << std::endl;
                    }
                    if(delta>limit) converged=false;
                    Corg[j] = c_new;
                }
            }
            YACK_XMLOG(xml,"-- converged       = " << yack_boolean(converged));
            if(converged)
            {
                return returnSolved(C0,xml);
            }

            goto CYCLE;

        }


    }
}
