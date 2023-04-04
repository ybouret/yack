
#include "yack/chem/realm/reactor.hpp"
#include "yack/chem/eqs/aftermath.hpp"
#include <iomanip>
#include "yack/math/iota.hpp"
#include "yack/apex/alga.hpp"
#include "yack/math/algebra/crout.hpp"
#include "yack/math/opt/optimize.hpp"

namespace yack
{
    using namespace math;

    namespace chemical
    {

        reactor:: ~reactor() noexcept
        {
        }

        reactor:: reactor(const domain &_) :
        spot_object(),
        dom(_),
        xlim(),
        xadd(),
        xmul(),
        Corg(dom.M),
        Cend(dom.M),
        Ctmp(dom.M),
        Korg(dom.L),
        blocked(dom.L),
        running(dom.L),
        Xi(dom.L),
        sigma(dom.L),
        shift(dom.L),
        Cs(dom.L,dom.M),
        Xl(dom.N),
        eqpxy( new eq_zpool() ),
        active(eqpxy),
        subset(eqpxy),
        next(0),
        prev(0)
        {
        }


        double reactor:: excess(const readable<double> &C)
        {
            xadd.free();
            for(const eq_node *en=active.head;en;en=en->next)
            {
                const equilibrium &eq = ***en;
                const size_t       ei = eq.indx[sub_level]; assert(running[ei]); assert(sigma[ei]<0);
                const double       ma = eq.mass_action(sub_level,C,Korg[ei],xmul) - shift[ei];
                xadd.push( fabs(ma/sigma[ei]) );
            }
            return xadd.sum();
        }

        double reactor:: operator()(const double u)
        {
            if(u<=0)
            {
                iota::load(Ctmp,Corg);
                return excess(Corg);
            }
            else
            {
                if(u>=1)
                {
                    iota::load(Ctmp,Cend);
                    return excess(Cend);
                }
                else
                {
                    const double v = 1.0 - u;
                    for(size_t i=Ctmp.size();i>0;--i)
                    {
                        const double c_org = Corg[i];
                        const double c_end = Cend[i];
                        double       c_max = c_org;
                        double       c_min = c_end;
                        if(c_min>c_max) cswap(c_min,c_max);
                        Ctmp[i] = clamp(c_min, c_org *v + c_end * u, c_max);
                    }
                    return excess(Ctmp);
                }
            }
        }


        struct eq_node_comparator
        {
            const readable<double> &Xi;

            inline int operator()(const eq_node *lhs,
                                  const eq_node *rhs) const noexcept
            {
                const size_t l = (***lhs).indx[sub_level];
                const size_t r = (***rhs).indx[sub_level];
                return comparison::increasing_abs(Xi[l],Xi[r]);
            }
        };

        void reactor:: solve(const xmlog            &xml,
                             writable<double>       &C0,
                             const readable<double> &K)
        {

            YACK_XMLSUB(xml, "reactor");
            const eq_node_comparator cmp = { Xi };


            // load Corg into sub_level description
            dom.spmap.send(Corg,C0);
            dom.eqmap.send(Korg,K);


            if(xml.verbose)
            {
                dom.spdisp(*xml << "Corg = ",sub_level,Corg) << std::endl;
                dom.eqdisp(*xml << "Korg = ",sub_level,Korg) << std::endl;
            }

        AFTERMATH:
            // determining all aftermaths
            active.clear();
            for(const eq_node *en=dom.head;en;en=en->next)
            {
                const equilibrium &  eq = ***en;
                const size_t         i  = eq.indx[sub_level];
                writable<double>   & Ci = Cs[i];
                const double         Ki = Korg[i];
                const aftermath      am = aftermath::solve(sub_level,eq,Ki,Corg,Ci,xlim,xmul,xadd);

                switch( am.state )
                {
                    case is_blocked:
                        blocked[i] = true;
                        running[i] = false;
                        Xi[i]      = 0;
                        sigma[i]   = 0;
                        shift[i]   = 0;
                        break;

                    case is_running:
                        blocked[i] = false;
                        running[i] = true;
                        Xi[i]      = am.value;
                        sigma[i]   = eq.slope(sub_level,Ci,Ki,xmul,xadd);    assert(sigma[i]<0);
                        shift[i]   = am.error;
                        active << eq;
                        break;
                }

                if(xml.verbose)
                {
                    dom.eqfmt.pad( *xml << eq, eq) << ": " << am;
                    xml() << ", ma = " << std::setw(15) << eq.mass_action(sub_level,Corg,Ki,xmul);
                    xml() << ", sigma=" << std::setw(15) << sigma[i] << std::endl;
                }
            }

            YACK_XMLOG(xml,"#active = " << active.size << " / " << dom.L << " / rank=" << dom.N);

            // initialize excess
            const double X0 = excess(Corg);
            YACK_XMLOG(xml,"excess0 = " << X0);

            if(X0<=0)
            {
                goto SUCCESS;
            }

            switch(active.size)
            {
                case 0:
                    YACK_XMLOG(xml,"all blocked");
                    goto SUCCESS;

                case 1: {
                    const equilibrium &eq = ***active.head;
                    YACK_XMLOG(xml,"unique " << eq);
                    iota::load(Corg, Cs[eq.indx[sub_level]] );
                    goto AFTERMATH;
                }
                default:
                    break;

            }

            assert(active.size>=2);

            // find optimal
            for(const eq_node *node=active.head;node;node=node->next)
            {
                const equilibrium &eq = ***node;
                const size_t       ei = eq.indx[sub_level];
                writable<double>  &Ci = Cs[ei];
                iota::load(Cend,Ci);
                triplet<double> u = { 0, -1, 1 };
                triplet<double> X = { X0, -1, (*this)(u.c) }; // force loading Ctmp
                std::cerr << "X=" << X << std::endl;
                optimize::run_for(*this,u,X,optimize::inside);
                dom.eqfmt.pad( *xml << eq, eq) << ": " << std::setw(15) << X.b << " @" << u.b << std::endl;
                std::cerr << X.b << " / " << excess(Ctmp) << std::endl;
            }


            exit(0);

        SUCCESS:
            YACK_XMLOG(xml,yack_success);
            dom.spmap.recv(C0,Corg);
            return;

#if 0

            // merging by increasing |Xi|
            merge_list_of<eq_node>::sort(active,cmp);

            if(xml.verbose)
            {
                for(const eq_node *node=active.head;node;node=node->next)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = eq.indx[sub_level];
                    dom.eqfmt.pad( *xml << eq, eq) << ": " << std::setw(15) << Xi[ei] << " " << dom.topo[ei] << std::endl;
                }
            }


            const size_t rmax = min_of(active.size,dom.N);
            const size_t m    = dom.M;
            matrix<apq> Q(dom.N,m);
            subset.clear();
            {
                const eq_node *curr = active.head;
                std::cerr << "init with " << ***curr << std::endl;
                subset << ***curr;
                for(curr=curr->next;curr;curr=curr->next)
                {
                    subset << ***curr;

                    size_t irow=1;
                    Q.ld(0);
                    for(const eq_node *node=subset.head;node;node=node->next,++irow)
                    {
                        writable<apq>       &target = Q[irow];
                        const readable<int> &source = dom.topo[ (***node).indx[sub_level] ];
                        for(size_t j=dom.M;j>0;--j) target[j] = source[j];
                    }

                    if( alga::rank_of(Q) != subset.size )
                    {
                        std::cerr << "discard   " << ***curr << std::endl;
                        subset.cut_tail();
                        continue;
                    }
                    else
                    {
                        std::cerr << "grow with " << ***curr << std::endl;
                        if(subset.size>=rmax) break;
                    }
                }
            }
            std::cerr << "subset.size=" << subset.size << std::endl;

            const size_t n = subset.size;
            math::crout<double> lu(dom.N);

            matrix<int>    localNu(n,m);
            vector<double> localXi(n,0);
            matrix<double> localPhi(n,m);
            vector<double> localSig(n,0);
            vector<double> xxi(n,0);

            {
                size_t irow = 1;
                for(const eq_node *node=subset.head;node;node=node->next, ++irow)
                {
                    const equilibrium &eq = ***node;
                    const size_t       ei = eq.indx[sub_level];
                    xlimits           &xl = Xl[irow];
                    localXi[irow]         = Xi[ei];
                    localSig[irow]        = sigma[ei];
                    iota::load(localNu[irow],dom.topo[ei]);
                    eq.grad(sub_level, localPhi[irow], Corg, K[eq.indx[top_level]], xmul);
                    xl(eq,Corg,dom.reg);
                    dom.eqfmt.pad(std::cerr << eq,eq) << " : " << xl << std::endl;
                }
            }
            std::cerr << "localNu  = " << localNu << std::endl;
            std::cerr << "localXi  = " << localXi << std::endl;
            std::cerr << "localPhi = " << localPhi << std::endl;
            std::cerr << "localSig = " << localSig << std::endl;

            return;


            matrix<double> Omega(n,n);
            for(size_t i=n;i>0;--i)
            {
                const double           den  = localSig[i];
                const readable<double> &phi = localPhi[i];
                writable<double>       &omi = Omega[i];
                omi[i] = 1.0;
                for(size_t j=n;j>i;--j)   omi[j] = xadd.dot(phi,localNu[j])/den;
                for(size_t j=i-1;j>0;--j) omi[j] = xadd.dot(phi,localNu[j])/den;

            }

            std::cerr << "Omega=" << Omega << std::endl;
            if( !lu.build(Omega,xadd) )
            {
                exit(0);
            }

            iota::load(xxi,localXi);
            lu.solve(Omega,xxi);
            std::cerr << "xxi=" << xxi << std::endl;

#endif

        }

    }

}
