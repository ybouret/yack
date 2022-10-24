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
#include "yack/arith/round10.hpp"

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

            
            return emax;
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


        bool reactor:: hasDominant(const double H0, const xmlog &xml)
        {
            YACK_XMLSUB(xml,"hasDominant");

            //------------------------------------------------------------------
            //
            // initialize search over groups
            //
            //------------------------------------------------------------------
            writable<double> &Cmin = Cend; // will hold winner C
            const group      *gmin = NULL; // will hold minimal group
            double            Hmin  = H0;  // initial Hamiltonian
            working.transfer(Cmin,Corg);   // initial Cend = Corg

            //------------------------------------------------------------------
            //
            // Loop over solving groups
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
                YACK_XMLOG(xml,"-- at global minimum, no dominant");
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
                writable<double>  &Omi = Omega[ei];
                writable<double>  &psi = Psi[ei];

                Omi.ld(0); Omi[ei] = 1.0;
                if(blocked[ei])
                {
                    NuA[ei].ld(0);
                    psi.ld(0);
                    assert(fabs(Xl[ei])<=0);
                }
                else
                {
                    const double Ki  = K[ei];
                    eq.grad_action(psi,Ki,Corg,xmul);
                    const double            den = sigma[ei]; assert(den<0);
                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej])/den;
                    }

                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej])/den;
                    }
                }
            }
        }

#if 0
        void reactor:: createOmega()
        {

            for(const enode *node=singles.head();node;node=node->next)
            {
                const equilibrium &eq  = ***node;
                const size_t       ei  = *eq;
                writable<double>  &Omi = Omega[ei];

                Omi.ld(0); Omi[ei] = 1.0;
                if(blocked[ei])
                {
                    xi[ei] = 0;
                }
                else
                {
                    xi[ei] = Xl[ei];
                    const readable<double> &psi = Psi[ei];
                    const double            den = sigma[ei]; assert(den<0);

                    for(const enode *scan=node->prev;scan;scan=scan->prev) {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej])/den;
                    }

                    for(const enode *scan=node->next;scan;scan=scan->next) {
                        const size_t ej = ****scan;
                        Omi[ej] = xadd.dot(psi,NuA[ej])/den;
                    }

                }

            }

        }
#endif

        void reactor:: deactivated(const size_t ei)
        {
            NuA[ei].ld(0);
            Psi[ei].ld(0);
            blocked[ei] = true;
            Xl[ei] = Gamma[ei] = sigma[ei] = 0;
        }


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
                    YACK_XMLOG(xml, "-- #dof = " << nrun << " / " << N << " (@level-1)");
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
            const bool atGlobalMinimum = hasDominant(H0,xml);
            if(!atGlobalMinimum)
            {
                YACK_XMLOG(xml,"-- topology: updating");
                emax = topoSingles(nrun,xml);
                YACK_XMLOG(xml,"-- topology: updated!");
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
                        YACK_XMLOG(xml, "-- #dof = " << nrun << " / " << N << " (@level-2)");
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
            bool   consistentState = true;
            prepareStep();

            //------------------------------------------------------------------
            //
            //
            // compute extent using limiting PRIMARY species
            //
            //
            //------------------------------------------------------------------
            unsigned trial = 0;
        COMPUTE_EXTENT:
            ++trial;
            YACK_XMLOG(xml, "-- computing extent [trial #" << trial << "]");
            iOmeg.assign(Omega);

            if( !solv.build(iOmeg,xadd) )
            {
                if( atGlobalMinimum )
                {
                    YACK_XMLOG(xml, "-- singular system");
                    return false;
                }
                YACK_XMLOG(xml, "-- try moving...");
                goto CYCLE;
            }

            iota::load(xi,Xl);
            solv.solve(iOmeg,xi,xadd);

            {
                bool recomputedOmega = false;
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
                        Omega[ei][ei]   *= 10;
                        recomputedOmega  = true;
                        consistentState  = false;
                    }
                    else
                    {

                    }

                }

                if(recomputedOmega)
                {
                    goto COMPUTE_EXTENT;
                }
            }



            //------------------------------------------------------------------
            //
            //
            // checking REPLICA
            //
            //
            //------------------------------------------------------------------

            {
                YACK_XMLSUB(xml, "forwarding");

                bool   invalidSpecies = false;
                xdiag.ld(1);


                for(const anode *node=working.head;node;node=node->next)
                {
                    const species &sp = **node; assert(sp.rank>0);
                    const size_t   j  = *sp;


                    xadd.ldz();
                    for(size_t i=N;i>0;--i) xadd.ld( NuA[i][j] * xi[i] );
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
                    if(verbose) std::cerr << std::endl;

                    if(d<0 && (-d)>c)
                    {
                        invalidSpecies  = true;
                        consistentState = false;
                        std::cerr << "Invalid replica " << sp.name << " !!" << std::endl;

                        const islot &eqs = held_by[j];
                        bool         may = false;
                        for(const inode *scan = eqs.head; scan; scan=scan->next)
                        {
                            const equilibrium   &eq = **scan;
                            const size_t         ei = *eq;    if(blocked[ei]) continue;
                            const double         xx = xi[ei];
                            const int            nu = NuA[ei][j];
                            std::cerr << "\tscanning " << eq.name <<  " @xi=" << xx << ", nu=" << nu << std::endl;

                            if( (xx<=0 && nu>0) || (xx>=0 && nu < 0) )
                            {
                                std::cerr << "\t\tdecreasing xi_" << eq.name << std::endl;
                                xdiag[ei] = 10;
                                may       = true;
                            }
                        }
                        if(!may)
                        {
                            std::cerr << "Couldn't correct [" << sp.name << "] !!" << std::endl;
                        }
                    }
                }

                std::cerr << "xdiag=" << xdiag << std::endl;
                if(invalidSpecies)
                {
                    std::cerr << "Need to recompute extent..." << std::endl;
                    for(size_t i=N;i>0;--i)
                    {
                        if(blocked[i]) continue;
                        Omega[i][i] *= xdiag[i];
                    }
                    goto COMPUTE_EXTENT;
                }

                std::cerr << "done secondary" << std::endl;
            }


            // compute Cend
            for(const anode *node=working.head;node;node=node->next)
            {
                const species &sp = **node; assert(sp.rank>0);
                const size_t   j  = *sp;
                Cend[j]  = max_of( Corg[j] + dC[j], 0.0 );
            }

            if(verbose) corelib(*xml << "-- Cend=","", Cend);

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

            // no overshoot
            double H1 = Hamiltonian(Cend);
            {
                triplet<double> U = { 0, -1 , 1};
                triplet<double> H = { H0, -1, H1 };
                optimize::run_for(*this, U, H, optimize::inside);
                YACK_XMLOG(xml,"-- moving H0=" << H0 << " to H(" << U.b << ")=" << H.b << " : dH=" << H.b - H0 << " @cycle=" << cycle);
                working.transfer(Cend,Ctry);
                H1 = H.b;
            }

            YACK_XMLOG(xml, "-- consistentState = " << yack_boolean(consistentState) );
            YACK_XMLOG(xml, "-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum) );


            if(!consistentState)
            {
                working.transfer(Corg,Cend);
                goto CYCLE;
            }


            // check convergence whilst updating Corg
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


            if(atGlobalMinimum)
            {
                exit(0);
            }



            goto CYCLE;

        }


    }
}
