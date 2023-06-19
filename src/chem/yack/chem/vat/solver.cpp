#include "yack/chem/vat/solver.hpp"
#include <iomanip>

namespace yack
{
    namespace Chemical
    {

        Solver:: Solver(const size_t maxClusterSize,
                        const size_t maximumSpecies) :
        eqsFund( new Equilibrium::Bank() ),
        spcFund( new Species::Bank()     ),
        running( eqsFund ),
        blocked( eqsFund ),
        extents( spcFund ),
        Corg(maximumSpecies,as_capacity),
        Ctmp(maximumSpecies,as_capacity),
        Ceq(maxClusterSize,maximumSpecies),
        Phi(Ceq),
        ams(maxClusterSize,as_capacity),
        xadd(),
        xmul()
        {

        }

        Solver:: ~Solver() noexcept
        {
            
        }

        void  Solver:: run(const xmlog                    &xml,
                           writable<Extended::Real>       &C0,
                           const Cluster                  &cluster,
                           const readable<Extended::Real> &K,
                           const readable<Extended::Real> &S)
        {
            YACK_XMLSUB(xml, "Solver::run");
            const size_t          m = cluster.lib.size;
            const size_t          n = cluster.size;
            const Extended::Real xr0;
            const Aftermath      am0;

            Corg.adjust(m,xr0);
            Ctmp.adjust(m,xr0);
            ams.adjust(n,am0);

            // transfer C
            for(const Species::Node *sn=cluster.lib.head;sn;sn=sn->next)
            {
                const Species &sp = ***sn;
                const size_t   sj = sp.indx[SubLevel];
                Corg[ sj ] = C0[ sp.indx[TopLevel] ];
            }


            running.clear();
            blocked.clear();
            for(const Equilibrium::Node *en=cluster.head;en;en=en->next)
            {
                const Equilibrium        &eq = ***en;
                const size_t              ei = eq.indx[SubLevel];
                const size_t              eI = eq.indx[TopLevel];
                writable<Extended::Real> &Ci = Ceq[ei];
                const Extended::Real      Ki = K[eI];
                const Extended::Real      Si = S[eI];
                const Aftermath          &am = ( ams[ei] = Aftermath::Evaluate(eq,Ki,Si,Ci,Corg,extents,SubLevel,xmul,xadd,Ctmp) );

                if(xml.verbose) cluster.pad(*xml << eq.name,eq) << " : " << Equilibrium::StatusText(am.status);
                switch(am.status)
                {
                    case Equilibrium::Blocked: blocked << eq;
                        if(xml.verbose) xml() << std::endl;
                        continue;
                    case Equilibrium::Running: running << eq; break;
                }

                writable<Extended::Real> &phi = Phi[ei];

                eq.gradAction(phi,SubLevel,Ki,Ci,SubLevel,xmul);
                
                if(xml.verbose)
                {
                    xml() << " @" << std::setw(15) << *am.extent;
                    eq.displayCompact(xml() <<  " -> ",Ci,SubLevel);
                    xml() << std::endl;
                }
            }
        }
    }

}
