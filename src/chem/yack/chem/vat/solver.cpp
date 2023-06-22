#include "yack/chem/vat/solver.hpp"
#include "yack/data/list/sort.hpp"
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
        Xi(maxClusterSize,as_capacity),
        Xa(maxClusterSize,as_capacity),
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


            struct OrderRunning
            {
                const readable<Extended::Real> &Xa;
                int operator()(const Equilibrium::Node *lhs,
                               const Equilibrium::Node *rhs) const
                {
                    const size_t li = (***lhs).indx[SubLevel];
                    const size_t ri = (***rhs).indx[SubLevel];
                    return comparison::decreasing(Xa[li],Xa[ri]);
                }
            };

            YACK_XMLSUB(xml, "Solver::run");
            const size_t          m = cluster.lib.size;
            const size_t          n = cluster.size;
            const Extended::Real  xr0;
            const Aftermath       am0;
            OrderRunning          orderRunning = { Xa };

            Xi.adjust(n,xr0);
            Xa.adjust(n,xr0);
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
                    case Equilibrium::Blocked:
                        blocked << eq;
                        Xi[ei] = Xa[ei]  = xr0;
                        if(xml.verbose) xml() << std::endl;
                        continue;

                    case Equilibrium::Running:
                        running << eq;
                        break;
                }

                writable<Extended::Real> &phi = Phi[ei];
                Xa[ei] = (Xi[ei] = am.extent).abs();

                eq.gradAction(phi,SubLevel,Ki,Ci,SubLevel,xmul);
                
                if(xml.verbose)
                {
                    xml() << " @" << std::setw(15) << *am.extent;
                    eq.displayCompact(xml() <<  " -> ",Ci,SubLevel);
                    xml() << ", q=" << *eq.quotient(xmul, Ki, Ci, SubLevel);
                    xml() << std::endl;
                }
            }

            merge_list_of<Equilibrium::Node>::sort(running,orderRunning);
            if(xml.verbose)
            {
                YACK_XMLOG(xml, "#running= " << running.size);
                for(const Equilibrium::Node *en=running.head;en;en=en->next)
                {
                    const Equilibrium        &eq = ***en;
                    const size_t              ei = eq.indx[SubLevel];
                    cluster.pad(*xml << eq.name,eq) << " @" << std::setw(15) << *Xi[ei] << " = " << Xi[ei] << std::endl;
                }
            }

        }
    }

}
