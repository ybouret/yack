#include "yack/chem/vat/cluster.hpp"
#include "yack/math/algebra/ortho-family.hpp"
#include "yack/system/imported.hpp"
#include "yack/woven/qcompress.hpp"
#include "yack/woven/qbuilder.hpp"

namespace yack
{
    namespace Chemical
    {

        void Cluster:: discoverLaws(const xmlog &xml)
        {
            const size_t nc = lib.size;
            if(nc<=1) return;
            YACK_XMLSUB(xml,"Cluster::discoverLaws");
            woven::zrepo     repo(nc);

            //------------------------------------------------------------------
            //
            // using WOVEn to build possible laws
            //
            //------------------------------------------------------------------
            {
                matrix<int>  Q(nc,nc);
                if(!math::ortho_family::build(Q,Nu,true))
                    throw imported::exception(CLID,"singular topology!!");

                matrix<int>  P;
                const size_t rank = woven::qcompress::build(P,Q);
                if(rank<=0)
                    throw imported::exception(CLID,"unexpected null rank");

                woven::qbuilder  work(nc);
                work(repo,P,rank,true);
            }

            //------------------------------------------------------------------
            //
            // process repo
            //
            //------------------------------------------------------------------
            typedef core_repo<const woven::zvector> zvecs;
            zvecs good;
            for(const woven::zvector *v=repo.head;v;v=v->next)
            {
                size_t nCoef = 0;
                bool   isPos = true;
                for(size_t i=v->size();i>0;--i)
                {
                    switch( (*v)[i].s )
                    {
                        case __zero__:          continue;
                        case positive: ++nCoef; continue;
                        case negative: isPos = false;
                            break;
                    }
                    break;
                }
                if(!isPos||nCoef<=1)  continue;
                good << *v;
            }

            if(good.size<=0)
            {
                YACK_XMLOG(xml,"no possible conservation");
                return;
            }

            //------------------------------------------------------------------
            //
            // create laws
            //
            //------------------------------------------------------------------
            const size_t qn = good.size;
            coerce(Qmat).make(qn,nc);
            {
                size_t i = 1;
                for(const zvecs::node_type *node=good.head;node;node=node->next,++i)
                {
                    writable<unsigned>  &u = coerce(Qmat)[i];
                    const readable<apz> &z = (***node);
                    for(size_t j=nc;j>0;--j)
                    {
                        u[j] = z[j].cast_to<unsigned>("law coefficient");
                    }

                    ConservationLaw &claw = * coerce(laws).push_back( new ConservationLaw() );
                    for(const SpNode *sn=lib.head;sn;sn=sn->next)
                    {
                        const Species &sp = ***sn;
                        const size_t   sj = sp.indx[SubLevel];
                        const unsigned nu = u[sj];
                        if(nu>0)
                        {
                            claw.push_back( new Actor(nu,sp) );
                        }
                    }

                }
            }
            coerce(Qrnk) = apex::flak::rank(Qmat);
            coerce(laws).finalize();
            YACK_XMLOG(xml, " Qmat = " << Qmat);
            YACK_XMLOG(xml, " Qrnk = " << Qrnk);
            YACK_XMLOG(xml, " laws = " << laws);

            assembleActs(xml);

        }


        static inline void tryReduceCanons(Canons &target)
        {
            Canons store;
            while(target.size)
            {
                Canon *rhs = target.pop_front();
                for(Canon *lhs = store.head; lhs; lhs=lhs->next)
                {
                    if(lhs->linkedTo(*rhs))
                    {
                        lhs->merge_back(*rhs);
                        delete rhs;
                        rhs = 0;
                        break;
                    }
                }

                if(rhs)
                    store.push_back(rhs);

            }
            target.swap_with(store);
        }

        void Cluster:: assembleActs(const xmlog &xml)
        {
            YACK_XMLSUB(xml,"Cluster::assembleActs");
            {
                Canons &target = coerce(acts);
                for(const ConservationLaw *node=laws.head;node;node=node->next)
                {
                    bool                   use = false;
                    const ConservationLaw &law = *node;
                    for(Canon *canon=target.head;canon;canon=canon->next)
                    {
                        if(canon->endorses(law))
                        {
                            use = true;
                            *canon << law;
                            break;
                        }
                    }

                    if(use)
                    {
                        tryReduceCanons(target);
                    }
                    else
                    {
                        target.push_back( new Canon(law) );
                    }
                }
            }
            if(xml.verbose)
            {
                *xml << "#acts=" << acts.size << std::endl;
                for(const Canon *c=acts.head;c;c=c->next)
                {
                    *xml << *c << std::endl;
                }
            }

        }


    }

}


