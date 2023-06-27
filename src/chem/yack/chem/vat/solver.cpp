#include "yack/chem/vat/solver.hpp"
#include "yack/data/list/sort.hpp"
#include "yack/apex/flak.hpp"
#include "yack/math/keto.hpp"
#include "yack/math/algebra/svd.hpp"
#include <iomanip>

namespace yack
{
    using namespace math;

    namespace Chemical
    {

        Solver:: Solver(const size_t maxClusterSize,
                        const size_t maximumSpecies) :
        eqsFund( new Equilibrium::Bank() ),
        spcFund( new Species::Bank()     ),
        running( eqsFund ),
        blocked( eqsFund ),
        founder( eqsFund ),
        extents( spcFund ),
        Xi(maxClusterSize,as_capacity),
        Xa(maxClusterSize,as_capacity),
        Corg(maximumSpecies,as_capacity),
        Ctmp(maximumSpecies,as_capacity),
        dC(maximumSpecies,as_capacity),
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
            dC.adjust(m,xr0);
            ams.adjust(n,am0);

            // transfer C
            for(const Species::Node *sn=cluster.lib.head;sn;sn=sn->next)
            {
                const Species &sp = ***sn;
                const size_t   sj = sp.indx[SubLevel];
                Corg[ sj ] = C0[ sp.indx[TopLevel] ];
            }

            unsigned cycle = 0;
        CYCLE:
            ++cycle;
            YACK_XMLOG(xml, "-------- cycle=" << cycle << " --------");
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
                        coerce(eq.indx[AuxLevel]) = running.size;
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


            merge_list_of<Equilibrium::Node>::sort<Equilibrium::CoopRepo,OrderRunning>(running,orderRunning);

            if(xml.verbose)
            {
                YACK_XMLOG(xml, "#running= " << running.size << " / rank=" << cluster.rank);
                for(const Equilibrium::Node *en=running.head;en;en=en->next)
                {
                    const Equilibrium        &eq = ***en;
                    const size_t              ei = eq.indx[SubLevel];
                    cluster.pad(*xml << eq.name,eq) << " @" << std::setw(15) << *Xi[ei] << ", nu=" <<cluster.Nu0[ei]<<std::endl;
                }
            }


            const size_t nrun = running.size;
            if(nrun<=0)
            {
                // all blocked: Ok!
                return;
            }

            buildFounder(xml,cluster);

            const size_t nf = founder.size;
            matrix<Extended::Real> Omega(nf,nf);
            vector<Extended::Real> XStar(nf,0);

            for(const Equilibrium::Node *lhs=founder.head;lhs;lhs=lhs->next)
            {
                const Equilibrium              &I   = ***lhs;
                const size_t                    i   = I.indx[AuxLevel];
                const size_t                    ii  = I.indx[SubLevel];
                const readable<Extended::Real> &phi = Phi[ii];
                writable<Extended::Real>       &omi = Omega[i];
                const Extended::Real            den = I.dot(phi,SubLevel,xadd);
                omi[i] = 1;
                XStar[i] = Xi[ ii ];
                //std::cerr << "den=" << den << std::endl;
                for(const Equilibrium::Node *rhs=founder.head;rhs;rhs=rhs->next)
                {
                    const Equilibrium &J = ***rhs;
                    const size_t       j = J.indx[AuxLevel];
                    if(i==j) continue;
                    omi[j] = J.dot(phi,SubLevel,xadd) / den;
                }

            }

            std::cerr << "Omega=" << Omega << std::endl;
            std::cerr << "XStar=" << XStar << std::endl;

            matrix<Extended::Real> V(nf,nf);
            vector<Extended::Real> w(nf,0);
            svd<double> SVD(nf);

            if( !SVD.build(Omega,w,V) )
            {
                std::cerr << "No SVD" << std::endl;
                exit(0);
            }
            std::cerr << "w=" << w << std::endl;
            vector<Extended::Real> XX(nf,0);
            SVD.solve(Omega,w,V,XX,XStar);
            std::cerr << "XX = " << XX << std::endl;

            for(size_t j=m;j>0;--j)
            {
                xadd.free();
                xadd.push(Corg[j]);
                for(const Equilibrium::Node *node=founder.head;node;node=node->next)
                {
                    const Equilibrium   &eq = ***node;
                    const Extended::Real xi = XX[eq.indx[AuxLevel]];
                    const Extended::Real dc = cluster.Nu0[ eq.indx[SubLevel] ][j] * xi;
                    xadd.push(dc);
                }
                Ctmp[j] = xadd.reduce();
                dC[j]   = Ctmp[j] - Corg[j];
                Corg[j] = Ctmp[j];
            }
            cluster.for_each_species(std::cerr << "dC=", "\td_[",dC, "]",SubLevel) << std::endl;
            cluster.for_each_species(std::cerr << "CC=", "\t  [",Ctmp, "]",SubLevel) << std::endl;


            goto CYCLE;


        }

        static inline
        size_t founderToMatrix(matrix<apq>                 &qfam,
                             const Equilibrium::CoopRepo &founder,
                               const matrix<int>           &Nu0)
        {
            size_t i=0;
            for(const Equilibrium::Node *node=founder.head;node;node=node->next)
            {
                const Equilibrium &eq = ***node;
                keto::load(qfam[++i],Nu0[ eq.indx[SubLevel]]);
            }
            return i;
        }

        void Solver:: buildFounder(const xmlog &xml, const Cluster &cluster)
        {
            assert(running.size>0);
            YACK_XMLSUB(xml,"selecting founder");
            founder.clear();
            const size_t rmax = min_of(running.size,cluster.rank);
            matrix<apq>  qfam(rmax,cluster.lib.size);

            const Equilibrium::Node *en = running.head;
            while(true)
            {
                const size_t       i  = founderToMatrix(qfam,founder,cluster.Nu0);
                const size_t       r  = i+1;
                const Equilibrium &eq = ***en;
                keto::load(qfam[r],cluster.Nu0[eq.indx[SubLevel]]);
                if(apex::flak::rank_of(qfam) == r)
                {
                    founder << eq; assert(founder.size == r);
                    if(xml.verbose)
                    {
                        cluster.pad(*xml << " (+) " << eq.name,eq) << " @" << std::setw(15) << *Xi[eq.indx[SubLevel]] << std::endl;
                    }
                    if(r>=rmax)
                        break;

                }
                if(NULL==(en = en->next)) break;
            }

            {
                size_t i=0;
                for(const Equilibrium::Node *node=founder.head;node;node=node->next)
                {
                    coerce( (***node).indx[AuxLevel] ) = ++i;
                }
            }


        }
    }

}
