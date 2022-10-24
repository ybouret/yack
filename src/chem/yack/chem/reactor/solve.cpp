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
        void reactor:: deactivated(const size_t ei)
        {
            NuA[ei].ld(0);
            Psi[ei].ld(0);
            blocked[ei] = true;
            Xl[ei] = Gamma[ei] = sigma[ei] = 0;
        }
#endif

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
            std::cerr << "Omega=" << Omega << std::endl;
            iOmeg.assign(Omega);
            if(trial>=100)
            {
                exit(0);
            }

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

                        const islot     &hEqs = held_by[j];
                        bool             corr = false;
                        for(const inode *scan = hEqs.head; scan; scan=scan->next)
                        {
                            const equilibrium   &eq = **scan;
                            const size_t         ei = *eq;    if(blocked[ei]) continue;
                            const double         xx = xi[ei];
                            const int            nu = NuA[ei][j];

                            if( (xx<=0 && nu>0) || (xx>=0 && nu < 0) )
                            {
                                xdiag[ei] = 10;
                                corr      = true;
                            }
                        }

                        if(!corr)
                        {
                            YACK_XMLOG(xml, "couldn't correct [" << sp.name << "] !!");
                            if(atGlobalMinimum)
                            {
                                return false;
                            }
                            else
                            {
                                goto CYCLE;
                            }
                        }
                    }
                }

                if(invalidSpecies)
                {
                    for(size_t i=N;i>0;--i)
                    {
                        if(blocked[i]) continue;
                        Omega[i][i] *= xdiag[i];
                    }
                    goto COMPUTE_EXTENT;
                }

            }

            //------------------------------------------------------------------
            //
            // compute Cend
            //
            //------------------------------------------------------------------
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

            //------------------------------------------------------------------
            //
            // no overshoot
            //
            //------------------------------------------------------------------
            double H1 = Hamiltonian(Cend);
            {
                triplet<double> U = {  0, -1 , 1 };
                triplet<double> H = { H0, -1, H1 };
                optimize::run_for(*this, U, H, optimize::inside);
                YACK_XMLOG(xml,"-- moving H0=" << H0 << " to H(" << U.b << ")=" << H.b << " : dH=" << H.b - H0 << " @cycle=" << cycle);
                working.transfer(Corg,Ctry);
                H1 = H.b;
            }

            //------------------------------------------------------------------
            //
            // check what happened
            //
            //------------------------------------------------------------------
            const double dH = H1 - H0;

            YACK_XMLOG(xml, "-- consistentState = " << yack_boolean(consistentState) );
            YACK_XMLOG(xml, "-- atGlobalMinimum = " << yack_boolean(atGlobalMinimum) );
            YACK_XMLOG(xml, "-- dH              = " << dH );



            if(!consistentState)
            {
                // needed to be adjusted : not good
                goto CYCLE;
            }
            else
            {
                // consistent state
                if(atGlobalMinimum || dH >= 0)
                {
                    return returnSolved(C0,xml);
                }
                else
                {
                    goto CYCLE;
                }

            }



        }


    }
}
